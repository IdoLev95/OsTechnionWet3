#include "customAllocator.h"
static Block* allblockList = nullptr;//Always Set To first in list
void insert_all_block_list(Block* block_to_insert)
{
    if (allblockList == nullptr) {
        // If the list is empty, initialize it with the new block
        allblockList = block_to_insert;
    } else if (block_to_insert->loc_on_heap < allblockList->loc_on_heap) {
        // Insert at the beginning if loc_on_heap is smaller than the first block
        block_to_insert->SetNextBlock(allblockList);
        allblockList->SetLastBlock(block_to_insert);
        allblockList = block_to_insert;
    } else {
        // Traverse the list to find the correct position
        Block* current = allblockList;
        while (current->nextBlock != nullptr &&
               current->nextBlock->loc_on_heap < block_to_insert->loc_on_heap) {
            current = current->nextBlock;
        }

        // Insert the block in the correct position
        block_to_insert->SetNextBlock(current->nextBlock);
        if (current->nextBlock != nullptr) {
            current->nextBlock->SetLastBlock(block_to_insert);
        }
        current->SetNextBlock(block_to_insert);
        block_to_insert->SetLastBlock(current);
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
	size = ALIGN_TO_MULT_OF_4(size);
	if(size <= 0)
	{
		throw std::invalid_argument("Size must be greater than zero.");
	}
	Block* find = Search_in_free_space(size +  ALIGN_TO_MULT_OF_4(sizeof(Block))); // meaning we have enough location in our free space in order to define the remain as free block
	if(find == nullptr)
	{
		void* loc_on_Heap = sbrk(size+sizeof(Block));
	    // Check if sbrk failed
	    if (loc_on_Heap == (void*)-1) {
	       perror("sbrk failed");
	    }
		//TODO: need to insert validation on success
		Block* block_to_add = (Block *)loc_on_Heap;
		new (block_to_add) Block(size + sizeof(Block), true,loc_on_Heap + sizeof(Block));
		insert_all_block_list(block_to_add);
		return block_to_add->loc_on_heap;
	}
	else
	{
		if( find->size < size + sizeof(Block))
		{
			throw std::invalid_argument("Size is not big enough");
		}
		size_t  remaining_size = find->size - size ;
	   // Split the block
		Block* new_block = reinterpret_cast<Block*>(
			reinterpret_cast<char*>(find) + sizeof(Block) + size);

		// Update the metadata of the original block
		find->size = size;
		find->is_free = false;

		// Initialize the new block
		new (new_block) Block(remaining_size, true,
			reinterpret_cast<void*>(reinterpret_cast<char*>(new_block) + sizeof(Block)));

		// Insert the new block into the free list
		insert_all_block_list(new_block);

		// Return the allocated memory
		return find->loc_on_heap;
	}
}
void OurFree(Block* block_to_free);

void* customMalloc(size_t size);
void customFree(void* ptr);
void* customCalloc(size_t nmemb, size_t size);
void* customRealloc(void* ptr, size_t size);
