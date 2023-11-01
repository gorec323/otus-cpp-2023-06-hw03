#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <bitset>

template <typename T, std::size_t N = 10>
class MyPollAllocator
{
public:
    using value_type = T;

//    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type; // UB if std::false_type and a1 != a2;

    MyPollAllocator() noexcept = default;
    MyPollAllocator(const MyPollAllocator &) = default;
    MyPollAllocator(MyPollAllocator &&other) noexcept = default;

    template <class U>
    MyPollAllocator(const MyPollAllocator<U, N> &a) noexcept
    {
    }

    MyPollAllocator &operator=(const MyPollAllocator &)
    {
        return *this;
    }

    MyPollAllocator &operator=(MyPollAllocator &&other) noexcept = default;

    ~MyPollAllocator() = default;

    T *allocate(std::size_t size)
    {
        if (size == 0)
            return nullptr;

        std::size_t startPosition;
        if (can_allocate(size, startPosition)) {
            if (!m_pool) {
                m_pool.reset(new T[N]);
            }
            for (size_t j {startPosition}; j < (startPosition + size); ++j)
                m_allocatedFlags.set(j);

            return m_pool.get() + startPosition;
        } else {
//            if (size > N)
//                throw std::bad_alloc();
            return new T[size];
        }
    }

    void deallocate(T *p, std::size_t n) noexcept
    {
        if (pointer_in_buffer(p, n)) {

            std::size_t diffSize = (p - m_pool.get());
            if (diffSize >= N)
                return;

            while (n > 0) {
                m_allocatedFlags[diffSize] = false;
                ++diffSize;
                --n;
            }
        } else {
            delete[] p;
        }
    }

    template <typename U>
    struct rebind
    {
        using other = MyPollAllocator<U, N>;
    };

    template <typename U>
    constexpr bool operator==(const MyPollAllocator<U, N> &other) const noexcept
    {
        return this->m_pool == other.m_pool;
    }

    constexpr bool operator==(const MyPollAllocator &other) const noexcept
    {
        return true;
    }

private:
    using storage_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

    bool pointer_in_buffer(const T *p, std::size_t size) const noexcept
    {
        return ((m_pool.get() <= p) && ((p + size) <= (m_pool.get() + N)));
    }

    bool can_allocate(std::size_t size, std::size_t &startIndex) const noexcept
    {
        for (size_t i = 0; (i + size) <= N; ++i) {
            bool canAllocate {true};
            for (size_t j = size; (canAllocate) && (j > 0); --j) {
                canAllocate &= !m_allocatedFlags[size - j + i];
            }

            if (canAllocate) {
                startIndex = i;
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<T[]> m_pool;
    std::bitset<N> m_allocatedFlags;
};

template <class T, class U>
constexpr bool operator==(const MyPollAllocator<T> &a1, const MyPollAllocator<U> &a2) noexcept
{
    return a1 == a2;
}

template <class T, class U>
constexpr bool operator!=(const MyPollAllocator<T> &a1, const MyPollAllocator<U> &a2) noexcept
{
    return !(a1 == a2);
}
