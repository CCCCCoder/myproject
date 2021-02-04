#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "my_types.h"
#define MEM_HEADER_MAGIC_HEAD	(0x12345678)
#define MEM_HEADER_MAGIC_TAIL   (0x87654321)

typedef struct MemHeader{
	U32 magic;
	U32 size;
	U32  isFree;
	U8  body[0];
}MemHeader_t;
#endif
