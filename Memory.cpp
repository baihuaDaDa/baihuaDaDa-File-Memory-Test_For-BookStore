#include "Memory.h"

Memory::Memory(const string &blockNode_file_name, const string &element_file_name)
        : memory_BlockNode(blockNode_file_name), memory_element(element_file_name), element_file_name(element_file_name) {
    if (!memory_BlockNode.file_exist()) {
        memory_BlockNode.initialise();
        BlockNode head_node, tail_node;
        head_node.next = tail_pos;
        tail_node.pre = head_pos;
        memory_BlockNode.write(head_node);
        memory_BlockNode.write(tail_node);
    }
    if (!memory_element.file_exist()) {
        memory_element.initialise();
    } // Check if the file exist. If not, then initialise it.
    memory_BlockNode.read(head, head_pos);
    memory_BlockNode.read(tail, tail_pos);
    memory_BlockNode.get_info(num_of_block, 1);
}

void Memory::Insert(const char index[LENGTH_OF_STRING], int &value) {
    Pair element(index, value);
    int pos = FindElement(element);
    BlockNode now;
    Pair data[SIZE_OF_BLOCK] = {};
    fstream file;
    if (pos == head_pos) {
        memory_BlockNode.read(head, head_pos);
        pos = head.next;
        if (pos == tail_pos) {
            BlockNode new_block(element, sizeof(Pair[SIZE_OF_BLOCK]) * (num_of_block++), 1, head_pos, tail_pos);
            memory_BlockNode.write_info(num_of_block, 1);
            head.next = memory_BlockNode.write(new_block);
            tail.pre = head.next;
            memory_BlockNode.update(head, head_pos);
            memory_BlockNode.update(tail, tail_pos);
            file.open(element_file_name, std::ios::out | std::ios::in);
            file.seekp(new_block.address);
            file.write(reinterpret_cast<char *>(&element), sizeof(Pair));
            file.close();
            return;
        }
        memory_BlockNode.read(now, pos);
        if (now.size + 1 <= SIZE_OF_BLOCK) {
            file.open(element_file_name, std::ios::in);
            file.seekg(now.address);
            file.read(reinterpret_cast<char *>(data + 1), sizeof(Pair) * now.size);
            file.close();
            data[0] = element;
            now.element = element;
            now.size++;
            memory_BlockNode.update(now, pos);
            file.open(element_file_name, std::ios::out | std::ios::in);
            file.seekp(now.address);
            file.write(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
            file.close();
        } else {
            BlockNode new_block(element, sizeof(Pair[SIZE_OF_BLOCK]) * (num_of_block++), SIZE_OF_BLOCK / 2, now.pre, pos);
            now.size /= 2;
            Pair new_data[SIZE_OF_BLOCK] = {};
            file.open(element_file_name, std::ios::in);
            file.seekg(now.address);
            file.read(reinterpret_cast<char *>(new_data + 1), sizeof(Pair) * new_block.size);
            file.seekg(now.address + new_block.size * sizeof(Pair));
            file.read(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
            file.close();
            new_data[0] = element;
            now.element = data[0];
            new_block.size++;
            BlockNode pre;
            memory_BlockNode.read(pre, now.pre);
            pre.next = memory_BlockNode.write(new_block);
            now.pre = pre.next;
            memory_BlockNode.update(pre, new_block.pre);
            memory_BlockNode.update(now, pos);
            memory_BlockNode.write_info(num_of_block, 1);
            file.open(element_file_name, std::ios::out | std::ios::in);
            file.seekp(now.address);
            file.write(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
            file.seekp(new_block.address);
            file.write(reinterpret_cast<char *>(new_data), sizeof(Pair) * new_block.size);
            file.close();
        } // 裂块
        return;
    } // 首个块的第一个位置插入
    memory_BlockNode.read(now, pos);
    file.open(element_file_name, std::ios::in);
    file.seekg(now.address);
    file.read(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
    file.close();
    int element_pos = BinarySearchPair(element, data, now);
    if (element_pos < now.size) {
        if (cmp_pair(data[element_pos], element) == 0) {
            return;
        }
    }
    if (now.size + 1 <= SIZE_OF_BLOCK) {
        for (int i = now.size - 1; i >= element_pos; i--) {
            data[i + 1] = data[i];
        }
        data[element_pos] = element;
        now.size++;
        memory_BlockNode.update(now, pos);
        file.open(element_file_name, std::ios::out | std::ios::in);
        file.seekp(now.address);
        file.write(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
        file.close();
    } else {
        const int half_size = SIZE_OF_BLOCK / 2;
        BlockNode new_block(now.element, sizeof(Pair[SIZE_OF_BLOCK]) * (num_of_block++), half_size, now.pre, pos);
        now.size = half_size;
        Pair new_data[SIZE_OF_BLOCK] = {};
        if (element_pos < half_size) {
            for (int i = 0; i < element_pos; i++) {
                new_data[i] = data[i];
            }
            for (int i = half_size; i > element_pos; i--) {
                new_data[i] = data[i - 1];
            }
            new_data[element_pos] = element;
            for (int i = 0; i < half_size; i++) {
                data[i] = data[half_size + i];
            }
            new_block.size++;
            now.element = data[0];
        } else {
            for (int i = 0; i < half_size; i++) {
                new_data[i] = data[i];
            }
            for (int i = 0; i < element_pos - half_size; i++) {
                data[i] = data[i + half_size];
            }
            for (int i = half_size; i > element_pos - half_size; i--) {
                data[i] = data[i + half_size - 1];
            }
            data[element_pos - half_size] = element;
            now.element = data[0];
            now.size++;
        };
        BlockNode pre;
        memory_BlockNode.read(pre, now.pre);
        pre.next = memory_BlockNode.write(new_block);
        now.pre = pre.next;
        memory_BlockNode.update(pre, new_block.pre);
        memory_BlockNode.update(now, pos);
        memory_BlockNode.write_info(num_of_block, 1);
        file.open(element_file_name, std::ios::out | std::ios::in);
        file.seekp(now.address);
        file.write(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
        file.seekp(new_block.address);
        file.write(reinterpret_cast<char *>(new_data), sizeof(Pair) * new_block.size);
        file.close();
    } // 裂块
    // 其余位置插入
}

void Memory::Delete(const char index[LENGTH_OF_STRING], const int &value) {
    Pair element(index, value);
    int pos = FindElement(element);
    if (pos == head_pos) {
        return;
    }
    BlockNode now;
    Pair data[SIZE_OF_BLOCK] = {};
    fstream file;
    memory_BlockNode.read(now, pos);
    file.open(element_file_name, std::ios::in);
    file.seekg(now.address);
    file.read(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
    file.close();
    int element_pos = BinarySearchPair(element, data, now);
    if (element_pos < now.size) {
        if (cmp_pair(data[element_pos], element) == 0) {
            if (now.size > 1) {
                for (int i = element_pos; i < now.size - 1; i++) {
                    data[i] = data[i + 1];
                }
                now.size--;
                now.element = data[0];
                memory_BlockNode.update(now, pos);
                file.open(element_file_name, std::ios::out | std::ios::in);
                file.seekp(now.address);
                file.write(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
                file.close();
            } else {
                BlockNode pre, next;
                memory_BlockNode.read(pre, now.pre);
                memory_BlockNode.read(next, now.next);
                pre.next = now.next;
                next.pre = now.pre;
                memory_BlockNode.update(pre, now.pre);
                memory_BlockNode.update(next, now.next);
            } // 删除空块
        }
    }
}

void Memory::Find(const char index[LENGTH_OF_STRING]) {
    Pair element(index, 0);
    int pos = FindElement(element);
    BlockNode now;
    if (pos == head_pos) {
        memory_BlockNode.read(head, head_pos);
        pos = head.next;
        if (pos == tail_pos) {
            std::cout << "null\n";
            return;
        }
    }
    fstream file;
    Pair data[SIZE_OF_BLOCK] = {};
    int i;
    bool flag, total_flag = false;
    while (pos != tail_pos) { // 此处冗余，多搜索了一块，后续可优化
        flag = false;
        memory_BlockNode.read(now, pos);
        file.open(element_file_name, std::ios::in);
        file.seekg(now.address);
        file.read(reinterpret_cast<char *>(data), sizeof(Pair) * now.size);
        file.close();
        for (i = 0; i < now.size; i++) {
            if (cmp_string(data[i].index, index) == 0) {
                flag = true;
                total_flag = true;
                std::cout << data[i].value << ' ';
            } else if (flag) {
                break;
            }
        }
        if (i != now.size) {
            break;
        }
        pos = now.next;
    }
    if (!total_flag) {
        std::cout << "null";
    }
    std::cout << '\n';
}