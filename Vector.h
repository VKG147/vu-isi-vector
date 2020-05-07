#pragma once

#include <iostream>
#include <algorithm>
#include <memory>
#include <cstddef>
#include <stdexcept>
#include <limits>
#include <string>
#include "isIterator.h"

template<class T, class Allocator = std::allocator<T>>
class Vector {
    using value_type = T;

    using allocator_type = Allocator;

    using size_type = std::size_t;

    using reference = T&;
    using const_reference = const T&;

    using iterator = T*;

private:
    iterator root_;
    iterator end_;
    iterator limit_;

    allocator_type alloc;

    void create()
    {
        root_ = end_ = limit_ = alloc.allocate(0);
    }
    void create(size_type n, const T& value)
    {
        root_ = alloc.allocate(n);
        end_ = limit_ = root_ + n;
        std::uninitialized_fill(root_, end_, value);
    }

public:
    /* CONSTRUCTORS */
    Vector() {
        create();
    }
    explicit Vector(const allocator_type &allocator)
    {
        alloc = allocator;
        create();
    }
    explicit Vector(size_type n, const T& val = T(), const allocator_type &allocator = allocator_type())
    {
        alloc = allocator;;
        create(n, val);
    }
    template< class InputIt, class = typename std::enable_if_t<is_iterator<InputIt>::value>>
    Vector(InputIt first, InputIt last, const allocator_type &allocator = allocator_type())
    {
        alloc = allocator;
        create(first, last);
    }
    Vector(const Vector<T>& v, const allocator_type &allocator = allocator_type())
    {
        alloc = allocator;
        create(v.begin(), v.end());
    }
    Vector(Vector<T>&& v, const allocator_type &allocator = allocator_type()) noexcept
    {
        alloc = allocator;
        create();
        v.swap(*this);
    }
    Vector(std::initializer_list<value_type> il, const allocator_type &allocator = allocator_type())
    {
        alloc = allocator;
        create(il.begin(), il.end());
    }

    ~Vector()
    {
        destroy();
    }

    // Element access
    reference front()
    {
        if (root_ == end_)
            throw std::out_of_range("front() called on empty Vector");
        return *root_;
    }
    const_reference front() const
    {
        if (root_ == end_)
            throw std::out_of_range("front() called on empty Vector");
        return *root_;
    }

    reference back()
    {
        if (root_ == end_)
            throw std::out_of_range("back() called on empty Vector");
        return *(end_ - 1);
    }
    const_reference back() const
    {
        if (root_ == end_)
            throw std::out_of_range("back() called on empty Vector");
        return *(end_ - 1);
    }

    reference at(size_type pos) { return root_[pos]; }
    const_reference at(size_type pos) const { return root_[pos]; }

    T* data() { return root_; }
    const T* data() const { return root_; }

    // Modifiers
    void clear() noexcept
    {
        if (root_) {
            iterator it = end_;
            while (it != root_) {
                alloc.destroy(--it);
            }
        }
        end_ = root_;
    }

    void push_back(const T& val)
    {
        if (end_ == limit_) grow();
        append(val);
    }
    void push_back(T&& val)
    {
        if (end_ == limit_) grow();
        append(std::move(val));
    }

    void pop_back()
    {
        if (root_ == end_) return;
        alloc.destroy(--end_);
    }
};