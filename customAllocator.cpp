#include "customAllocator.h"
static set<MemoryBlock> allocatedBlocks;
static set<MemoryBlock> freeBlocks;
bool MemoryBlock::operator<(const MemoryBlock& other) const {
		        return start < other.start;
		    }
void* OurMalloc(size_t size);
void OurFree(MemoryBlock* block_to_free);
