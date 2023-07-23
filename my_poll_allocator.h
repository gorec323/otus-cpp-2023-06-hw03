#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <stdexcept>

template <typename T, std::size_t N = 10>
class MyPollAllocator
{
public:
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::false_type; // UB if std::false_type and a1 != a2;

    MyPollAllocator() noexcept = default;

    template <class U>
    MyPollAllocator(const MyPollAllocator<U, N> &a) noexcept :
        m_pool {a.m_pool}
    {
        std::copy(std::begin(a.m_allocatedFlags), std::end(a.m_allocatedFlags), std::begin(m_allocatedFlags));
    }

    ~MyPollAllocator() = default;

    T *allocate(std::size_t size)
    {
        if (size == 0)
            return nullptr;

        if (size > POOL_COUNT)
            throw std::bad_alloc();

        if (!m_pool) {
            m_pool.reset(::operator new(POOL_COUNT * sizeof(T)),
                         [](void *p)
                         {
                            ::operator delete(p, POOL_COUNT * sizeof(T));
                         });
            std::fill_n(std::begin(m_allocatedFlags), POOL_COUNT, false);
        }

        for (size_t i = 0; (i + size) <= POOL_COUNT; ++i) {
            bool canAllocate{true};
            for (size_t j = size; (canAllocate) && (j > 0); --j) {
                canAllocate &= !m_allocatedFlags[i];
            }

            if (canAllocate) {
                std::fill_n(std::begin(m_allocatedFlags) + i, size, true);
                return static_cast<T *>(m_pool.get()) + i;
            }
        }

        throw std::bad_alloc();
    }

    void deallocate(T *p, std::size_t n) noexcept
    {
        if (p < m_pool.get())
            return;
        // throw std::out_of_range("p < start pool address");

        std::size_t diffSize = (p - static_cast<T *>(m_pool.get()));
        if (diffSize >= POOL_COUNT)
            return;
        // throw std::out_of_range("p + n >= start pool address + POOL_COUNT");

        while (n > 0) {
            m_allocatedFlags[diffSize] = false;
            ++diffSize;
            --n;
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

    std::shared_ptr<void> m_pool = nullptr;
    bool m_allocatedFlags[N];

private:
    static constexpr std::size_t POOL_COUNT{N};
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
