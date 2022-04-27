

#include <stdio.h>
#include <Windows.h>

#include <stdarg.h>

#include <wingdi.h>
#include <GL/gl.h>			/* OpenGL header file */
//#include <GL/glu.h> // Not essential?			/* OpenGL utilities header file */

#include "base.h"

#include "math.h"
#include "random.h"
#include "game.h"
static game_state globalGame = {};

#include "renderer.h"
static renderer_state globalRenderer = {};
#include "renderer.cpp"

#define CREATE_CONSOLE false

struct button_state {
	b32 isDown;
	s32 transitionCount;
};

b32 ButtonWentDown(button_state *b){
	if (b->isDown && (b->transitionCount % 2)){
		return true;
	}
	return false;
}
b32 ButtonWentUp(button_state *b){
	if (!b->isDown && (b->transitionCount % 2)){
		return true;
	}
	return false;
}

struct keyboard_input{
	union{
		button_state asArray[48];
		struct{
			button_state letters[26];
			button_state numbers[10];
			button_state escape;
			button_state enter;
			button_state space;
			button_state shift;
			button_state control;
			button_state backspace;
			button_state alt;
			button_state tab;
			button_state arrowLeft;
			button_state arrowRight;
			button_state arrowUp;
			button_state arrowDown;
		};
	};
};
struct input_state {
	keyboard_input keyboard;
	button_state mouseButtons[5];
	v2 mousePos;
	v2 windowDim;
	s32 mouseWheel;
};

void UpdateButtonState(button_state *b, b32 wentDown){
	if (b->isDown && !wentDown){
		b->isDown = false;
		b->transitionCount++;
	}else if (!b->isDown && wentDown){
		b->isDown = true;
		b->transitionCount++;
	}
}



static HANDLE globalStdHandle = {};

static LARGE_INTEGER globalPerformanceFrequency;
static b32 globalRunning;
static input_state globalInput = {};
static HCURSOR globalCursor;
static char globalExePath[1024] = {};


void DebugPrint(char *str){
	//OutputDebugStringA(str);
	WriteFile(globalStdHandle, str, (DWORD)strlen(str), 0, 0);
}

void DebugPrintf(char *format, ...){
	va_list argptr;
	va_start(argptr, format);

	char localStr[1024];
	vsprintf_s(localStr, sizeof(localStr), format, argptr);

	va_end(argptr);

	localStr[1023] = 0; // null-terminate
	DebugPrint(localStr);
}

LARGE_INTEGER GetCurrentTimeCounter(){
	LARGE_INTEGER result = {};
	QueryPerformanceCounter(&result);
	return result;
}

f32 GetSecondsElapsed(LARGE_INTEGER t0, LARGE_INTEGER t1){
	f32 result = (f32)(t1.QuadPart - t0.QuadPart) / (f32)globalPerformanceFrequency.QuadPart;
	return result;
}

v2 GetWindowDimension(HWND window){
	RECT rect = {};
	GetClientRect(window, &rect);

	v2 result = {(f32)(rect.right - rect.left), (f32)(-rect.top + rect.bottom)};
	return result;
}

u8 *AllocateMemory(size_t size){
	return (u8 *)malloc(size);
}
void DeallocateMemory(void * ptr){
	free(ptr);
}
struct read_file_result{
	u8 *base;
	size_t size;
};
read_file_result ReadEntireFile(char *path, b32 relative){
	read_file_result result = {};
	char finalPath[1024] = {};
	if (relative){
		sprintf_s(finalPath, ArrayCount(finalPath), "%s%s", globalExePath, path);
	}else{
		sprintf_s(finalPath, ArrayCount(finalPath), "%s%s", path);
	}
	
	HANDLE file = CreateFile(finalPath, GENERIC_READ, 0, 0, OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (file){
		LARGE_INTEGER fileSize;
		BOOL success = GetFileSizeEx(file, &fileSize);
		if (success){
			result.base = AllocateMemory(fileSize.QuadPart);
			success = ReadFile(file, result.base, fileSize.QuadPart, 0, 0);
			if (success){
				result.size = fileSize.QuadPart;
			}else{
				DeallocateMemory(result.base);
			}
		}
	}

	return result;
}


LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
	{
        PostQuitMessage(0);
		globalRunning = false;
        return 0;
	}

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(window, &ps);

        //FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
		//SwapBuffers(hdc);

        EndPaint(window, &ps);
		return 0;
    }

	case WM_SETCURSOR:
	{
		

		v2 winDim = GetWindowDimension(window);
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		ScreenToClient(window, &mousePoint);
		v2 mousePos = {(f32)(s32)mousePoint.x, (f32)(s32)(winDim.y - mousePoint.y)};

		if (mousePos.x >= 0 && mousePos.x < winDim.x && mousePos.y >= 0 && mousePos.y < winDim.y){
			SetCursor(globalCursor);
			return 0;
		}else{
			return DefWindowProc(window, msg, wParam, lParam);
		}
	}

    }

    return DefWindowProc(window, msg, wParam, lParam);
}

