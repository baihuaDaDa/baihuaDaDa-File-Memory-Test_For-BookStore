#include "Memory.h"

template<class INDEX, class VALUE>
Memory<INDEX, VALUE>::Memory(const string &index_file_name, const string &value_file_name)
        : memory_index(index_file_name), memory_value(value_file_name) {
    if (!memory_index.file_exist()) {
        memory_index.initialise();
    }
    if (!memory_value.file_exist()) {
        memory_value.initialise();
    } // check if the file exist
}

template<class INDEX, class VALUE>
void Memory<INDEX, VALUE>::Insert(const INDEX &index, const VALUE &value) {
    int pos = FindIndex(index);
    if (pos == -1) {
        BlockNode new_block(index, SIZE_OF_BLOCK * (num_of_block++), 1, -1);
        BlockNode tail;
        pos = memory_index.get_tail() - sizeof(BlockNode);
        memory_index.read(tail, pos);
        tail.next = memory_index.write(new_block);
        memory_index.update(tail, pos);
        memory_value.update(value, new_block.address);
    } else {
        BlockNode now, next;
        memory_index.read(now, pos);
        memory_index.read(next, now.next);
        while (next.index == index) {
            pos = now.next;
            now = next;
            memory_index.read(next, now.next);
        }
        if (now.size + 1 <= SIZE_OF_BLOCK) {
            memory_value.update(value, now.address + now.size * sizeof(VALUE));
            now->size++;
        } else {
            BlockNode new_block(index, SIZE_OF_BLOCK * (num_of_block++), 1, now.next);
            now.next = memory_index.write(new_block);
            memory_index.update(now, pos);
            memory_value.update(value, now.address);
        }
    }
}

template<class INDEX, class VALUE>
void Memory<INDEX, VALUE>::Delete(const INDEX &index, const VALUE &value) {

}

template<class INDEX, class VALUE>
void Memory<INDEX, VALUE>::Find(const INDEX &index) {
    int pos = FindIndex(index);
    std::set<VALUE> data;
    VALUE tmp;
    int size, address;
    if (pos == -1) {
        std::cout << "null\n";
        return;
    }
    BlockNode now;
    memory_index.read(now, pos);
    while (now.index == index) {
        size = now.size;
        address = now.address;
        for (int i = 0; i < size; i++) {
            memory_value.read(tmp, address);
            data.insert(tmp);
            address += sizeof(VALUE);
        }
        pos = now.next;
        memory_index.read(now, pos);
    }
    for (auto iter = data.begin(); iter != data.end(); iter++) {
        std::cout << *iter << (iter != --data.end() ? ' ' : '\n');
    }
}