#ifndef RGE_ARENA_IMPL
#define RGE_ARENA_IMPL

#include "arena.hpp"

template <class T>
Arena<T>::Arena(size_t capacity) {
  _capacity = capacity;
  _length = 0;
  _buffer = static_cast<T*>(::operator new(capacity));
}

template <class T>
Arena<T>::Arena(const Arena& old_arena) {
  _capacity = old_arena->capacity;
  _length = 0;
  _buffer = static_cast<T*>(::operator new(_capacity));
  for (size_t index = 0; index < old_arena->_length; index++)
    _buffer[index] = old_arena->_buffer[index];
}

template <class T>
Arena<T>::~Arena() {
  ::operator delete(_buffer);
}

template <class T>
size_t Arena<T>::capacity() const noexcept {
  return _capacity;
}

template <class T>
size_t Arena<T>::length() const noexcept {
  return _length;
}

template <class T>
const void* Arena<T>::cptr() const noexcept {
  return _buffer;
}

template <class T>
void* Arena<T>::ptr() noexcept {
  return _buffer;
}

template <class T>
bool Arena<T>::is_empty() const noexcept {
  return _capacity == 0;
}

template <class T>
bool Arena<T>::reserve(size_t amount) {
  void* new_buffer = new unsigned char[amount];
  _buffer = new_buffer;
  return true;
}

template <class T>
bool Arena<T>::resize(size_t new_capacity) {
  if (new_capacity == _capacity)
    return true;

  // Don't shrink, just return `true` (success)
  if (new_capacity < _capacity)
    return true;

  // Expand/Grow
  T* new_buffer = ::operator new(new_capacity);
  if (new_buffer == nullptr)
    return false;
  for (size_t index = 0; index < _length; index++)
    new_buffer[index] = _buffer[index];

  delete[] _buffer;
  _buffer = new_buffer;
  
  return true;
}

template <class T>
void* Arena<T>::allocate(size_t amount) {
  if (amount + _length > _capacity)
    return nullptr;
  void* ptr = (unsigned char*)_buffer + _length;
  _length += amount;
  return ptr;
}

template <class T>
void* Arena<T>::allocate_aligned(size_t amount, uint8_t alignment) {
  size_t corrected = (_length + 1) & 0xFF'FF'FF'FF'FF'FF'FF'F8;
  return nullptr;
}

template <class T>
bool Arena<T>::clear() {
  _length = 0;
  return true;
}

template <class T>
bool Arena<T>::wipe() {
  if (_buffer == nullptr)
    return false;
  unsigned char* iter = (unsigned char*)_buffer;
  while (_length)
    iter[--_length] = 0;
  return true;
}

template <class T>
bool Arena<T>::destroy() {
  if (_buffer == nullptr)
    return false;
  delete (unsigned char*)_buffer;
  return true;
}

#endif /* RGE_ARENA_IMPL */
