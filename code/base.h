
// types, common utilities, etc.

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
typedef float    f32;
typedef double   f64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef u64      b64;
typedef u32      b32;
typedef u16      b16;
typedef u8       b8;



#define Assert(x) { if (!(x)) { *(int *)0 = 0; } }
#define AssertRange(x0, x1, x2) Assert(((x0) <= (x1)) && ((x1) <= (x2)))

#define ArrayCount(arr) (sizeof(arr) / sizeof(arr[0]))


// 
// Memory Stack
//
struct memory_stack {
	u8 *base;
	size_t size;
	size_t used;
};

memory_stack MemoryStack(void *base, size_t size){
	memory_stack stack = {(u8 *)base, size, 0};
	return stack;
}

u8 *PushSize(memory_stack *ms, size_t size){
	Assert(ms->used + size <= ms->size);
	u8 *result = ms->base + ms->used;
	ms->used += size;
	return result;
}
#define PushStruct(stack, type) ((type *)PushSize(stack, sizeof(type)))

// WARNING: the returned pointer will become invalidated after you push more stuff!
u8 *PopSize(memory_stack *ms, size_t size){
	Assert(ms->used >= size);
	ms->used -= size;
	return ms->base + ms->used;
}
#define PopStruct(stack, type) ((type *)PopSize(stack, sizeof(type)))


b32 CanPushSize(memory_stack *ms, size_t size){
	if (ms->used + size <= ms->size)
		return true;
	return false;
}
#define CanPushStruct(stack, type) CanPushSize(stack, sizeof(type))



//
// String
//

// Tries to seaparate the string by the spaces...
// dest and src can't be the same.
void ChopStringToFitWidth(char *dest, s32 destBufferSize, char *src, f32 width){
	f32 xAdvance = 20.f - 2.f;
	s32 maxCharsInLine = (s32)(width / xAdvance);
	if (maxCharsInLine < 0)
		maxCharsInLine = 0;

	char *destLimit = dest + destBufferSize;
	char *destIt = dest;
	char *srcIt = src;
	while(destIt < dest + destBufferSize){ // each iteration is a line
		s32 numCharsToCopy = maxCharsInLine;
		b32 foundNewLine = false;
		b32 foundNull = false;
		b32 foundSpace = false;
		for(s32 i = 0; i < maxCharsInLine; i++){
			char c = srcIt[i];
			if (c == '\n'){
				numCharsToCopy = i + 1;
				foundNewLine = true;
				break;
			}else if (c == '\0'){
				foundNull = true;
				numCharsToCopy = i;
				break;
			}else if (c == ' '){
				foundSpace = true;
				numCharsToCopy = i;
			}
		}
		// Copy
		if (destIt + numCharsToCopy >= destLimit){ // Filled all dest buffer
			if (destIt < destLimit - 1){
				memcpy(destIt, srcIt, (s32)(destLimit - destIt) - 1);
			}
			destIt = destLimit - 1;
			break;
		}
		memcpy(destIt, srcIt, numCharsToCopy);
		srcIt += numCharsToCopy;
		destIt += numCharsToCopy;
		if (foundNull)
			break;
		if (foundNewLine){
			continue;
		}
		if (foundSpace){
			srcIt += 1;
		}
		// Add \n
		if (srcIt[0] != '\0' && srcIt[0] != '\n'){
			if (destIt + 1 >= destLimit){
				break;
			}
			destIt[0] = '\n';
			destIt++;
		}
	}
	destIt[0] = '\0';
}





#endif
