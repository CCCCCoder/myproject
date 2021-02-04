#ifndef __MY_MEMORY_H__
#define __MY_MEMORY_H__

#include "my_types.h"

void *my_alloc(U64 size);
void my_free(void *pAddr);
#endif
