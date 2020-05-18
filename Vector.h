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
class Vector
{
    using value_type = T;

    using allocator_type = Allocator;

    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

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

    void grow()
    {
        size_type newSize = std::max(size_type(1), 2 * capacity());
        change_size(newSize);
    }

    void change_size(size_type newSize)
    {
        iterator newRoot = alloc.allocate(newSize);
        iterator newEnd = std::uninitialized_copy(root_, end_, newRoot);
        destroy();

        root_ = newRoot;
        end_ = newEnd;
        limit_ = root_ + newSize;
    }

    void append(const T& value)
    {
        alloc.construct(end_++, value);
    }
    void append(T&& value)
    {
        alloc.construct(end_++, std::move(value));
    }

    void shift(iterator from, size_type units)
    {
        for(iterator it = end_ - 1; it >= from; it--) {
            if (it + units < end_) {
                alloc.destroy(it + units);
            }
            alloc.construct(it + units, *it);
        }

        for (int i = 0; i < units; ++i) {
            if (from + i >= end_) {
                break;
            }
            alloc.destroy(from + i);
        }
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
    size_type max_size() const noexcept { return std::numeric_limits<difference_type>::max(); }
    allocator_type get_allocator() const { return alloc; }

    bool empty() const noexcept { return root_ == end_; }

    void reserve(size_type newSize)
    {
        if (newSize <= capacity())
            return;
        if (newSize > max_size())
            throw std::length_error("Can't reserve size for " + std::to_string(newSize) + " elements");

        change_size(newSize);
    }

    void shrink_to_fit()
    {
        if (size() == capacity())
            return;
        change_size(size());
    }

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

    iterator erase(const_iterator pos)
    {
        size_type index = pos - root_;
        iterator pos_it = root_ + index;

        while(pos_it != (end_ - 1)) {
            alloc.destroy(pos_it);
            alloc.construct(pos_it, *(pos_it+1));
            pos_it++;
        }
        alloc.destroy(--end_);
        return root_ + index;;
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        if(first == last || first < root_ || last > end_) return nullptr;
        size_type indexFrom = first - root_;
        size_type indexTo = last - root_;
        iterator from = root_ + indexFrom;
        iterator to = root_ + indexTo;

        for(iterator it = from; it < to; it++) alloc.destroy(from);

        while(to != end_) {
            *from = *to;
            from++;
            to++;
        }
        end_ -= (indexTo - indexFrom);
    }

    void swap(Vector<T> &other)
    {
        iterator temp;
        temp = other.end_;
        other.end_ = end_;
        end_ = temp;

        temp = other.root_;
        other.root_ = root_;
        root_ = temp;
    }

    iterator insert(const_iterator pos, const T& value)
    {
        size_type index = pos - root_;
        if (end_ == limit_) grow();

        if(size() != 0) shift(root_ + index, 1);

        alloc.construct(root_ + index, value);
        end_++;
        return root_ + index;
    }
    iterator insert( const_iterator pos, T&& value )
    {
        size_type index = pos - root_;
        if (end_ == limit_) grow();

        if(size() != 0) shift(root_ + index, 1);

        alloc.construct(root_ + index, std::move(value));
        end_++;
        return root_ + index;
    }
    iterator insert( const_iterator pos, size_type count, const T& value )
    {
        size_type index = pos - root_;
        if (end_ == limit_ || size() + count > capacity()) {
            size_type newSize = std::max(size()*2, size_type(1));
            while(newSize < size() + count) newSize *= 2;
            change_size(newSize);
        }

        if(size() != 0) shift(root_ + index, count);

        for(int i = 0; i < count; ++i) {
            alloc.construct(root_ + index + i, value);
        }
        end_ += count;
        return root_ + index;
    }
    template< class InputIt, class = typename std::enable_if_t<is_iterator<InputIt>::value>>
    iterator insert( const_iterator pos, InputIt first, InputIt last )
    {
        size_type index = pos - root_;
        size_type count = last - first;
        if (end_ == limit_ || size() + count > capacity()) {
            size_type newSize = std::max(size()*2, size_type(1));
            while(newSize < size() + count) newSize *= 2;
            change_size(newSize);
        }

        if(size() != 0) shift(root_ + index, count);

        for(int i = 0; i < count; ++i) {
            alloc.construct(root_ + index + i, *(first + i));
        }
        end_ += count;
        return root_ + index;
    }
    iterator insert( const_iterator pos, std::initializer_list<T> ilist )
    {
        size_type index = pos - root_;
        size_type count = ilist.size();

        if (end_ == limit_ || size() + count > capacity()) {
            size_type newSize = std::max(size()*2, size_type(1));
            while(newSize < size() + count) newSize *= 2;
            change_size(newSize);
        }

        if(size() != 0) shift(root_ + index, count);

        for(int i = 0; i < count; ++i) {
            alloc.construct(root_ + index + i, *(ilist.begin() + i));
        }
        end_ += count;
        return root_ + index;
    }

    void assign( size_type count, const T& value )
    {
        clear();
        if (count > capacity()) change_size(count);
        for (int i = 0; i < count; ++i) {
            append(value);
        }
    }
    template< class InputIt, class = typename std::enable_if_t<is_iterator<InputIt>::value>>
    void assign( InputIt first, InputIt last )
    {
        size_type count = last - first;
        clear();

        if (count > capacity()) change_size(count);
        for (int i = 0; i < count; ++i) {
            append(*(first + i));
        }
    }
    void assign( std::initializer_list<T> ilist )
    {
        size_type count = ilist.size();
        clear();

        if (count > capacity()) change_size(count);
        for (int i = 0; i < count; ++i) {
            append(*(ilist.begin() + i));
        }
    }

    void resize(size_type count, const value_type& value = value_type())
    {
        if (count == size()) return;
        if (count < size()) {
            while(size() > count) alloc.destroy(--end_);
            return;
        }

        if(count > capacity()) change_size(count);
        std::uninitialized_fill(end_, end_ + (count - size()), value);
        end_ += (count - size());
    }

    void print()
    {
        iterator it = root_;
        std::cout << "[ ";
        while (it != end_) {
            std::cout << " " << *it;
            it++;
            if(it != end_) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // Operators
    T& operator[](size_type i)
    {
        if (i >= size())
            throw std::out_of_range("Vector subscript out of range");
        return root_[i];
    }
    const T& operator[](size_type i) const
    {
        if (i >= size())
            throw std::out_of_range("Vector subscript out of range");
        return root_[i];
    }
    Vector& operator=(const Vector& v)
    {
        if (&v == this)
            return *this;

        destroy();
        create(v.begin(), v.end());
    }

    /* NON-MEMBER FUNCTIONS */
    friend void swap(Vector<T>& lhs, Vector<T>& rhs)
    {
        lhs.swap(rhs);
    }

    friend bool operator== (const Vector<T> &lhs, const Vector<T> &rhs)
    {
        if (lhs.size() != rhs.size()) return false;
        for (int i = 0; i < lhs.size(); ++i) {
            if(lhs[i] != rhs[i]) return false;
        }
        return true;
    }
    friend bool operator!= (const Vector<T> &lhs, const Vector<T> &rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator< (const Vector<T> &lhs, const Vector<T> &rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    friend bool operator<= (const Vector<T> &lhs, const Vector<T> &rhs)
    {
        return lhs < rhs || (!(lhs > rhs) && !(lhs < rhs));
    }
    friend bool operator> (const Vector<T> &lhs, const Vector<T> &rhs)
    {
        return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }
    friend bool operator>= (const Vector<T> &lhs, const Vector<T> &rhs)
    {
        return lhs > rhs || (!(lhs > rhs) && !(lhs < rhs));
    }
};