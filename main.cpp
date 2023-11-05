#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <my_poll_allocator.h>
#include <my_vector.h>
#include "version.h"

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
    static_assert(std::is_integral<T>::value, "T должен быть целым числом");
    if (v == 1)
        return v;

    if (v == 0)
        return 1;

    return v * factorial(v - 1);
}

int main()
{
    using namespace std;

    cout << "My allocators app version: " << PROJECT_VERSION << endl;

    constexpr int ITER_COUNT{10};
    {   // создание экземпляра map<int, int>
        map<int, int> m;

        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
        for (int i = 0; i < ITER_COUNT; ++i)
        {
            m.emplace(i, factorial(i));
        }

        // вывод на экран всех значений map
        printMapContainerItems(m);

        // создание экземпляра map<int, int> с новым аллокатором
        map<int, int, std::less<int>, MyPollAllocator<pair<const int, int>, ITER_COUNT>> mPool;

        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
        for (int i = 0; i < ITER_COUNT; ++i)
        {
            mPool.emplace(i, factorial(i));
        }

        // вывод на экран всех значений map
        printMapContainerItems(mPool);
    }

    { // создание экземпляра своего контейнера для хранения значений типа int
        MyVector<int> v;

        // заполнение 10 элементами от 0 до 9
        for (size_t i = 0; i < ITER_COUNT; ++i)
            v.push_back(i);

        // вывод на экран всех значений, хранящизся в контейнере
        printContainerItems(v);
    }

    {   // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
        MyVector<int, MyPollAllocator<int, 10>> v;

        // заполнение 10 элементами от 0 до 9// заполнение 10 элементами от 0 до 9
        for (size_t i = 0; i < ITER_COUNT; ++i)
            v.push_back(i);

        // вывод на экран всех значений, хранящизся в контейнере
        printContainerItems(v);

        // для себя
        std::cout << std::endl << std::string(50, '*') << std::endl << "for me" << std::endl;

        auto v2 = std::move(v);
        printContainerItems(v2);
        v = v2;
        printContainerItems(v);
    }

    // проверка работы контейнера с новым аллокатором и std::initializer_list и move
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

    return 0;
}
