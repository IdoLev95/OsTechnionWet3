#include "customAllocator.h"
static Block* blockListFree;//Always Set To first in list
static Block* blockListAlloc;//Always Set To first in list
void* OurMalloc(size_t size)
{

}
void OurFree(Block* block_to_free);

void* customMalloc(size_t size);
void customFree(void* ptr);
void* customCalloc(size_t nmemb, size_t size);
void* customRealloc(void* ptr, size_t size);
