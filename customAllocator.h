#ifndef __CUSTOM_ALLOCATOR__
#define __CUSTOM_ALLOCATOR__

/*=============================================================================
* do no edit lines below!
=============================================================================*/
#include <stddef.h> //for size_t
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
using namespace std;

void* customMalloc(size_t size);
void customFree(void* ptr);
//void* customCalloc(size_t nmemb, size_t size);
//void* customRealloc(void* ptr, size_t size);
/*=============================================================================
* do no edit lines above!
=============================================================================*/

/*=============================================================================
* if writing bonus - uncomment lines below
=============================================================================*/
// #ifndef __BONUS__
// #define __BONUS__
// #endif
// void* customMTMalloc(size_t size);
// void customMTFree(void* ptr);
// void* customMTCalloc(size_t nmemb, size_t size);
// void* customMTRealloc(void* ptr, size_t size);

// void heapCreate();
// void heapKill();

/*=============================================================================
* defines
=============================================================================*/
#define SBRK_FAIL (void*)(-1)
#define ALIGN_TO_MULT_OF_4(x) (((((x) - 1) >> 2) << 2) + 4)

enum BlockStatus{
	FREE,
	ALLOCATED
};

/*=============================================================================
* Blocblock_to_freek
=============================================================================*/
//suggestion for block usage - feel free to change this
class Block
{
public:
	size_t size;
    bool is_free;
    Block* nextBlock;
    Block* lastBlock;
    void* loc_on_heap;

		Block(size_t Size,bool Is_free,void* Loc_on_heap):size(ALIGN_TO_MULT_OF_4(Size)),is_free(Is_free),nextBlock(nullptr)
	,lastBlock(nullptr),loc_on_heap(Loc_on_heap){};
		unsigned int GetEndMemoryBlock();
		void SetNextBlock(Block* NextBlock){nextBlock = NextBlock;}
		void SetLastBlock(Block* LastBlock){lastBlock = LastBlock;}
		unsigned int GetEnd();

};
extern Block* allblockList;
#endif // CUSTOM_ALLOCATOR
