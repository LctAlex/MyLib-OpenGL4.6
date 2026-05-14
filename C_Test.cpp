#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = { 1, 2, 3, 4, 5};
    std::cout << *(vec.data() + 1); //data() == &front()
    return 0;
}