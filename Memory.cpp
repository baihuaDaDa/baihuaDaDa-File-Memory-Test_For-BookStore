#include "Memory.h"

Memory::Memory(const string &index_file_name, const string &value_file_name)
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

void Memory::Insert(const char index[LENGTH_OF_STRING], int &value) {
    int pos = FindIndex(index);
    if (pos == -1) {
        BlockNode new_block(index, sizeof(int) * SIZE_OF_BLOCK * (num_of_block++), 1, tail.pre, tail_pos);
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
        while (cmp_string(now.index, index)) {
            pos = now.next;
            memory_index.read(now, pos);
        }
        pos = now.pre;
        memory_index.read(now, pos);
        if (now.size + 1 <= SIZE_OF_BLOCK) {
            memory_value.update(value, now.address + now.size * sizeof(int));
            now.size++;
            memory_index.update(now, pos);
        } else {
            BlockNode next;
            memory_index.read(next, now.next);
            BlockNode new_block(index, sizeof(int) * SIZE_OF_BLOCK * (num_of_block++), 1, pos, now.next);
            memory_index.write_info(num_of_block, 1);
            now.next = memory_index.write(new_block);
            next.pre = now.next;
            memory_index.update(now, pos);
            memory_index.update(next, new_block.next);
            memory_value.update(value, new_block.address);
        }
    }
}

void Memory::Delete(const char index[LENGTH_OF_STRING], const int &value) {
    int pos = FindIndex(index);
    if (pos == -1) {
        return;
    }
    std::vector<int> data;
    int tmp;
    int address;
    BlockNode now;
    memory_index.read(now, pos);
    while (cmp_string(now.index, index)) {
        address = now.address;
        for (int i = 0; i < now.size; i++) {
            memory_value.read(tmp, address);
            if (tmp != value) {
                data.push_back(tmp);
            }
            address += sizeof(int);
        }
        if (data.empty()) {
            BlockNode pre, next;
            memory_index.read(pre, now.pre);
            memory_index.read(next, now.next);
            pre.next = now.next;
            next.pre = now.pre;
            num_of_block--;
            memory_index.write_info(num_of_block, 1);
            memory_index.update(pre, now.pre);
            memory_index.update(next, now.next);
        } else {
            now.size = 0;
            address = now.address;
            for (int i = 0; i < data.size(); i++) {
                memory_value.update(data[i], address);
                address += sizeof(int);
                now.size++;
            }
            data.clear();
            memory_index.update(now, pos);
        }
        pos = now.next;
        memory_index.read(now, pos);
    }

}

void Memory::Find(const char index[LENGTH_OF_STRING]) {
    int pos = FindIndex(index);
    if (pos == -1) {
        std::cout << "null\n";
        return;
    }
    std::set<int> data;
    int tmp;
    int size, address;
    BlockNode now;
    memory_index.read(now, pos);
    while (cmp_string(now.index, index)) {
        size = now.size;
        address = now.address;
        for (int i = 0; i < size; i++) {
            memory_value.read(tmp, address);
            data.insert(tmp);
            address += sizeof(int);
        }
        pos = now.next;
        memory_index.read(now, pos);
    }
    for (auto iter = data.begin(); iter != data.end(); iter++) {
        std::cout << *iter << (iter != --data.end() ? ' ' : '\n');
    }
}