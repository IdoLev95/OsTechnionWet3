#include "customAllocator.h"
static Block* allblockList = nullptr;//Always Set To first in list
void insert_all_block_list(Block* block_to_insert)
{
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
Block* Search_in_free_space(size_t size)
{
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
void customFree(void* ptr);
void* customCalloc(size_t nmemb, size_t size);
void* customRealloc(void* ptr, size_t size);
