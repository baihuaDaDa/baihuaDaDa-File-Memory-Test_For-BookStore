#ifndef INC_2074_MEMORY_H
#define INC_2074_MEMORY_H

#include "MemoryRiver.h"
#include <iostream>
#include <set>
#include <vector>

class Memory {

private:
    static constexpr int LENGTH_OF_STRING = 65;
    const int SIZE_OF_BLOCK = 2;
    int num_of_block;

    struct BlockNode {

        char index[LENGTH_OF_STRING]{};
        int address;
        int size;
        int pre;
        int next;

        BlockNode() : index(""), address(-1), size(0), pre(-1), next(-1) {}

        BlockNode(const char index[LENGTH_OF_STRING], int address, int size, int pre, int next) : address(address), size(size), pre(pre), next(next) {
            for (int i = 0; i < LENGTH_OF_STRING; i++) {
                this->index[i] = index[i];
            }
        }

        ~BlockNode() = default;

    }; // maintain the block link directly in the file

    BlockNode head, tail;
    int head_pos = sizeof(int), tail_pos = sizeof(int) + sizeof(BlockNode);

    static bool cmp_string(const char a[LENGTH_OF_STRING], const char b[LENGTH_OF_STRING]) {
        for (int i = 0; i < LENGTH_OF_STRING; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    int FindIndex(const char index[LENGTH_OF_STRING]) {
        BlockNode p;
        int pos = sizeof(int);
        while (pos != -1) {
            memory_index.read(p, pos);
//            std::cout << p.size << p.pre << std::endl;
//            if (p.index == "") std::cout << 1 <<  std::endl;
            if (cmp_string(p.index, index)) {
                return pos;
            }
            pos = p.next;
        }
        return -1;
    } // find the first address of a specific index

    MemoryRiver<BlockNode, 1> memory_index;
    MemoryRiver<int, 0> memory_value;

public:

    Memory(const string &, const string &); // constructor

    void Insert(const char[LENGTH_OF_STRING], int &);

    void Delete(const char[LENGTH_OF_STRING], const int &);

    void Find(const char[LENGTH_OF_STRING]);

    int get_num_of_block() const {
        return num_of_block;
    }

};

#endif //INC_2074_MEMORY_H
