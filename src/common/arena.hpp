#ifndef RGE_ARENA_DEF
#define RGE_ARENA_DEF

#include <cstdint>
typedef uint64_t size_t;

template <class T>
class Arena {
  private:
    typedef T value_type;
    
    size_t _capacity;
    size_t _length;
    T* _buffer;

  public:
    Arena(size_t capacity);
    Arena(const Arena& old_arena);
    ~Arena();

    size_t capacity() const noexcept;
    size_t length() const noexcept;
    const void* cptr() const noexcept;
    void* ptr() noexcept;

    bool is_empty() const noexcept;

    bool reserve(size_t amount);
    bool resize(size_t new_capacity);
    void* allocate(size_t amount);
    void* allocate_aligned(size_t amount, uint8_t alignment);

    bool clear();
    bool wipe();
    bool destroy();
    bool deallocate(size_t size); // Compatibility with other C++ allocators
};

#endif /* RGE_ARENA_DEF */

/*  Since C++ standard doesn't allow separating implementation of
    template classes/functions in a "reusable" way, we do cursed
    STU to fight linker errors
*/
#ifndef RGE_ARENA_IMPL
#include "arena.stu-impl.cpp"
#endif /* RGE_ARENA_NO_IMPL */
