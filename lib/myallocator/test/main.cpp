#include <gtest/gtest.h>
#include <list>
#include <my_poll_allocator.h>
#include "memuse.h"

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

TEST(MyAllocatorTest, Ctor)
{


    using namespace std;

    constexpr int ITER_COUNT{10};
    {   // создание экземпляра map<int, int>


        // создание экземпляра map<int, int> с новым аллокатором
        map<int, int, std::less<int>, MyPollAllocator<pair<const int, int>, ITER_COUNT>> mPool;

        // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
        const auto initial = memuse();

        for (int i = 0; i < ITER_COUNT; ++i) {
            mPool.emplace(i, factorial(i));
        }
        const auto expected = memuse(); // - для создания tupple
        EXPECT_EQ(expected.second - 1, initial.second + 1);

        for (int i = 0; i < ITER_COUNT; ++i) {
            EXPECT_EQ(mPool[i], factorial(i));
        }

    }

    // проверка работы контейнера с новым аллокатором и std::initializer_list и move
    {
        const auto initial = memuse();
        std::vector<int, MyPollAllocator<int>> v1 = {1, 2, 3, 4, 5};
        auto expected = memuse(); // -1 для создания tupple
        EXPECT_EQ(expected.second - 1, initial.second + 1);

        const auto initial2 = memuse();
        std::vector<int, MyPollAllocator<int>> v2 = {4, 3, 1, 2};
        expected = memuse(); // -1 для создания tupple
        EXPECT_EQ(expected.second - 1, initial2.second + 1);

        v2 = std::move(v1);

        EXPECT_EQ(v2.size(), 5);
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(v2[i], static_cast<int>(i + 1));
        }

    }

    {
        std::list<int, MyPollAllocator<int>> l;
        const auto initial = memuse();
        l.push_back(3);
        auto expected = memuse(); // -1 для создания tupple
        EXPECT_EQ(expected.second - 1, initial.second + 1);

        const auto initial2 = memuse();
        std::list<int, MyPollAllocator<int>> l2 = {4, 3, 1, 2};
        expected = memuse(); // -1 для создания tupple
        EXPECT_EQ(expected.second - 1, initial2.second + 1);

        l2 = std::move(l);

//        EXPECT_EQ(l2.size(), 4);
//        for (size_t i = 0; i < 5; ++i) {
//            EXPECT_EQ(l2[i], static_cast<int>(i + 1));
//        }
    }
}

TEST(MyAllocatorTest, CopyCtor)
{
    std::vector<int, MyPollAllocator<int>> v1 = {1, 2, 3, 4, 5};
    std::vector<int, MyPollAllocator<int>> v2 = v1;

    for (auto &item : v2) {
        ++item;
    }

    EXPECT_EQ(v2.size(), 5);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_NE(v2[i], v1[i]);
    }
}

TEST(MyAllocatorTest, CopyAssign)
{
    std::vector<int, MyPollAllocator<int>> v1 = {1, 2, 3, 4, 5};
    std::vector<int, MyPollAllocator<int>> v2;

    v2 = v1;

    for (auto &item : v2) {
        ++item;
    }

    EXPECT_EQ(v2.size(), 5);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_NE(v2[i], v1[i]);
    }
}
