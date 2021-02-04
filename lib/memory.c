#include "stdio.h"
#include "memory.h"
#include "my_types.h"

void *my_alloc(U64 size)
{
	MemHeader_t *pHeader  = NULL;
	U64	     allocLen = 0;
	U64          len      = 0;

    MY_BUG_ON(size == 0);
    
	allocLen = size + sizeof(MemHeader_t) + sizeof(U32);
	pHeader = malloc(allocLen);
	if (NULL == pHeader) {
		printf("%s failed, (size:%lld)\n",__FUNCTION__, size);
		goto end;
	}
	pHeader->magic  = MEM_HEADER_MAGIC_HEAD;
	pHeader->size   = size;
	pHeader->isFree = 0;
	len = size + sizeof(MemHeader_t);
	*(U32 *)((S8 *)pHeader + len) = MEM_HEADER_MAGIC_TAIL;
end:
	return (void *)pHeader->body;
}

void my_free(void *pAddr)
{
	MemHeader_t *pHeader = NULL;
	
	if (NULL == pAddr) {
		goto end;
	}
	pHeader = (S8 *)pAddr - sizeof(MemHeader_t);
	
	pHeader->isFree = 1;
	free(pHeader);
end:
	return;
}