static s32 globalTextureCount = 0;
void LoadTexture(loaded_image *image){
	Assert(image->texId == 0);

	//s32 texId = globalTextureCount++;
	globalTextureCount++;
	GLuint texId = globalTextureCount;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (s32)image->dim.x, (s32)image->dim.y,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, image->base);
	image->texId = (s32)texId;
}

#include "game.cpp"


void RenderCommands(v2 windowDim){
	glViewport(0,0, windowDim.x, windowDim.y);
	
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	f32 a = (windowDim.x ? 2.0f/windowDim.x : 1.f);
	f32 b = (windowDim.y ? 2.0f/windowDim.y : 1.f);
	f32 proj[] = {    a,     0,     0,    0,
				      0,     b,     0,    0,
				      0,     0,   1.f,    0,
				   -1.f, -1.0f,     0,  1.f };
	glLoadMatrixf(proj);
	
	glLineWidth(1.f);

	
	for(s32 z = 0; z < NUM_RENDER_DEPTHS; z++){
		render_command *commands = globalRenderer.commands[z];
		s32 numCommands = globalRenderer.numCommands[z];
		for(s32 commandIndex = 0; commandIndex < numCommands; commandIndex++){
			render_command *command = &commands[commandIndex];

			switch(command->type){
			case Render_Clear:
			{
				glDisable(GL_TEXTURE_2D);
				auto c = (render_command_clear *)command->data;

				glClearColor(c->color.r, c->color.g, c->color.b, c->color.a);
				glClear(GL_COLOR_BUFFER_BIT);

			} break;


			case Render_TexQuad:
			{
				auto c = (render_command_tex_quad *)command->data;
				
				glBindTexture(GL_TEXTURE_2D, c->tex->texId);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBegin(GL_TRIANGLES);


				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				glTexCoord2f(c->texP[0].x, c->texP[0].y);
				glVertex2f(c->p[0].x, c->p[0].y); // Triangle 1
				glTexCoord2f(c->texP[1].x, c->texP[1].y);
				glVertex2f(c->p[1].x, c->p[1].y);
				glTexCoord2f(c->texP[2].x, c->texP[2].y);
				glVertex2f(c->p[2].x, c->p[2].y);
				glTexCoord2f(c->texP[0].x, c->texP[0].y);
				glVertex2f(c->p[0].x, c->p[0].y); // Triangle 2
				glTexCoord2f(c->texP[2].x, c->texP[2].y);
				glVertex2f(c->p[2].x, c->p[2].y);
				glTexCoord2f(c->texP[3].x, c->texP[3].y);
				glVertex2f(c->p[3].x, c->p[3].y);

				glEnd();

			} break;

			case Render_String:
			{
				auto c = (render_command_string *)command->data;

				s32 lineHeight = 32;
				
				glBindTexture(GL_TEXTURE_2D, c->fontTex->texId);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBegin(GL_TRIANGLES);

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				
				v2 p = c->p;
				f32 xAdvance = (20.f - 2.f)*c->scale.x;
				for(char *it = c->str; *it; it++){
					char chr = *it;
					if (chr >= 32 && chr <= 126){
						// Break line
						if (c->maxWidth > 0 && (p.x + xAdvance > c->p.x + c->maxWidth*c->scale.x)){
							p = V2(c->p.x, p.y - lineHeight*c->scale.y);
						}
						
						v2 draw0 = p - V2(1.f*c->scale.x, 9.f*c->scale.y);
						if (c->roundPos){
							draw0 = V2(Round(draw0.x), Round(draw0.y));
						}
						v2 draw1 = draw0 + V2(20.f*c->scale.x, 32.f*c->scale.y);

						v2 tex0 = V2((2.f + (20.f + 2.f)*((chr - 32) % 32)) / c->fontTex->dim.x,
									 ((2.f + 32.f)*(2.f - (chr - 32) / 32)) / c->fontTex->dim.y);
						v2 tex1 = tex0 + V2(20.f/c->fontTex->dim.x, 32.f/c->fontTex->dim.y);
						
						glTexCoord2f(tex0.x, tex0.y); // Triangle 1
						glVertex2f(draw0.x, draw0.y);
						glTexCoord2f(tex0.x, tex1.y);
						glVertex2f(draw0.x, draw1.y);
						glTexCoord2f(tex1.x, tex1.y);
						glVertex2f(draw1.x, draw1.y);
						glTexCoord2f(tex0.x, tex0.y); // Triangle 2
						glVertex2f(draw0.x, draw0.y);
						glTexCoord2f(tex1.x, tex1.y);
						glVertex2f(draw1.x, draw1.y);
						glTexCoord2f(tex1.x, tex0.y);
						glVertex2f(draw1.x, draw0.y);

						p.x += xAdvance;

					}else if (chr == '\n'){
						p = V2(c->p.x, p.y - lineHeight*c->scale.y);
					}
				}
				
				glEnd();

			} break;
			
			case Render_Rect:
			{
				auto c = (render_command_rect *)command->data;

				glDisable(GL_TEXTURE_2D);
				//glBegin(GL_TRIANGLES);

				//glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				//glVertex2f(c->min.x, c->min.y); // Triangle 1
				//glVertex2f(c->min.x, c->max.y);
				//glVertex2f(c->max.x, c->max.y);
				//glVertex2f(c->min.x, c->min.y); // Triangle 2
				//glVertex2f(c->max.x, c->max.y);
				//glVertex2f(c->max.x, c->min.y);

				glBegin(GL_QUADS);
				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				glVertex2f(c->min.x, c->min.y); // Quad 1
				glVertex2f(c->min.x, c->max.y);
				glVertex2f(c->max.x, c->max.y);
				glVertex2f(c->max.x, c->min.y);

				glEnd();
			} break;
			
			case Render_ColoredQuad:
			{
				auto c = (render_command_colored_quad *)command->data;

				glDisable(GL_TEXTURE_2D);
	
				glBegin(GL_QUADS);
				glColor4f(c->color[0].r, c->color[0].g, c->color[0].b, c->color[0].a);
				glVertex2f(c->p[0].x, c->p[0].y);
				glColor4f(c->color[1].r, c->color[1].g, c->color[1].b, c->color[1].a);
				glVertex2f(c->p[1].x, c->p[1].y);
				glColor4f(c->color[2].r, c->color[2].g, c->color[2].b, c->color[2].a);
				glVertex2f(c->p[2].x, c->p[2].y);
				glColor4f(c->color[3].r, c->color[3].g, c->color[3].b, c->color[3].a);
				glVertex2f(c->p[3].x, c->p[3].y);

				glEnd();
			} break;

				case Render_Triangle:
			{
				auto c = (render_command_triangle *)command->data;

				glDisable(GL_TEXTURE_2D);
				glBegin(GL_TRIANGLES);

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				glVertex2f(c->p[0].x, c->p[0].y);
				glVertex2f(c->p[1].x, c->p[1].y);
				glVertex2f(c->p[2].x, c->p[2].y);

				glEnd();

			} break;

				case Render_Circle:
			{
				auto c = (render_command_circle *)command->data;

				glDisable(GL_TEXTURE_2D);
				glBegin(GL_TRIANGLES);

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);

				v2 vertex = c->o + V2(c->r, 0);
				s32 numSides = 32;
				for(s32 i = 1; i <= numSides; i++){
					v2 nextVertex = c->o + V2LengthDir(c->r, 2*PI * (f32)i / (f32)numSides);
				
					glVertex2f(c->o.x, c->o.y);
					glVertex2f(nextVertex.x, nextVertex.y);
					glVertex2f(vertex.x, vertex.y);
					
					vertex = nextVertex;
				}

				glEnd();
				
			} break;

				case Render_RectLine:
			{
				auto c = (render_command_rect_line *)command->data;

				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				glVertex2f(c->min.x, c->min.y); // Line 1
				glVertex2f(c->min.x, c->max.y+1);
				glVertex2f(c->min.x, c->max.y); // Line 2
				glVertex2f(c->max.x, c->max.y); 
				glVertex2f(c->max.x, c->max.y); // Line 3
				glVertex2f(c->max.x, c->min.y);
				glVertex2f(c->max.x, c->min.y); // Line 4
				glVertex2f(c->min.x, c->min.y);
				

				glEnd();

			} break;

				case Render_TriangleLine:
			{
				auto c = (render_command_triangle_line *)command->data;

				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				glVertex2f(c->p[0].x, c->p[0].y); // Line 1
				glVertex2f(c->p[1].x, c->p[1].y);
				glVertex2f(c->p[1].x, c->p[1].y); // Line 2
				glVertex2f(c->p[2].x, c->p[2].y);
				glVertex2f(c->p[2].x, c->p[2].y); // Line 3
				glVertex2f(c->p[0].x, c->p[0].y);

				glEnd();

			} break;

				case Render_CircleLine:
			{
				auto c = (render_command_circle_line *)command->data;

				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINES);

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);

				v2 vertex = c->o + V2(c->r, 0);
				s32 numSides = 32;
				for(s32 i = 1; i <= numSides; i++){
					v2 nextVertex = c->o + V2LengthDir(c->r, 2*PI * (f32)i / (f32)numSides);

					glVertex2f(vertex.x, vertex.y);
					glVertex2f(nextVertex.x, nextVertex.y);

					vertex = nextVertex;
				}

				glEnd();
			} break;

				case Render_Line:
			{
				auto c = (render_command_line *)command->data;

				glDisable(GL_TEXTURE_2D);
				glLineWidth(c->lineSize);
				glBegin(GL_LINES);
				

				glColor4f(c->color.r, c->color.g, c->color.b, c->color.a);
				glVertex2f(c->p0.x, c->p0.y); // Line 1
				glVertex2f(c->p1.x, c->p1.y);

				glEnd();
				glLineWidth(1.f);

			} break;
			
				case Render_Map:
			{
				auto c = (render_command_map *)command->data;

				glDisable(GL_TEXTURE_2D);
				glPointSize(c->pointSize);
				glBegin(GL_POINTS);

				u16 seaLevel = c->seaLevel;
				f32 alpha = c->alpha;
				f32 pointSize = c->pointSize;
				v2 dim = c->max - c->min;
				v2 mapPos = RoundV2(c->min + V2(pointSize/2));
				v2 mapDim = V2(Ceil((c->max.x - c->min.x)/pointSize), Ceil((c->max.y - c->min.y)/pointSize))*pointSize;

				// TODO: Optimize.
				v2 numPoints = mapDim/pointSize;
				v2 pointSizeInWorld = {TILES_X*TILE_SIZE/numPoints.x, TILES_Y*TILE_SIZE/numPoints.y};
				if (c->drawDeepWater){
					for(f32 y = 0; y < numPoints.y; y += 1.f){
						f32 worldY = y*pointSizeInWorld.y;
						for(f32 x = 0; x < numPoints.x; x += 1.f){
							f32 worldX = x*pointSizeInWorld.x;
							tile t = *WorldPosGetTile(V2(worldX, worldY));
							v4 color = TileGetColor(t, seaLevel);

							glColor4f(color.r, color.g, color.b, alpha);

							v2 pos = mapPos + V2(x, y)*pointSize;
							glVertex2f(pos.x, pos.y);
						}
					}
				}else{
					u16 deepSeaLevel = (u16)MaxS32(0, (s32)seaLevel - DEEP_WATER_DEPTH);
					for(f32 y = 0; y < numPoints.y; y += 1.f){
						f32 worldY = y*pointSizeInWorld.y;
						for(f32 x = 0; x < numPoints.x; x += 1.f){
							f32 worldX = x*pointSizeInWorld.x;
							tile *t = WorldPosGetTile(V2(worldX, worldY));
							if (t->h < deepSeaLevel)
								continue;
							v4 color = TileGetColor(*t, seaLevel);

							glColor4f(color.r, color.g, color.b, alpha);

							v2 pos = mapPos + V2(x, y)*pointSize;
							glVertex2f(pos.x, pos.y);
						}
					}
				}

				glEnd();

			} break;

			default:
				Assert(false);
			}

		}

	}
}

