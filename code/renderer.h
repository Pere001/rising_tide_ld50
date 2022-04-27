

#ifndef RENDERER_H
#define RENDERER_H



enum render_command_type{
	Render_Clear,

	Render_Rect,
	Render_ColoredQuad,
	Render_Triangle,
	Render_Circle,
	Render_TexQuad,
	Render_String,

	Render_RectLine,
	Render_TriangleLine,
	Render_CircleLine,
	Render_Line,

	Render_Map,
};

struct render_command{
	render_command_type type;
	u8 *data; // interpreted differently depending on type.
};

#define MAX_RENDER_COMMANDS_PER_DEPTH 2048
#define NUM_RENDER_DEPTHS 100
struct renderer_state{
	memory_stack commandsData;
	render_command commands[NUM_RENDER_DEPTHS][MAX_RENDER_COMMANDS_PER_DEPTH];
	s32 numCommands[NUM_RENDER_DEPTHS];
};


// Font texture:
// Character size:  20 x 32
// Num characters: 32 to 126 = 95
// Characters per row = 20
// Margin between characters: 2
// Tex dim (without margin): 480 x 128
// Tex dim (just margin): 21*2 x 5*2 = 42 x 10
// Tex dim (with margin): 502 x 138

//  !"#"ADS$#%&'()*+,-./0123456789:;<=>? 
//  @ABCDEFGHIJKLMNOPQRSTUVWWXYZ[\]^_
// `abcdefghijklmnopqrstuvwxyz{|}~
#endif