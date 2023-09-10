#pragma once

#include <memory>
#include <algorithm>
#include <cstring>

template <typename T, typename Allocator = std::allocator<T>>
class MyVector
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;

    using reference = value_type &;
    using const_reference = const value_type &;

    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

    using iterator = pointer;
    using const_iterator = const_pointer;

    // MyVector() = default;
    // ~MyVector() = default;

    iterator begin() const noexcept
    {
        return m_data;
    }

    const_iterator cbegin() const noexcept
    {
        return m_data;
    }

    const_iterator cend() const noexcept
    {
        return &m_data[m_size];
    }

    iterator end() const noexcept
    {
        return &m_data[m_size];
    }

    void push_back(const_reference value)
    {
        if (m_size == m_capacity) {
            m_capacity = m_capacity * 2 + 1;
            auto newData = m_allocator.allocate(m_capacity);
            if (m_size > 0) {
                std::memcpy(newData, m_data, m_size * sizeof(value_type));
            }
            m_allocator.deallocate(m_data, m_size);
            m_data = newData;
            
        }
        m_data[m_size] = value;
        ++m_size;
    }

private:
    allocator_type m_allocator;
    size_type m_size {0};
    size_type m_capacity {0};
    pointer m_data {nullptr};
};
