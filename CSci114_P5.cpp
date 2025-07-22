/* Noah Wiley
* CSCI 114
* Programming 5... This Program was written, tested & complied using CLion on Linux (macOS) using C++ 11
*
* To Execute The Program:
* To compile:   g++ -std=c++20 CSci114_P5.cpp -o CSci114_P5.out
* Run  ./CSci114_P5.out
*
* Output:
*512 bytes have been allocated at block 0 for request 1
256 bytes have been allocated at block 0 for request 2
108 bytes have been allocated at block 0 for request 3
360 bytes have been allocated at block 1023 for request 4
512 bytes have returned backed to block 0 for request 1
256 bytes have returned backed to block 0 for request 2
480 bytes have been allocated at block 0 for request 5
108 bytes have returned backed to block 0 for request 3
1020 bytes have been allocated at block 1022 for request 6
360 bytes have returned backed to block 1023 for request 4
480 bytes have returned backed to block 0 for request 5
1020 bytes have returned backed to block 1022 for request 6

*/
#include <iostream>
#include <fstream>

using namespace std;

struct FreeBlock {
    int blockId;
    int size;
    FreeBlock* next;
};

struct Allocation {
    int requestId;
    int blockId;
    int size;
    Allocation* next;
};

void insertFreeBlockSorted(FreeBlock*& head, FreeBlock* newBlock) {
    if (!head || newBlock->size < head->size) {
        newBlock->next = head;
        head = newBlock;
        return;
    }

    FreeBlock* curr = head;
    while (curr->next && curr->next->size < newBlock->size)
        curr = curr->next;
    newBlock->next = curr->next;
    curr->next = newBlock;
}

void removeAllocation(Allocation*& head, int requestId, int& blockId, int& size) {
    Allocation* prev = nullptr;
    Allocation* curr = head;
    while (curr) {
        if (curr->requestId == requestId) {
            blockId = curr->blockId;
            size = curr->size;
            if (prev)
                prev->next = curr->next;
            else
                head = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void insertAllocationSorted(Allocation*& head, Allocation* newAlloc) {
    if (!head || newAlloc->requestId < head->requestId) {
        newAlloc->next = head;
        head = newAlloc;
        return;
    }
    Allocation* curr = head;
    while (curr->next && curr->next->requestId < newAlloc->requestId)
        curr = curr->next;
    newAlloc->next = curr->next;
    curr->next = newAlloc;
}

int main() {
    FreeBlock* freeList = nullptr;
    for (int i = 0; i < 1024; ++i) {
        auto* block = new FreeBlock{i, 1024, nullptr};
        insertFreeBlockSorted(freeList, block);
    }

    Allocation* allocList = nullptr;
    ifstream infile ("requests-1.txt");
    string type;
    int requestId, size;

    while (infile >> type) {
        if (type == "A") {
            infile >> requestId >> size;
            FreeBlock* curr = freeList;
            FreeBlock* prev = nullptr;
            while (curr && curr->size < size) {
                prev = curr;
                curr = curr->next;
            }
            if (!curr) {
                cout << "Request " << requestId << " cannot be found, not enough memory.\n";
                continue;
            }
            int blockId = curr->blockId;
            curr->size -= size;
            cout << size << " bytes have been allocated at block " << blockId << " for request " << requestId << endl;

            auto alloc = new Allocation{requestId, blockId, size, nullptr};
            insertAllocationSorted(allocList, alloc);

            if (curr->size == 0) {
                if (prev)
                    prev->next = curr->next;
                else
                    freeList = curr->next;
                delete curr;
            } else {
                if (prev)
                    prev->next = curr->next;
                else
                    freeList = curr->next;
                curr->next = nullptr;
                insertFreeBlockSorted(freeList, curr);
            }
        }
        else if (type == "R") {
            infile >> requestId;
            int blockId, allocSize;
            removeAllocation(allocList, requestId, blockId, allocSize);
            cout << allocSize << " bytes have returned backed to block " << blockId << " for request " << requestId << endl;
            auto block = new FreeBlock{blockId, allocSize, nullptr};
            insertFreeBlockSorted(freeList, block);
        }
    }

    infile.close();

    ofstream outfile("final_size.txt");
    FreeBlock* curr = freeList;
    while (curr) {
        outfile << curr->size << endl;
        curr = curr->next;
    }
    outfile.close();

    while (freeList) {
        FreeBlock* temp = freeList;
        freeList = freeList->next;
        delete temp;
    }
    while (allocList) {
        Allocation* temp = allocList;
        allocList  = allocList->next;
        delete temp;
    }
    return 0;
}