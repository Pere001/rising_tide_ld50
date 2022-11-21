

// Renderer




// Use this as implicit renderer_state:
//static renderer_state globalRenderer = {};

struct render_command_clear{
    v4 color;
};

struct render_command_string{
    v2 p;
    v2 scale;
    v4 color;
    char *str;
    loaded_image *fontTex;
    s32 maxWidth;
    b32 roundPos;
};

struct render_command_rect{
    v2 min, max;
    v4 color;
};
struct render_command_colored_quad{
    v2 p[4]; // Order: CW from corner 0,0
    v4 color[4];
};
struct render_command_triangle{
    v2 p[3];
    v4 color;
};
struct render_command_circle{
    v2 o;
    f32 r;
    v4 color;
};
struct render_command_tex_quad{
    loaded_image *tex;
    v2 p[4];
    v2 texP[4];
    v4 color;
};

struct render_command_line{
    v2 p0;
    v2 p1;
    v4 color;
    f32 lineSize;
};
struct render_command_rect_line{
    v2 min, max;
    v4 color;
};
struct render_command_triangle_line{
    v2 p[3];
    v4 color;
};
struct render_command_circle_line{
    v2 o;
    f32 r;
    v4 color;
};

struct render_command_map{
    v2 min, max;
    f32 alpha;
    f32 pointSize;
    s32 seaLevel;
    b32 drawDeepWater;
};


#define PushRenderCommand(commandType, type, z) ((type *)PushRenderCommand_(commandType, sizeof(type), z))
u8 *PushRenderCommand_(render_command_type commandType, size_t commandSize, s32 z){
    Assert(z >= 0 && z < NUM_RENDER_DEPTHS);
    if (!CanPushSize(&globalRenderer.commandsData, commandSize) ||
        globalRenderer.numCommands[z] >= MAX_RENDER_COMMANDS_PER_DEPTH)
    {
        Assert(false);
        return 0;
    }

    u8 *result = PushSize(&globalRenderer.commandsData, commandSize);
    globalRenderer.commands[z][globalRenderer.numCommands[z]].type = commandType;
    globalRenderer.commands[z][globalRenderer.numCommands[z]].data = result;
    globalRenderer.numCommands[z]++;
    return result;
}

//
// Render Functions
//
void DrawClear(v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_Clear, render_command_clear, z);
    if (!c) return;

    c->color = col;
}


void DrawString(char *str, v2 pos, v2 scale = V2(1.f), s32 z = 50, v4 col = V4_White(),
                s32 maxWidth = 0, b32 roundPos = true, f32 shadow = 0){
    if (shadow){
        DrawString(str, pos + V2(2.f, -2.f), scale, z, V4_Black(shadow), maxWidth, roundPos, false);
    }
    // Push string.
    s32 strLength = strlen(str);
    if (!CanPushSize(&globalRenderer.commandsData, strLength + 1))
        return;
    char *pushedStr = (char *)PushSize(&globalRenderer.commandsData, strLength + 1);
    memcpy(pushedStr, str, strLength + 1);

    auto c = PushRenderCommand(Render_String, render_command_string, z);
    if (!c) return;

    c->fontTex = globalGame.defaultFontTex;
    c->str = pushedStr;
    c->scale = scale;
    c->p = pos;
    c->color = col;
    c->maxWidth = maxWidth;
    c->roundPos = roundPos;

}


void DrawSpriteRect(loaded_image *tex, v2 texPos, v2 dim, v2 pos, v2 scale, s32 z = 50,
                    v4 col = V4_White(), b32 flipX = false){
    auto c = PushRenderCommand(Render_TexQuad, render_command_tex_quad, z);
    if (!c) return;
    
    v2 texMin = V2(texPos.x/tex->dim.x, texPos.y/tex->dim.y);
    v2 texMax = texMin + V2(dim.x/tex->dim.x, dim.y/tex->dim.y);
    if (flipX){
        f32 temp = texMin.x;
        texMin.x = texMax.x;
        texMax.x = temp;
    }
    c->texP[0] = texMin;
    c->texP[1] = V2(texMin.x, texMax.y);
    c->texP[2] = texMax;
    c->texP[3] = V2(texMax.x, texMin.y);
    
    v2 pMax = pos + V2(dim.x*scale.x, dim.y*scale.y);
    c->p[0] = pos;
    c->p[1] = V2(pos.x, pMax.y);
    c->p[2] = pMax;
    c->p[3] = V2(pMax.x, pos.y);


    c->color = col;
    c->tex = tex;
}
void DrawSpriteQuad(loaded_image *tex, v2 p0, v2 p1, v2 p2, v2 p3, v2 dim, s32 z = 50, v4 col = V4_White()){
    auto c = PushRenderCommand(Render_TexQuad, render_command_tex_quad, z);
    if (!c) return;

    c->p[0] = p0;
    c->p[1] = p1;
    c->p[2] = p2;
    c->p[3] = p3;
    c->color = col;
    c->tex = tex;
}