void ResetRenderCommands(){
	globalRenderer.commandsData.used = 0;
	for(s32 z = 0; z < NUM_RENDER_DEPTHS; z++){
		globalRenderer.numCommands[z] = 0;
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
	QueryPerformanceFrequency(&globalPerformanceFrequency);

	size_t rendererCommandsSize = 2*1024*1024;
	memset(&globalRenderer, 0, sizeof(globalRenderer));
	globalRenderer.commandsData = MemoryStack(malloc(rendererCommandsSize), rendererCommandsSize);
	
	globalCursor = LoadCursor(0, IDC_ARROW);
	
	size_t gamePermSize = 200*1024*1024; 
	globalGame.permMem = MemoryStack(malloc(gamePermSize), gamePermSize);
	size_t frameMemSize = 100*1024*1024; 
	globalGame.frameMem = MemoryStack(malloc(frameMemSize), frameMemSize);

	// Load exe path
	GetModuleFileNameA(0, globalExePath, ArrayCount(globalExePath));
	{
		s32 len = strlen(globalExePath);
		for(s32 i = len-1; i >= 0; i--){
			if (globalExePath[i] == '\\')
				break;
			globalExePath[i] = '\0';
		}
	}
	//
	// Create window
	//
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc   = WindowProc;
	windowClass.hInstance     = instance;
	windowClass.lpszClassName = "Game Window Class";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&windowClass);

	v2 initialWindowSize = {880.f, 640.f};
	RECT windowFrameSize = {0, 0, (s32)initialWindowSize.x, (s32)initialWindowSize.y};
	AdjustWindowRect(&windowFrameSize, WS_OVERLAPPEDWINDOW|WS_VISIBLE, false); 
	HWND window = CreateWindowEx(0, windowClass.lpszClassName, "Ludum Dare 50 Game",
				                 WS_OVERLAPPEDWINDOW|WS_VISIBLE,
				                 CW_USEDEFAULT, CW_USEDEFAULT,
								 windowFrameSize.right, windowFrameSize.bottom,
								 //(s32)initialWindowSize.x, (s32)initialWindowSize.y,
				                 0, 0, instance, 0);
    if (!window)
        return 0;
    //ShowWindow(window, cmdShow);
	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
    SetCursor(cursor);

	

	//
	// Create console
	//
	if (CREATE_CONSOLE){
		if(AttachConsole((DWORD)-1) == 0){ // wasn't launched from console
			AllocConsole(); // alloc your own instead
		}
	}
	globalStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);


	globalInput.windowDim = GetWindowDimension(window);
	DebugPrintf("mida finestra: %i, %i\n", (s32)globalInput.windowDim.x, (s32)globalInput.windowDim.y);
	DebugPrintf("El cami de l'exe es %s\n", globalExePath);

	HDC dc = GetDC(window);

	//
	// OpenGl
	//
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;
    int pf = ChoosePixelFormat(dc, &pfd);

	if (!pf)
		return 0;
	
	if (!SetPixelFormat(dc, pf, &pfd))
		return 0;

	DescribePixelFormat(dc, pf, sizeof(pfd), &pfd);

	// Device context
	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	s32 timeInFrames = 0;
	
	v2 testPunt = {0, 0};

	LARGE_INTEGER lastFrameTime = GetCurrentTimeCounter();
	globalRunning = true;
	while(globalRunning){

		//
		// Message Loop
		//
		MSG msg = { };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
			switch(msg.message){
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				//
				// Keyboard Input
				//
				b32 wentDown = (msg.message == WM_KEYDOWN);
				auto k = &globalInput.keyboard;
				if (msg.wParam == VK_ESCAPE){
					UpdateButtonState(&k->escape, wentDown);
				}else if (msg.wParam == VK_RETURN){
					UpdateButtonState(&k->enter, wentDown);
				}else if (msg.wParam == VK_SPACE){
					UpdateButtonState(&k->space, wentDown);
				}else if (msg.wParam == VK_SHIFT){
					UpdateButtonState(&k->shift, wentDown);
				}else if (msg.wParam == VK_CONTROL){
					UpdateButtonState(&k->control, wentDown);
				}else if (msg.wParam == VK_BACK){
					UpdateButtonState(&k->backspace, wentDown);
				}else if (msg.wParam == VK_MENU){
					UpdateButtonState(&k->alt, wentDown);
				}else if (msg.wParam == VK_TAB){
					UpdateButtonState(&k->tab, wentDown);
				}else if (msg.wParam == VK_LEFT){
					UpdateButtonState(&k->arrowLeft, wentDown);
				}else if (msg.wParam == VK_RIGHT){
					UpdateButtonState(&k->arrowRight, wentDown);
				}else if (msg.wParam == VK_UP){
					UpdateButtonState(&k->arrowUp, wentDown);
				}else if (msg.wParam == VK_DOWN){
					UpdateButtonState(&k->arrowDown, wentDown);
				}else if (msg.wParam >= 'A' && msg.wParam <= 'Z'){
					UpdateButtonState(&k->letters[msg.wParam - 'A'], wentDown);
				}else if (msg.wParam >= '0' && msg.wParam <= '9'){
					UpdateButtonState(&k->numbers[msg.wParam - '0'], wentDown);
				}

			} break;
			
			case WM_MOUSEWHEEL:
			{
				s32 d = GET_WHEEL_DELTA_WPARAM(msg.wParam) / WHEEL_DELTA;
				globalInput.mouseWheel += d;
			} break;

			
			default:
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			}
		}


		//
		// Mouse Input
		//
		globalInput.windowDim = GetWindowDimension(window);

		POINT mousePoint;
		GetCursorPos(&mousePoint);
		ScreenToClient(window, &mousePoint);
		globalInput.mousePos.x = (f32)(s32)mousePoint.x;
		globalInput.mousePos.y = (f32)(s32)(globalInput.windowDim.y - mousePoint.y);

		b32 hasFocus = (GetFocus() == window);
		if (PointInRect(globalInput.mousePos, V2(0), globalInput.windowDim) && hasFocus){
			UpdateButtonState(&globalInput.mouseButtons[0], GetKeyState(VK_LBUTTON)  & (1 << 15));
			UpdateButtonState(&globalInput.mouseButtons[1], GetKeyState(VK_MBUTTON)  & (1 << 15));
			UpdateButtonState(&globalInput.mouseButtons[2], GetKeyState(VK_RBUTTON)  & (1 << 15));
			UpdateButtonState(&globalInput.mouseButtons[3], GetKeyState(VK_XBUTTON1) & (1 << 15));
			UpdateButtonState(&globalInput.mouseButtons[4], GetKeyState(VK_XBUTTON2) & (1 << 15));
		}

		//
		// Game Code
		//
		if (globalInput.keyboard.arrowLeft.isDown)
			testPunt.x += -1.f;
		if (globalInput.keyboard.arrowRight.isDown)
			testPunt.x += 1.f;
		if (globalInput.keyboard.arrowDown.isDown)
			testPunt.y += -1.f;
		if (globalInput.keyboard.arrowUp.isDown)
			testPunt.y += 1.f;
		

		//v2 mPos = globalInput.mousePos;
		//v2 rectSize = {20.f, 30.f};
		//DrawRect(testPunt - rectSize/2, rectSize, V4_Yellow(), 10);

		//rectSize = V2(15.f,15.f);
		//DrawRectLine(mPos - rectSize/2, rectSize, V4_Blue(), 10);

		//DrawCircleLine(mPos, 50.f, V4_Green(), 20);
		//DrawCircle(mPos + V2(100.f, 0.f), 30.f, V4_Grey(.2f,.4f), 30);

		//DrawTriangle(mPos + V2(-100.f, -50.f),
		//			 mPos + V2(-150.f, 0.f),
		//			 mPos + V2(-80.f, 30.f), V4_Black(.5f), 40);
		//DrawTriangleLine(mPos + V2(-10.f, -50.f),
		//			 mPos + V2(-15.f, 0.f),
		//			 mPos + V2(-80.f, 30.f), V4(.3f, 0.f, 1.f, .9f), 40);

		

		GameMain();

		//
		// Render
		//
		
		RenderCommands(globalInput.windowDim);
		ResetRenderCommands();

		glFlush();
		SwapBuffers(dc);


		//
		// Sleep to render at 60 FPS
		//
		while(true){
			LARGE_INTEGER newFrameTime = GetCurrentTimeCounter();
			f32 timeElapsed = GetSecondsElapsed(lastFrameTime, newFrameTime);
			if (timeElapsed > 1/60.f){
				lastFrameTime = newFrameTime;
				break;
			}
			if (1/60.f - timeElapsed > 0.005f){
				Sleep(1);
			}
		}

		// Reset button input.
		for(s32 i = 0; i < ArrayCount(globalInput.keyboard.asArray); i++){
			globalInput.keyboard.asArray[i].transitionCount = 0;
		}
		for(s32 i = 0; i < ArrayCount(globalInput.mouseButtons); i++){
			globalInput.mouseButtons[i].transitionCount = 0;
		}

		// Reset mousewheel
		globalInput.mouseWheel = 0;

		// Reset Frame Memory:
		globalGame.frameMem.used = 0;

		timeInFrames++;
	}

	FreeConsole();

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(window, dc);
	wglDeleteContext(rc);
	DestroyWindow(window);

    return 0;

}