#include "stdio.h"
#include "memory.h"
#include "my_types.h"
#include "stdlib.h"
#include <string.h>

U64 *my_alloc(U64 size, const S8 *pDesc)
{
	MemHeader_t *pHeader  = NULL;
	U64	         allocLen = 0;
	U64          len      = 0;
    void        *pAddr    = NULL;
    
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

	pAddr =  (U64 *)pHeader->body;
end:
    return pAddr;
}

U64 *my_zalloc(U64 size, const S8 *pDesc)
{
	U64 *pMem = NULL;
	
	pMem = my_alloc(size, pDesc);
	if (NULL != pMem) {
		memset(pMem, 0 ,sizeof(*pMem));
	}
	
	return pMem;
}

void my_free(U64 *pAddr)
{
	MemHeader_t *pHeader = NULL;
	
	if (NULL == pAddr) {
		goto end;
	}
	pHeader = (MemHeader_t *)((S8 *)pAddr - sizeof(MemHeader_t));
	
	pHeader->isFree = 1;
	free(pHeader);
end:
	return;
}
