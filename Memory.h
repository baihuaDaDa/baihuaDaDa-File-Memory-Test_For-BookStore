#ifndef INC_2074_MEMORY_H
#define INC_2074_MEMORY_H

#include "MemoryRiver.h"
#include <iostream>
#include <set>
#include <vector>

template<class INDEX, class VALUE>
class Memory {

private:
    const int SIZE_OF_BLOCK = 10;
    int num_of_block;

    struct BlockNode {

        INDEX index;
        int address;
        int size;
        int pre;
        int next;

        BlockNode() : address(-1), size(0), pre(-1), next(-1) {}

        BlockNode(INDEX index, int address, int size, int pre, int next) : index(index), address(address), size(size), pre(pre), next(next) {}

    }; // maintain the block link directly in the file

    BlockNode head, tail;
    int head_pos = sizeof(int), tail_pos = sizeof(int) + sizeof(BlockNode);

    int FindIndex(const INDEX &index) {
        BlockNode p;
        int pos = sizeof(int);
        while (pos != -1) {
            memory_index.read(p, pos);
            if (p.index == index) {
                return pos;
            }
            pos = p.next;
        }
        return -1;
    } // find the first address of a specific index

    MemoryRiver<BlockNode, 1> memory_index;
    MemoryRiver<VALUE, 0> memory_value;

public:

    Memory(const string &, const string &); // constructor

    void Insert(const INDEX &, VALUE &);

    void Delete(const INDEX &, const VALUE &);

    void Find(const INDEX &);

};

#endif //INC_2074_MEMORY_H
