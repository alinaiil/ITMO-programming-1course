#pragma once

#include <vector>
#include <iostream>

template<typename T>
class Allocator {
    static_assert(!std::is_same_v<T, void>, "Type cannot be void");
public:
    typedef T value_type;
private:
    struct Block {
        T *address;
        int nChunks = 1;
        bool isUsed = false;
    };

    size_t totalSize = 1024;
    std::vector<Block> blocks;
    T *begin;
    size_t chunkSize = 4;
    size_t CHUNK = sizeof(T);

    size_t align(size_t toAlign) {
        toAlign = toAlign + chunkSize * CHUNK - (toAlign % (chunkSize * CHUNK));
        return toAlign;
    }

public:
    Allocator(size_t AllocSizeChunks, size_t ChunkSize) {
        totalSize = AllocSizeChunks;
        chunkSize = ChunkSize;
        begin = reinterpret_cast<T *>(malloc(AllocSizeChunks * ChunkSize));
        blocks = std::vector<Block>(AllocSizeChunks);
        blocks[0].address = begin;
        blocks[0].nChunks = AllocSizeChunks;
        blocks[0].isUsed = false;
        for (int i = 1; i < AllocSizeChunks; i++) {
            blocks[i].address = blocks[i - 1].address + chunkSize;
        }
    }

    Allocator() : Allocator(1000, 4) {}

    Allocator(const Allocator &allocator) {
        this->totalSize = allocator.totalSize;
        this->chunkSize = allocator.chunkSize;
        this->blocks = allocator.blocks;
    }

    ~Allocator() {
        delete[] begin;
    }

    T *allocate(size_t toAllocate) {
        toAllocate = align(toAllocate * CHUNK);
        for (int currentInd = 0; currentInd < totalSize; currentInd++) {
            if (!blocks[currentInd].isUsed && toAllocate <= blocks[currentInd].nChunks * chunkSize * CHUNK) {
                blocks[currentInd + (toAllocate / (chunkSize * CHUNK))].nChunks =
                        blocks[currentInd].nChunks - toAllocate / (chunkSize * CHUNK);
                blocks[currentInd + (toAllocate / (chunkSize * CHUNK))].isUsed = false;
                blocks[currentInd].nChunks = toAllocate / (chunkSize * CHUNK);
                blocks[currentInd].isUsed = true;
                return blocks[currentInd].address;
            }
        }
    }

    void deallocate(T *where, size_t toDeallocate) {
        if (!toDeallocate) {
            return;
        }
        toDeallocate = align(toDeallocate * CHUNK);
        int whereInd = (where - begin) / chunkSize;
        size_t counter = 0, counterInd = 0;
        for (int i = whereInd; i < totalSize; i++) {
            counter += blocks[i].nChunks * chunkSize * CHUNK;
            counterInd++;
            if (counter > toDeallocate) {
                break;
            }
        }
        for (int i = 0; i < counterInd; i++) {
            blocks[i].isUsed = false;
        }
        for (int i = 0; i < totalSize; i++) {
            for (int j = i + 1; j < totalSize; j += blocks[j].nChunks * chunkSize * CHUNK) {
                if (blocks[i].isUsed || blocks[j].isUsed) {
                    i = j;
                    break;
                }
                blocks[j].isUsed = true;
                blocks[i].nChunks += blocks[j].nChunks;
            }
        }
    }

    template<typename U, typename... Args>
    void construct(U *ptr, Args &&... args) {
        new(reinterpret_cast<void *>(ptr)) U{std::forward<Args>(args)...};
    }

    template<typename U>
    void destroy(U *ptr) {
        ptr->~U();
    }
};
