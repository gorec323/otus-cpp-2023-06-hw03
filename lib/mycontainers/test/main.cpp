#include <gtest/gtest.h>
#include <my_vector.h>
#include <array>

TEST(MyVectorTest, sizeCase)
{
    MyVector<int> v;
    EXPECT_EQ(v.size(), 0);

    constexpr std::array<int, 5> etalonArr {4, 7, 0, 1, 500};
    std::size_t etalonSize {0};
    for (auto &&etalonItem : etalonArr) {
        v.push_back(etalonItem);
        ++etalonSize;
        EXPECT_EQ(v.size(), etalonSize);
    }
}

TEST(MyVectorTest, iterationCase)
{
    MyVector<int> v;

    constexpr std::array<int, 5> etalonArr {4, 7, 0, 1, 500};
    std::size_t etalonSize {0};
    for (auto &&etalonItem : etalonArr) {
        v.push_back(etalonItem);
        ++etalonSize;
        EXPECT_EQ(v.size(), etalonSize);
    }

    for (auto arrIt = etalonArr.cbegin(), vIt = v.cbegin(); etalonArr.cend() != arrIt; ++arrIt, ++vIt) {
        EXPECT_EQ(*arrIt, *vIt);
    }
}
