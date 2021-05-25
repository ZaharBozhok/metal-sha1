#include <iostream>

const size_t s_wordSize = 5;

void f(char param[][s_wordSize], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << param[i] << '\n';
    }
}

int main()
{
    const size_t rows = 2;
    char arr[rows][s_wordSize] = {0};
    memcpy(arr[0], "abcd", 4);
    memcpy(arr[1], "cho", 4);
    f(reinterpret_cast<char(*)[s_wordSize]>(arr), rows);
    return 0;
}