void DrawSpriteRectRotated(loaded_image *tex, v2 texPos, v2 dim, v2 texPivotPos, v2 pivotPos,
                           f32 angle, v2 scale, s32 z = 50, v4 col = V4_White()){
    auto c = PushRenderCommand(Render_TexQuad, render_command_tex_quad, z);
    if (!c) return;

    v2 texP0 = V2(texPos.x/tex->dim.x, texPos.y/tex->dim.y);
    v2 texP1 = texP0 + V2(dim.x/tex->dim.x, dim.y/tex->dim.y);
    c->texP[0] = texP0;
    c->texP[1] = V2(texP0.x, texP1.y);
    c->texP[2] = texP1;
    c->texP[3] = V2(texP1.x, texP0.y);

    v2 c0 = Hadamard(texPos - texPivotPos, scale);
    v2 c1 = V2((texPos.x - texPivotPos.x)*scale.x, (texPos.y + dim.y - texPivotPos.y)*scale.y);
    v2 c2 = Hadamard(texPos + dim - texPivotPos, scale);
    v2 c3 = V2((texPos.x + dim.x - texPivotPos.x)*scale.x, (texPos.y - texPivotPos.y)*scale.y);
    f32 sin = Sin(angle);
    f32 cos = Cos(angle);
    c->p[0] = pivotPos + V2(c0.x*cos -c0.y*sin, c0.y*cos + c0.x*sin);
    c->p[1] = pivotPos + V2(c1.x*cos -c1.y*sin, c1.y*cos + c1.x*sin);
    c->p[2] = pivotPos + V2(c2.x*cos -c2.y*sin, c2.y*cos + c2.x*sin);
    c->p[3] = pivotPos + V2(c3.x*cos -c3.y*sin, c3.y*cos + c3.x*sin);
    c->color = col;
    c->tex = tex;
}

void DrawRect(v2 pos, v2 dim, v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_Rect, render_command_rect, z);
    if (!c) return;

    c->min = pos;
    c->max = pos + dim;
    c->color = col;
}
void DrawRectFrame(v2 pos, v2 dim, v2 outerMargin, v4 col, s32 z = 50){
    // Left
    DrawRect(pos - outerMargin, V2(outerMargin.x, dim.y + outerMargin.y), col, z);
    // Top
    DrawRect(V2(pos.x - outerMargin.x, pos.y + dim.y), V2(dim.x + outerMargin.x, outerMargin.y), col, z);
    // Right
    DrawRect(V2(pos.x + dim.x, pos.y), V2(outerMargin.x, dim.y + outerMargin.y), col, z);
    // Bottom
    DrawRect(V2(pos.x, pos.y - outerMargin.y), V2(dim.x + outerMargin.x, outerMargin.y), col, z);
}
void DrawColoredQuad(v2 p00, v2 p01, v2 p11, v2 p10,
                     v4 col00, v4 col01, v4 col11, v4 col10, s32 z = 50){
    auto c = PushRenderCommand(Render_ColoredQuad, render_command_colored_quad, z);
    if (!c) return;
    
    c->p[0] = p00;
    c->p[1] = p01;
    c->p[2] = p11;
    c->p[3] = p10;
    c->color[0] = col00;
    c->color[1] = col01;
    c->color[2] = col11;
    c->color[3] = col10;
}
void DrawTriangle(v2 p0, v2 p1, v2 p2, v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_Triangle, render_command_triangle, z);
    if (!c) return;

    c->p[0] = p0;
    c->p[1] = p1;
    c->p[2] = p2;
    c->color = col;
}
void DrawCircle(v2 o, f32 r, v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_Circle, render_command_circle, z);
    if (!c) return;

    c->o = o;
    c->r = r;
    c->color = col;
}




void DrawLine(v2 p0, v2 p1, v4 col, s32 z = 50, f32 lineSize = 1.f){
    auto c = PushRenderCommand(Render_Line, render_command_line, z);
    if (!c) return;

    c->p0 = p0;
    c->p1 = p1;
    c->color = col;
    c->lineSize = lineSize;
}
void DrawRectLine(v2 pos, v2 dim, v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_RectLine, render_command_rect_line, z);
    if (!c) return;

    c->min = pos;
    c->max = pos + dim;
    c->color = col;
}
void DrawTriangleLine(v2 p0, v2 p1, v2 p2, v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_TriangleLine, render_command_triangle_line, z);
    if (!c) return;

    c->p[0] = p0;
    c->p[1] = p1;
    c->p[2] = p2;
    c->color = col;
}
void DrawCircleLine(v2 o, f32 r, v4 col, s32 z = 50){
    auto c = PushRenderCommand(Render_CircleLine, render_command_circle_line, z);
    if (!c) return;

    c->o = o;
    c->r = r;
    c->color = col;
}
 
void DrawMap(v2 pos, v2 dim, f32 alpha, f32 pointSize, s32 seaLevel, s32 z = 50, b32 drawDeepWater = true){
    auto c = PushRenderCommand(Render_Map, render_command_map, z);
    if (!c) return;

    c->min = pos;
    c->max = pos + dim;
    c->alpha = alpha;
    c->pointSize = pointSize;
    c->seaLevel = seaLevel;
    c->drawDeepWater = drawDeepWater;
}
