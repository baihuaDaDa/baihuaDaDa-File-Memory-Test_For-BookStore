#include "Memory.h"

template<class INDEX, class VALUE>
Memory<INDEX, VALUE>::Memory(const string &index_file_name, const string &value_file_name)
        : memory_index(index_file_name), memory_value(value_file_name) {
    if (!memory_index.file_exist()) {
        memory_index.initialise();
        BlockNode head_node, tail_node;
        head_node.next = tail_pos;
        tail_node.pre = head_pos;
        memory_index.write(head_node);
         memory_index.write(tail_node);
    }
    if (!memory_value.file_exist()) {
        memory_value.initialise();
    } // Check if the file exist. If not, then initialise it.
    memory_index.read(head, head_pos);
    memory_index.read(tail, tail_pos);
    memory_index.get_info(num_of_block, 1);
}

template<class INDEX, class VALUE>
void Memory<INDEX, VALUE>::Insert(const INDEX &index, const VALUE &value) {
    int pos = FindIndex(index);
    if (pos == -1) {
        BlockNode new_block(index, SIZE_OF_BLOCK * (num_of_block++), 1, tail.pre, tail_pos);
        memory_index.write_info(num_of_block, 1);
        BlockNode end;
        memory_index.read(end, tail.pre);
        tail.pre = memory_index.write(new_block);
        end.next = tail.pre;
        memory_index.update(tail, tail_pos);
        memory_index.update(end, new_block.pre);
        memory_value.update(value, new_block.address);
    } else {
        BlockNode now;
        memory_index.read(now, pos);
        while (now.index == index) {
            pos = now.next;
            memory_index.read(now, pos);
        }
        pos = now.pre;
        memory_index.read(now, pos);
        if (now.size + 1 <= SIZE_OF_BLOCK) {
            memory_value.update(value, now.address + now.size * sizeof(VALUE));
            now.size++;
        } else {
            BlockNode next;
            memory_index.read(next, now.next);
            BlockNode new_block(index, SIZE_OF_BLOCK * (num_of_block++), 1, pos, now.next);
            memory_index.write_info(num_of_block, 1);
            now.next = memory_index.write(new_block);
            next.pre = now.next;
            memory_index.update(now, pos);
            memory_index.update(next, new_block.next);
            memory_value.update(value, now.address);
        }
    }
}

template<class INDEX, class VALUE>
void Memory<INDEX, VALUE>::Delete(const INDEX &index, const VALUE &value) {
    int pos = FindIndex(index);
    if (pos == -1) {
        std::cout << "null\n";
        return;
    }
    std::vector<VALUE> data;
    VALUE tmp;
    int size, address;
    BlockNode now;
    memory_index.read(now, pos);
    while (now.index == index) {
        address = now.address;
        for (int i = 0; i < now.size; i++) {
            memory_value.read(tmp, address);
            if (tmp != value) {
                data.push_back(tmp);
            }
            address += sizeof(VALUE);
        }
        if (data.empty()) {
            BlockNode pre, next;
            memory_index.read(pre, now.pre);
            memory_index.read(next, now.next);
            pre.next = now.next;
            next.pre = now.pre;
            num_of_block--;
            memory_index.write_info(num_of_block, 1);
        } else {
            now.size = 0;
            address = now.address;
            for (int i = 0; i < data.size(); i++) {
                memory_value.update(data[i], address);
                address += sizeof(VALUE);
                now.size++;
            }
            data.clear();
        }
        pos = now.next;
        memory_index.read(now, pos);
    }

}

template<class INDEX, class VALUE>
void Memory<INDEX, VALUE>::Find(const INDEX &index) {
    int pos = FindIndex(index);
    if (pos == -1) {
        std::cout << "null\n";
        return;
    }
    std::set<VALUE> data;
    VALUE tmp;
    int size, address;
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