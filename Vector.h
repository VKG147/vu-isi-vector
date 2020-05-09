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
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
    template< class InputIt, class = typename std::enable_if_t<is_iterator<InputIt>::value>>
    void create(InputIt begin, InputIt end)
    {
        root_ = alloc.allocate(end - begin);
        end_ = limit_ = std::uninitialized_copy(begin, end, root_);
    }
    void create(const Vector<T>&& other)
    {
        root_ = alloc.allocate(other.end() - other.begin());
        end_ = limit_ = std::uninitialized_copy(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()), root_);
    }

    void destroy()
    {
        if (root_) {
            iterator it = end_;
            while (it != root_) {
                alloc.destroy(--it);
            }
            alloc.deallocate(root_, limit_ - root_);
        }
        root_ = end_ = limit_;
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

    /* MEMBER FUNCTIONS */
    // Iterators
    iterator begin() { return root_; }
    const_iterator begin() const { return root_; }
    const_iterator cbegin() const { return root_; }

    iterator end() { return end_; }
    const_iterator end() const { return end_; }
    const_iterator cend() const { return end_; }

    reverse_iterator rbegin() { return end_; }
    const_reverse_iterator rbegin() const { return end_; }
    const_reverse_iterator crbegin() const { return end_; }

    reverse_iterator rend() { return root_; }
    const_reverse_iterator rend() const { return root_; }
    const_reverse_iterator crend() const { return root_; }

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

    //  Getters
    size_type size() const noexcept { return end_ - root_; }
    size_type capacity() const noexcept { return limit_ - root_; }
    size_type max_size() const noexcept { return std::numericlimit_s<difference_type>::max(); }
    allocator_type get_allocator() const { return alloc; }

    bool empty() const noexcept { return root_ == end_; }

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

    template< class... Args >
    iterator emplace(const_iterator pos, Args&&... args)
    {
        T val = T(std::forward<Args>(args)...);
        return insert(pos, val);
    }

    template<class... Args>
    void emplace_back(Args&&... args)
    {
        T val = T(std::forward<Args>(args)...);
        if (end_ == limit_) grow();
        append(val);
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