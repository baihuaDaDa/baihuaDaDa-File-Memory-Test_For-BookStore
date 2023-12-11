#include "Memory.h"

int main() {
    Memory memory("index.bin", "value.bin");
    int n, value;
    string instruction;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        char index[65] = {};
        std::cin >> instruction;
        if (instruction == "insert") {
            std::cin >> index >> value;
            memory.Insert(index, value);
//            std::cout << memory.get_num_of_block() << std::endl;
        }
        if (instruction == "delete") {
            std::cin >> index >> value;
            memory.Delete(index, value);
//            std::cout << memory.get_num_of_block() << std::endl;
        }
        if (instruction == "find") {
            std::cin >> index;
            memory.Find(index);
//            std::cout << memory.get_num_of_block() << std::endl;
        }
    }
    return 0;
}
