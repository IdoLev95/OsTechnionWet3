#ifndef __CUSTOM_ALLOCATOR__
#define __CUSTOM_ALLOCATOR__

/*=============================================================================
* do no edit lines below!
=============================================================================*/
#include <stddef.h> //for size_t
#include <set>
using namespace std;

void* customMalloc(size_t size);
void customFree(void* ptr);
void* customCalloc(size_t nmemb, size_t size);
void* customRealloc(void* ptr, size_t size);
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

/*=============================================================================
* Block
=============================================================================*/
//suggestion for block usage - feel free to change this
class Block
{
    size_t size;
    unsigned int start;
    bool is_free;
    Block* nextBlock;
    Block* lastBlock;
	public:
		Block(size_t Size,unsigned int Start,bool Is_free):size(ALIGN_TO_MULT_OF_4(Size)),start(Start),is_free(Is_free),nextBlock(NULL),lastBlock(NULL){};
		unsigned int GetEndMemoryBlock();
		void SetNextBlock(Block* NextBlock);
		void SetLastBlock(Block* LastBlock);
		unsigned int GetEnd();

};
extern Block* blockListFree;
extern Block* blockListAlloc;

#endif // CUSTOM_ALLOCATOR
