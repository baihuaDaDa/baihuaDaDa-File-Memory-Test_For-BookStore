#include "Memory.h"

int main() {
    Memory<string, int> memory("index.bin", "value.bin");
    int n, value;
    string instruction, index;
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        std::cin >> instruction;
        if (instruction == "insert") {
            std::cin >> index >> value;
            memory.Insert(index, value);
        }
        if (instruction == "delete") {
            std::cin >> index >> value;
            memory.Delete(index, value);
        }
        if (instruction == "find") {
            std::cin >> index;
            memory.Find(index);
        }
    }
    return 0;
}
