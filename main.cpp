#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <my_poll_allocator.h>
#include <my_vector.h>

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

    if (v == 0)
        return 1;

    return v * factorial(v - 1);
}

int main()
{
    using namespace std;

    constexpr int ITER_COUNT{10};
    { // создание map с 10 элементами с ключом от 0 до 9 и значением в виде факториала ключа
        map<int, int> m;

        for (int i = 0; i < ITER_COUNT; ++i)
        {
            m.emplace(i, factorial(i));
        }

        printMapContainerItems(m);

        map<int, int, std::less<int>, MyPollAllocator<int, ITER_COUNT>> mPool;

        for (int i = 0; i < ITER_COUNT; ++i)
        {
            mPool.emplace(i, factorial(i));
        }

        printMapContainerItems(mPool);
    }

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

    { // самописный контейнер
        MyVector<int> v;
        for (size_t i = 0; i < ITER_COUNT; ++i)
            v.push_back(i);

        printContainerItems(v);
    }

    {   // самописный контейнер с самописным аллокатором
        // 26 - минимальное число с учётом того, что вектор резервируется память порциями (2*size+1)
        MyVector<int, MyPollAllocator<int, 26>> v;

        for (size_t i = 0; i < ITER_COUNT; ++i) {
            std::cout << "1." << i << " MyVector Myalloc" << std::endl;
            v.push_back(i);
        }

        printContainerItems(v);

        auto v2 = std::move(v);
        printContainerItems(v2);
        v = v2;
        printContainerItems(v);
    }

    return 0;
}
