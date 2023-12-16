#ifndef INC_2074_MEMORY_H
#define INC_2074_MEMORY_H

#include "MemoryRiver.h"
#include <iostream>
#include <set>
#include <vector>

class Memory {

private:
    static constexpr int LENGTH_OF_STRING = 66;
    static constexpr int SIZE_OF_BLOCK = 100;
    int num_of_block;

    struct Pair {
        char index[LENGTH_OF_STRING];
        int value;

        Pair() : index(""), value(0) {}

        Pair(const char index[LENGTH_OF_STRING], const int &value) : value(value) {
            for (int i = 0; i < LENGTH_OF_STRING; i++) {
                this->index[i] = index[i];
            }
        }
    };

    struct BlockNode {

        Pair element;
        int address;
        int size;
        int pre;
        int next;

        BlockNode() : element(), address(-1), size(0), pre(-1), next(-1) {}

        BlockNode(Pair element, int address, int size, int pre, int next) : element(element), address(address), size(size), pre(pre), next(next) {}

        ~BlockNode() = default;

    }; // maintain the block link directly in the file

    BlockNode head, tail;
    int head_pos = sizeof(int), tail_pos = sizeof(int) + sizeof(BlockNode);

    static int cmp_string(const char a[LENGTH_OF_STRING], const char b[LENGTH_OF_STRING]) {
        for (int i = 0; i < LENGTH_OF_STRING; i++) {
            if (a[i] < b[i]) {
                return -1;
            } else if (a[i] > b[i]) {
                return 1;
            }
        }
        return 0;
    }

    static int cmp_pair(const Pair &a, const Pair &b) {
        int flag = cmp_string(a.index, b.index);
        if (flag) {
            return flag;
        }
        if (a.value < b.value) {
            return -1;
        }
        if (a.value > b.value) {
            return 1;
        }
        return 0;
    }

    int FindElement(const Pair &element) {
        BlockNode node;
        int pos = head_pos;
        memory_BlockNode.read(node, pos);
        while (pos != tail_pos) {
//            std::cout << node.size << node.pre << std::endl;
//            if (node.index == "") std::cout << 1 <<  std::endl;
            if (cmp_pair(node.element, element) == 1) {
                break;
            }
            pos = node.next;
            memory_BlockNode.read(node, pos);
        }
        return node.pre;
    }

    int BinarySearchPair(const Pair &element, const Pair *data, const BlockNode &block) {
        int l = 0, r = block.size - 1, mid;
        while (l <= r) {
            mid = l + (r - l) / 2;
            int flag = cmp_pair(data[mid], element);
            if (flag == -1) {
                l = mid + 1;
            } else if (flag == 1) {
                r = mid - 1;
            } else {
                return mid;
            }
        }
        return l;
    }

    MemoryRiver<BlockNode, 1> memory_BlockNode;
    MemoryRiver<Pair[SIZE_OF_BLOCK], 0> memory_element;
    string element_file_name;

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
