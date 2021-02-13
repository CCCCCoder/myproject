#ifndef __MY_MEMORY_H__
#define __MY_MEMORY_H__

#include "my_types.h"

U64 *my_alloc(U64 size, const S8 *pDesc);
U64 *my_zalloc(U64 size, const S8 *pDesc);
void my_free(U64 *pAddr);
#endif
