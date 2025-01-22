#include "customAllocator.h"

int main() {
    try {
        // Allocate memory using customMalloc
        void* ptr1 = customMalloc(32);
        //std::cout << "Allocated 32 bytes at: " << ptr1 << std::endl;

        //void* ptr2 = customMalloc(64);
        //std::cout << "Allocated 64 bytes at: " << ptr2 << std::endl;

        // Free memory using customFree
        customFree(ptr1);
        //std::cout << "Freed memory at: " << ptr1 << std::endl;

        // Allocate memory again to test reuse
        //void* ptr3 = customMalloc(16);
        //std::cout << "Allocated 16 bytes at: " << ptr3 << std::endl;

        // Test customCalloc
        //void* ptr4 = customCalloc(4, 16);
        //std::cout << "Allocated and zeroed 4 x 16 bytes at: " << ptr4 << std::endl;

        // Test customRealloc
       // void* ptr5 = customRealloc(ptr2, 128);
        //std::cout << "Reallocated memory at: " << ptr2 << " to size 128 bytes at: " << ptr5 << std::endl;

        // Free all remaining allocations
        //customFree(ptr3);
        //customFree(ptr2);
        customFree(ptr1);
        if(allblockList != nullptr)
        {
        	cout<<"Be dammed" <<endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
