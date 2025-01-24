#include "customAllocator.h"
Block* allblockList = nullptr;//Always Set To first in list
Block* ptr_in_heap_to_block(void* ptr_start_heap);
void Remove_Block_From_Lit(Block* block_to_free);
void insert_all_block_list(Block* block_to_insert);
Block* Search_in_free_space(size_t size);


BlockStatus OurFree(Block* block_to_free){
    if(block_to_free->is_free){
        return FREE;
    }
	block_to_free->is_free = true;


	Block* cur_block = allblockList;
	while (cur_block != nullptr && cur_block->nextBlock != nullptr){
		if(cur_block->is_free){
			if(reinterpret_cast<char*>(cur_block) + cur_block->size == reinterpret_cast<char*>(block_to_free) ){//cheak if the previous block is free
				cur_block->nextBlock=block_to_free->nextBlock;
				cur_block->size+=block_to_free->size;
				if(block_to_free->nextBlock!= nullptr)
				{
					block_to_free->nextBlock->lastBlock=cur_block;
				}
				block_to_free = cur_block;

			}
			else if(reinterpret_cast<char*>(block_to_free)+block_to_free->size == reinterpret_cast<char*>(cur_block)){//cheak if the next block is free
				block_to_free->size+=cur_block->size;
				block_to_free->nextBlock=cur_block->nextBlock;
				cur_block->nextBlock->lastBlock=block_to_free;
			}
		}
		cur_block=cur_block->nextBlock;
	}
	if((reinterpret_cast<char*>(block_to_free) +block_to_free->size ) == sbrk(0)){
		Remove_Block_From_Lit(block_to_free);
		void* result = sbrk(-block_to_free->size);
		if (result == (void*) -1) {
			// Handle error
			perror("sbrk failed");
			block_to_free->is_free = false;
			return ALLOCATED;
		}
	}
	return FREE;
}

void* OurMalloc(size_t size,bool is_set_to_zero = false)
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
		new (block_to_add) Block(size + sizeof(Block), false,reinterpret_cast<char*>(loc_on_Heap) + sizeof(Block));
		if(is_set_to_zero)
		{
			memset(block_to_add->loc_on_heap,0,block_to_add->size - sizeof(Block));
		}
		insert_all_block_list(block_to_add);
		return block_to_add->loc_on_heap;
	}
	else
	{
		if( find->size < size + sizeof(Block))
		{
			throw std::invalid_argument("Size is not big enough");
		}
		size_t  remaining_size = find->size - size - sizeof(Block) ;
		if(remaining_size > sizeof(Block))
		{
		   // Split the block
			Block* new_block = reinterpret_cast<Block*>(
				reinterpret_cast<char*>(find) + sizeof(Block) + size);

			// Update the metadata of the original block
			find->size = size + sizeof(Block);
			find->is_free = false;
			if(is_set_to_zero)
			{
				memset(find->loc_on_heap,0,find->size - sizeof(Block));
			}
			// Initialize the new block
			new (new_block) Block(remaining_size, true,
				reinterpret_cast<void*>(reinterpret_cast<char*>(new_block) + sizeof(Block)));

			// Insert the new block into the free list
			insert_all_block_list(new_block);
		}
		else
		{
			find->is_free = false;
			if(is_set_to_zero)
			{
				memset(find->loc_on_heap,0,find->size - sizeof(Block));

			}
		}
		// Return the allocated memory
		return find->loc_on_heap;
	}
}
void* customRealloc(void* ptr, size_t size)
{
	if(ptr == NULL)
	{
		return customMalloc(size);
	}
	Block* curr_block = ptr_in_heap_to_block(ptr);
	size_t old_size = curr_block->size - sizeof(Block);//Only data size
	if(size + sizeof(Block)>= old_size) // There is double sizeof(Block) since if size < old size but there is no enough room to another block so we need to do the same
	{
		void* new_ptr_on_heap = OurMalloc(size);
		//Block* new_block = ptr_in_heap_to_block(new_ptr_on_heap);
		//curr_block->is_free = true;
		memcpy(new_ptr_on_heap,curr_block->loc_on_heap,curr_block->size-sizeof(Block));
		OurFree(curr_block);
		return new_ptr_on_heap;
	}
	else
	{
		size_t remaining_size = curr_block->size - size - sizeof(Block) ;
	   // Split the block
		Block* new_block = reinterpret_cast<Block*>(
			reinterpret_cast<char*>(curr_block) + sizeof(Block) + size);

		// Update the metadata of the original block
		curr_block->size = size + sizeof(Block);
		// Initialize the new block
		new (new_block) Block(remaining_size, false,
			reinterpret_cast<void*>(reinterpret_cast<char*>(new_block) + sizeof(Block)));

		// Insert the new block into the free list
		insert_all_block_list(new_block);
		OurFree(new_block);
		return curr_block->loc_on_heap;
	}
}
void* customCalloc(size_t nmemb, size_t size)
{

	if(size <=0 || nmemb <= 0)
	{
		cerr << "<calloc error>: passed nonpositive size" << endl;
		return NULL;
	}
	return OurMalloc(ALIGN_TO_MULT_OF_4(size)*nmemb,true);
}


void* customMalloc(size_t size)
{
	size = ALIGN_TO_MULT_OF_4(size);
	if(size <=0)
	{
		cerr << "<malloc error>: passed nonpositive size" << endl;
		return NULL;
	}
	return OurMalloc(size,false);
}
void customFree(void* ptr)
{
	Block* block_to_free = ptr_in_heap_to_block(ptr);
	BlockStatus free_status = OurFree(block_to_free);
	if(free_status != FREE)
	{
		throw std::invalid_argument("free status was not free");
	}
}


Block* ptr_in_heap_to_block(void* ptr_start_heap)
{
	if(allblockList == nullptr)
	{
		throw std::invalid_argument("called to free when there is no block");
	}
	Block* curr_block = allblockList;
	 while (curr_block != nullptr){
		 if(curr_block->loc_on_heap == ptr_start_heap)
		 {
			 return curr_block;
		 }
		 curr_block = curr_block->nextBlock;
	 }
	 throw std::invalid_argument("There was not block with that ptr");
}
void Remove_Block_From_Lit(Block* block_to_free)
{

	if(allblockList == nullptr)
	{
		throw std::invalid_argument("called to free when there is no block");
	}
	Block* block_before = block_to_free->lastBlock;
	Block* block_after = block_to_free->nextBlock;
	if(block_after!=nullptr)
	{
		throw std::invalid_argument("Not implemented - only use is block after = null_ptr");
	}
	else if(block_before == nullptr)
	{
		allblockList = nullptr;
	}
	else
	{
		block_before->nextBlock = nullptr;
	}
}
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
