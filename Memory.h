#ifndef INC_2074_MEMORY_H
#define INC_2074_MEMORY_H

#include "MemoryRiver.h"
#include <set>

template<class INDEX, class VALUE>
class Memory {

private:

    static const int SIZE_OF_BLOCK = 10;
    struct BlockNode {

        INDEX index;
        std::set<VALUE> data;
        BlockNode *next;

        BlockNode() : next(nullptr) {}

    };
    BlockNode *head;
    BlockNode *tail;

public:

    Memory() : head(nullptr), tail(nullptr) {}

    void Insert(const INDEX &, const VALUE &);

    void Delete(const INDEX &, const VALUE &);

    void Find(const INDEX &);

};

#endif //INC_2074_MEMORY_H
