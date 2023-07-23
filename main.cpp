#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include "my_poll_allocator.h"

using namespace std;

template <typename C>
void printContainerItems(const C &c) noexcept
{
    std::cout << "container:";
    for (auto &&item : c)
    {
        std::cout << " " << item;
    }
    std::cout << std::endl;
}

template <typename C>
void printMapContainerItems(const C &c) noexcept
{
    std::cout << "container:";
    for (auto &&item : c)
    {
        std::cout << " {" << item.first << ", " << item.second << "}";
    }
    std::cout << std::endl;
}

template <typename T>
constexpr T factorial(T v) noexcept
{
    if (v == 1)
        return v;

    return v * factorial(v -1);
}

int main()
{
    {
        std::vector<int, MyPollAllocator<int>> v1 = {1, 2, 3, 4, 5};
        printContainerItems(v1);

        std::vector<int, MyPollAllocator<int>> v2 = {4, 3, 1, 2};
        printContainerItems(v2);
        v2 = std::move(v1);
        printContainerItems(v2);
    }

    {
        std::list<int, MyPollAllocator<int>> l;
        l.push_back(3);
        printContainerItems(l);
        std::list<int, MyPollAllocator<int>> l2 = {4, 3, 1, 2};
        printContainerItems(l2);
        l2 = std::move(l);
        printContainerItems(l2);        
    }

    static constexpr std::size_t POOL_COUNT{11};
    std::map<std::size_t, std::size_t, std::less<std::size_t>, MyPollAllocator<std::pair<const std::size_t, std::size_t>, POOL_COUNT>> m;
    for (size_t i = 0, f = 1; i < POOL_COUNT; ++i)
    {
        f *= (i + 1);
        m.insert(std::make_pair(i, f));
    }

    printMapContainerItems(m);

    // my_vector<int, std::allocator<int>> vec;
    return 0;
}
