#include "customAllocator.h"
#include <vector>
#include <chrono>
void stressTest(size_t numAllocations);
void edgeCaseTests();
void alignmentTest();
void alignmentTest();
void customReallocTests();
void customCallocTests();
int main() {
    try {
        // Test1 : Allocate memory using customMalloc and then free it.
		{
			void* ptr1 = customMalloc(32);
			customFree(ptr1);
			if(allblockList != nullptr)
			{
				cout<<"Test 1 Failed" <<endl;
			}
        }
        // Test2 : Allocate 2 memory using customMalloc and then free the first.
		{
			void* ptr1 = customMalloc(32);
			void* ptr2 = customMalloc(40);
			customFree(ptr1);
			char* myptr = reinterpret_cast<char*>(allblockList);
			if(myptr + sizeof(Block) != ptr1 )
			{
				cout<<"Test 2 Failed part 1" <<endl;
			}
			if(allblockList->is_free == false)
			{
				cout<<"Test 2 Failed part 2 " <<endl;
			}
			if(reinterpret_cast<char*>(allblockList->nextBlock) + sizeof(Block) != ptr2 || reinterpret_cast<char*>(allblockList->nextBlock->lastBlock) + sizeof(Block) != ptr1)
			{
				cout<<"Test 2 Failed part 3 " <<endl;
			}
			customFree(ptr2);
		}
		//Test3: Allocate 3 blocks and unite the first 2 after they both free
		{
			void* ptr1 = customMalloc(32);
			void* ptr2 = customMalloc(40);
			void* ptr3 = customMalloc(80);
			customFree(ptr1);
			customFree(ptr2);
			if(allblockList->size != 72 + 2*sizeof(Block))
			{
				cout<<"Test 3 Failed part 1" <<endl;
			}
			if(allblockList->nextBlock->loc_on_heap != ptr3)
			{
				cout<<"Test 3 Failed part 2" <<endl;
			}
			if(allblockList->nextBlock->lastBlock != allblockList)
			{
				cout<<"Test 3 Failed part 3" <<endl;
			}
			if(allblockList->nextBlock->nextBlock != nullptr)
			{
				cout<<"Test 3 Failed part 4" <<endl;
			}
			customFree(ptr3);
		}
		//Test4: Allocate 4 blocks and unite the first 3 the release order is 1 3 2 so after 2 we need  to remain with only 2 blocks
		{
			void* ptr1 = customMalloc(32);
			void* ptr2 = customMalloc(40);
			void* ptr3 = customMalloc(80);
			void* ptr4 = customMalloc(800);
			customFree(ptr1);
			customFree(ptr3);
			customFree(ptr2);
			if(allblockList->size != 152 + 3*sizeof(Block))
			{
				cout<<"Test 4 Failed part 1" <<endl;
			}
			if(allblockList->nextBlock->loc_on_heap != ptr4)
			{
				cout<<"Test 4 Failed part 2" <<endl;
			}
			if(allblockList->nextBlock->lastBlock != allblockList)
			{
				cout<<"Test 4 Failed part 3" <<endl;
			}
			if(allblockList->nextBlock->nextBlock != nullptr)
			{
				cout<<"Test 4 Failed part 4" <<endl;
			}
			customFree(ptr4);
		}

		std::cout << "Starting Tests..." << std::endl;

		std::cout << "Running Stress Test..." << std::endl;
		stressTest(1000);

		std::cout << "Running Edge Case Tests..." << std::endl;
		edgeCaseTests();

		std::cout << "Running Alignment Test..." << std::endl;
		alignmentTest();



		std::cout << "Running customCalloc Tests..." << std::endl;
		customCallocTests();

		std::cout << "Running customRealloc Tests..." << std::endl;
		customReallocTests();

		std::cout << "All Tests Completed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void customCallocTests() {
    try {
        // Test zero-initialization
        void* ptr = customCalloc(10, sizeof(int));
        if (ptr == nullptr) {
            std::cerr << "customCalloc Test Failed: Allocation returned nullptr." << std::endl;
            return;
        }

        int* intArray = reinterpret_cast<int*>(ptr);
        for (size_t i = 0; i < 10; ++i) {
            if (intArray[i] != 0) {
                std::cerr << "customCalloc Test Failed: Memory not zero-initialized at index " << i << std::endl;
                customFree(ptr);
                return;
            }
        }

        std::cout << "customCalloc Test Passed: Memory is zero-initialized." << std::endl;
        customFree(ptr);
    } catch (const std::exception& e) {
        std::cerr << "customCalloc Test Caught Exception: " << e.what() << std::endl;
    }
}

void customReallocTests() {
    try {
        // Allocate initial block
        void* ptr = customMalloc(32);
        if (ptr == nullptr) {
            std::cerr << "customRealloc Test Failed: Initial allocation returned nullptr." << std::endl;
            return;
        }

        // Resize to a larger block
        void* newPtr = customRealloc(ptr, 64);
        if (newPtr == nullptr) {
            std::cerr << "customRealloc Test Failed: Reallocation to larger size returned nullptr." << std::endl;
            customFree(ptr);
            return;
        }

        if (newPtr == ptr) {
            std::cout << "customRealloc Test: Pointer moved to new location during resizing to larger size." << std::endl;
        }

        // Resize to a smaller block
        void* shrunkptr = customRealloc(newPtr, 16);
        if (shrunkptr == nullptr) {
            std::cerr << "customRealloc Test Failed: Reallocation to smaller size returned nullptr." << std::endl;
            return;
        }
        if(shrunkptr != newPtr)
        {
        	cerr << "customRealloc Test: Pointer moved to new location during resizing to smaller size." << std::endl;
        }

        std::cout << "customRealloc Test Passed: Resizing operations successful." << std::endl;
        customFree(shrunkptr);
        if(allblockList != nullptr)
        {
        	cerr << "customRealloc Test failed allblockList need to bu null" << endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "customRealloc Test Caught Exception: " << e.what() << std::endl;
    }
}
void stressTest(size_t numAllocations) {
    std::vector<void*> allocations;

    // Allocate memory
    for (size_t i = 0; i < numAllocations; ++i) {
        void* ptr = customMalloc(32);
        if (ptr == nullptr) {
            std::cerr << "Stress Test Failed: Allocation returned nullptr at iteration " << i << std::endl;
            return;
        }
        allocations.push_back(ptr);
    }

    // Free memory
    for (void* ptr : allocations) {
        customFree(ptr);
    }

    if (allblockList != nullptr) {
        std::cerr << "Stress Test Failed: Memory blocks were not properly cleaned up." << std::endl;
    } else {
        std::cout << "Stress Test Passed." << std::endl;
    }
}
void edgeCaseTests() {
    try {
        // Allocate zero bytes
        void* zeroAlloc = customMalloc(0);
        if (zeroAlloc != nullptr) {
            std::cerr << "Edge Case Test Failed: Zero allocation should return nullptr." << std::endl;
        }

        // Allocate a very large size
        void* largeAlloc = customMalloc(static_cast<size_t>(-1));
        if (largeAlloc != nullptr) {
            std::cerr << "Edge Case Test Failed: Very large allocation should return nullptr or throw." << std::endl;
        }

        // Check block splitting when sufficient space exists
        void* ptr1 = customMalloc(128);
        void* ptr2 = customMalloc(64);
        customFree(ptr1);

        void* ptr3 = customMalloc(64); // Should split the free block
        Block* block3 = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr3) - sizeof(Block));

        if (block3->size != 64 + sizeof(Block)) {
            std::cerr << "Edge Case Test Failed: Block was not properly split." << std::endl;
        }

        if (block3->nextBlock == nullptr || block3->nextBlock->size <= 0) {
            std::cerr << "Edge Case Test Failed: Remaining block was not created correctly after split." << std::endl;
        }

        customFree(ptr2);
        customFree(ptr3);

        // Check when there is not enough space for another block
        void* ptr4 = customMalloc(32);
        void* ptr5 = customMalloc(64);
        void* temp = customMalloc(4);
        customFree(ptr5);

        void* ptr6 = customMalloc(48); // Should use the entire block without splitting
        Block* block6 = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr6) - sizeof(Block));

        if (block6->size != 64 + sizeof(Block)) {
            std::cerr << "Edge Case Test Failed: Block was split incorrectly when space was insufficient for another block." << std::endl;
        }

        customFree(ptr4);
        customFree(ptr6);
        customFree(temp);
        if(allblockList != nullptr)
        	  {
        	cerr << "Edge case test failed: not all memory was free" << endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Edge Case Test Caught Exception: " << e.what() << std::endl;
    }

    std::cout << "Edge Case Tests Completed." << std::endl;
}

void alignmentTest() {
    void* ptr = customMalloc(32);
    if (reinterpret_cast<uintptr_t>(ptr) % 4 != 0) {
        std::cerr << "Alignment Test Failed: Allocated memory is not 4-byte aligned." << std::endl;
    } else {
        std::cout << "Alignment Test Passed." << std::endl;
    }
    customFree(ptr);
}
