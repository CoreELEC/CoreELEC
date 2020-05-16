/*
 * Copyright (C) 2014 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "private/bionic_macros.h"

template<typename T>
struct LinkedListEntry {
  LinkedListEntry<T>* next;
  T* element;
};

// ForwardInputIterator
template<typename T>
class LinkedListIterator {
 public:
  LinkedListIterator() : entry_(nullptr) {}
  LinkedListIterator(const LinkedListIterator<T>& that) : entry_(that.entry_) {}
  explicit LinkedListIterator(LinkedListEntry<T>* entry) : entry_(entry) {}

  LinkedListIterator<T>& operator=(const LinkedListIterator<T>& that) {
    entry_ = that.entry_;
    return *this;
  }

  LinkedListIterator<T>& operator++() {
    entry_ = entry_->next;
    return *this;
  }

  T* const operator*() {
    return entry_->element;
  }

  bool operator==(const LinkedListIterator<T>& that) const {
    return entry_ == that.entry_;
  }

  bool operator!=(const LinkedListIterator<T>& that) const {
    return entry_ != that.entry_;
  }

 private:
  LinkedListEntry<T> *entry_;
};

/*
 * Represents linked list of objects of type T
 */
template<typename T, typename Allocator>
class LinkedList {
 public:
  typedef LinkedListIterator<T> iterator;
  typedef T* value_type;

  LinkedList() : head_(nullptr), tail_(nullptr) {}
  ~LinkedList() {
    clear();
  }

  LinkedList(LinkedList&& that) {
    this->head_ = that.head_;
    this->tail_ = that.tail_;
    that.head_ = that.tail_ = nullptr;
  }

  void push_front(T* const element) {
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = head_;
    new_entry->element = element;
    head_ = new_entry;
    if (tail_ == nullptr) {
      tail_ = new_entry;
    }
  }

  void push_back(T* const element) {
    LinkedListEntry<T>* new_entry = Allocator::alloc();
    new_entry->next = nullptr;
    new_entry->element = element;
    if (tail_ == nullptr) {
      tail_ = head_ = new_entry;
    } else {
      tail_->next = new_entry;
      tail_ = new_entry;
    }
  }

  T* pop_front() {
    if (head_ == nullptr) {
      return nullptr;
    }

    LinkedListEntry<T>* entry = head_;
    T* element = entry->element;
    head_ = entry->next;
    Allocator::free(entry);

    if (head_ == nullptr) {
      tail_ = nullptr;
    }

    return element;
  }

  T* front() const {
    if (head_ == nullptr) {
      return nullptr;
    }

    return head_->element;
  }

  void clear() {
    while (head_ != nullptr) {
      LinkedListEntry<T>* p = head_;
      head_ = head_->next;
      Allocator::free(p);
    }

    tail_ = nullptr;
  }

  bool empty() {
    return (head_ == nullptr);
  }

  template<typename F>
  void for_each(F action) const {
    visit([&] (T* si) {
      action(si);
      return true;
    });
  }

  template<typename F>
  bool visit(F action) const {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (!action(e->element)) {
        return false;
      }
    }
    return true;
  }

  template<typename F>
  void remove_if(F predicate) {
    for (LinkedListEntry<T>* e = head_, *p = nullptr; e != nullptr;) {
      if (predicate(e->element)) {
        LinkedListEntry<T>* next = e->next;
        if (p == nullptr) {
          head_ = next;
        } else {
          p->next = next;
        }

        if (tail_ == e) {
          tail_ = p;
        }

        Allocator::free(e);

        e = next;
      } else {
        p = e;
        e = e->next;
      }
    }
  }

  void remove(T* element) {
    remove_if([&](T* e) {
      return e == element;
    });
  }

  template<typename F>
  T* find_if(F predicate) const {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (predicate(e->element)) {
        return e->element;
      }
    }

    return nullptr;
  }

  iterator begin() const {
    return iterator(head_);
  }

  iterator end() const {
    return iterator(nullptr);
  }

  iterator find(T* value) const {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (e->element == value) {
        return iterator(e);
      }
    }

    return end();
  }

  size_t copy_to_array(T* array[], size_t array_length) const {
    size_t sz = 0;
    for (LinkedListEntry<T>* e = head_; sz < array_length && e != nullptr; e = e->next) {
      array[sz++] = e->element;
    }

    return sz;
  }

  bool contains(const T* el) const {
    for (LinkedListEntry<T>* e = head_; e != nullptr; e = e->next) {
      if (e->element == el) {
        return true;
      }
    }
    return false;
  }

  static LinkedList make_list(T* const element) {
    LinkedList<T, Allocator> one_element_list;
    one_element_list.push_back(element);
    return one_element_list;
  }

 private:
  LinkedListEntry<T>* head_;
  LinkedListEntry<T>* tail_;
  DISALLOW_COPY_AND_ASSIGN(LinkedList);
};

#endif // __LINKED_LIST_H
