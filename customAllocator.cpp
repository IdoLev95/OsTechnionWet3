#include "customAllocator.h"
static Block* allblockList = nullptr;//Always Set To first in list
void insert_all_block_list(Block* block_to_insert){
	if(allblockList == nullptr)
	{
		allblockList = block_to_insert;
	}
	else
	{
		block_to_insert->SetNextBlock(allblockList);
		allblockList->SetLastBlock(block_to_insert);
		allblockList = block_to_insert;
	}
}
Block* Search_in_free_space(size_t size){
	Block* Best_Fitted_Block = nullptr;
	Block* curr_block_in_free = allblockList;
	size_t min_size_above_th = 0;
	while(curr_block_in_free != nullptr)
	{
		if(curr_block_in_free->is_free)
		{
			if((size <= curr_block_in_free->size)&&(min_size_above_th == 0 || min_size_above_th > curr_block_in_free->size))
			{
				Best_Fitted_Block = curr_block_in_free;
				min_size_above_th = curr_block_in_free->size;
			}
		}
		curr_block_in_free = curr_block_in_free->nextBlock;
	}
	return Best_Fitted_Block;
}
void* OurMalloc(size_t size)
{
	if(size <= 0)
	{
		throw std::invalid_argument("Size must be greater than zero.");
	}
	Block* find = Search_in_free_space(size+sizeof(Block));
	if(find == nullptr)
	{
		void* loc_on_Heap = sbrk(size+sizeof(Block));
		//TODO: need to insert validation on success
		Block* block_to_add = (Block *)loc_on_Heap;
		new (block_to_add) Block(size, true,loc_on_Heap + sizeof(Block));
		insert_all_block_list(block_to_add);
		return block_to_add->loc_on_heap;
	}
	else
	{
		if(find->size < size + sizeof(Block) + sizeof(Block))
		{
			//There is not enough data in the remains for another block data
		}
		else
		{
			// need to split to 2 blocks.
		}
	}
}
void OurFree(Block* block_to_free);

void* customMalloc(size_t size);

void* customCalloc(size_t nmemb, size_t size);
void* customRealloc(void* ptr, size_t size);

BlockStatus ourFree(Block* block_to_free){
    if(block_to_free->is_free){
        return FREE;
    }
    size_t size = block_to_free->size;
    block_to_free->is_free = true;
    if((block_to_free + size ) == sbrk(0)){
        void* result = sbrk(-size);
        if (result == (void*) -1) {
            // Handle error
            perror("sbrk failed");
            block_to_free->is_free = false;
            return ALLOCATED;
        }
    }
	block* cur_block = allblockList;
    while (cur_block->nextblock != nullptr){
		if(cur_block->is_free){
			if(cur_block + cur_block->size == block_to_free ){//cheak if the previous block is free
				cur_block->nextblock=block_to_free->nextBlock;
				cur_block->size+=block_to_free->size;
				block_to_free->NextBlock->lastblock=cur_block;
				block_to_free = cur_block;

			}
			else if(block_to_free+block_to_free->size == cur_block){//cheak if the next block is free
				block_to_free->size+=cur_block->size;
				block_to_free->nextblock=cur_block->nextblcok;
				cur_block->nextblock->lastblcok=block_to_free;
			}
		}
		cur_block=cur_block->nextblock;
	}
	    
    return FREE;
}


/*if (block_to_free -> lastBlock->is_free){
        block_to_free->lastBlock->size += block_to_free->size;
        block_to_free->lastBlock->nextBlock = block_to_free->nextBlock;
        if(block_to_free->nextBlock != NULL){
            block_to_free->nextBlock->lastBlock = block_to_free->lastBlock;
        }
        block_to_free->lastBlock = NULL;
        block_to_free->nextBlock = NULL;
        block_to_free->size = 0;
        block_to_free->is_free = false;
        return FREE;
    }
    if (block_to_free -> nextBlock->is_free){
        block_to_free->size += block_to_free->nextBlock->size;
        block_to_free->nextBlock = block_to_free->nextBlock->nextBlock;
        if(block_to_free->nextBlock != NULL){
            block_to_free->nextBlock->lastBlock = block_to_free;
        }
        block_to_free->lastBlock = NULL;
        block_to_free->nextBlock = NULL;
        block_to_free->is_free = false;
        return FREE;
    }    
    return FREE;
}

*/