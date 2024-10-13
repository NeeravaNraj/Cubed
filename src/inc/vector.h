#ifndef __VECTOR_H
#define __VECTOR_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VEC_FREE
#define VEC_FREE free
#endif
#ifndef VEC_MALLOC
#define VEC_MALLOC malloc
#endif // !VEC_MALLOC
#ifndef VEC_REALLOC
#define VEC_REALLOC realloc
#endif // !VEC_REALLOC
#ifndef VEC_CALLOC
#define VEC_CALLOC calloc
#endif // !VEC_CALLOC

typedef void (*Vec_elem_destructor)(void *elem);

typedef struct Vec_t {
  size_t capacity;
  size_t length;
  Vec_elem_destructor destructor;
} Vec_t;

#define Vec(type) type*

#define __vec_to_base(vec) (&((Vec_t *)(vec))[-1])

#define __base_to_vec(ptr) ((void *)&((Vec_t *)(ptr))[1])

#define Vec_capacity(vec) ((vec) ? __vec_to_base(vec)->capacity : (size_t)0)

#define Vec_length(vec) ((vec) ? __vec_to_base(vec)->length : (size_t)0)

#define __Vec_get_destructor(vec)                                              \
  ((vec) ? __vec_to_base(vec)->destructor : NULL)

#define Vec_isempty(vec) (Vec_length(vec) == 0)

#define Vec_begin(vec) (vec)

#define Vec_end(vec) ((vec) ? &((vec)[Vec_length(vec) - 1]) : NULL)

#define __Vec_grow_cap(size) ((size) ? ((size) << 1) : 1)

#define __Vec_reserve(vec, n)                                                  \
  do {                                                                         \
    size_t curr_cap = Vec_capacity(vec);                                       \
    if (curr_cap < (n)) {                                                      \
      __Vec_grow((vec), (n));                                                  \
    }                                                                          \
  } while (0)

#define __Vec_set_length(vec, len)                                             \
  do {                                                                         \
    if (vec) {                                                                 \
      __vec_to_base((vec))->length = (len);                                    \
    }                                                                          \
  } while (0)

#define __Vec_set_capacity(vec, cap)                                           \
  do {                                                                         \
    if (vec) {                                                                 \
      __vec_to_base((vec))->capacity = (cap);                                  \
    }                                                                          \
  } while (0)

#define __Vec_set_elem_destructor(vec, elem_destructor_fn)                     \
  do {                                                                         \
    if (vec) {                                                                 \
      __vec_to_base(vec)->destructor = (elem_destructor_fn);                   \
    }                                                                          \
  } while (0)

#define Vec_init(vec, capacity, elem_destructor_fn)                            \
  do {                                                                         \
    if (!vec) {                                                                \
      __Vec_reserve(vec, capacity);                                            \
      __Vec_set_elem_destructor(vec, elem_destructor_fn);                      \
    }                                                                          \
  } while (0)

#define __Vec_grow(vec, size)                                                  \
  do {                                                                         \
    const size_t new_size = (size) * sizeof(*(vec)) + sizeof(Vec_t);           \
    if (vec) {                                                                 \
      void *curr_p1_ = __vec_to_base(vec);                                     \
      void *curr_p2_ = VEC_REALLOC(curr_p1_, new_size);                        \
      assert(curr_p2_);                                                        \
      (vec) = __base_to_vec(curr_p2_);                                         \
      __Vec_set_capacity(vec, size);                                           \
    } else {                                                                   \
      void *curr_p_ = VEC_MALLOC(new_size);                                    \
      assert(curr_p_);                                                         \
      (vec) = __base_to_vec(curr_p_);                                          \
      __Vec_set_length(vec, 0);                                                \
      __Vec_set_capacity(vec, size);                                           \
    }                                                                          \
  } while (0)

#define Vec_delete(vec, i)                                                     \
  do {                                                                         \
    if (vec) {                                                                 \
      if (Vec_length(vec) < Vec_capacity(vec) / 2) {                           \
        __Vec_grow(vec, Vec_capacity(vec) / 2);                                \
      }                                                                        \
      const size_t curr_len_ = Vec_length(vec);                                \
      if ((i) < curr_len_) {                                                   \
        Vec_elem_destructor elem_destructor__ = __Vec_get_destructor(vec);     \
        if (elem_destructor__) {                                               \
          elem_destructor__(&(vec)[i]);                                        \
        }                                                                      \
        __Vec_set_length((vec), curr_len_ - 1);                                \
        memmove((vec) + (i), vec + (i) + 1,                                    \
                sizeof(*(vec)) * (curr_len_ - 1 - (i)));                       \
      }                                                                        \
    }                                                                          \
  } while (0)

#define Vec_clear(vec)                                                         \
  do {                                                                         \
    if (vec) {                                                                 \
      Vec_elem_destructor elem_destructor__ = __Vec_get_destructor(vec);       \
      if (elem_destructor__) {                                                 \
        for (size_t i_ = 0; i_ < Vec_length((vec)); ++i_) {                    \
          elem_destructor__(&(vec)[i_]);                                       \
        }                                                                      \
      }                                                                        \
      __Vec_set_length(vec, 0);                                                \
    }                                                                          \
  } while (0)

#define Vec_free(vec)                                                          \
  do {                                                                         \
    if (vec) {                                                                 \
      void *p1_ = __vec_to_base(vec);                                          \
      Vec_elem_destructor elem_destructor__ = __Vec_get_destructor(vec);       \
      if (elem_destructor__) {                                                 \
        for (size_t i_ = 0; i_ < Vec_length(vec); ++i_) {                      \
          elem_destructor__(&(vec)[i_]);                                       \
        }                                                                      \
      }                                                                        \
      VEC_FREE(p1_);                                                           \
    }                                                                          \
  } while (0)

#define Vec_push(vec, value)                                                   \
  do {                                                                         \
    size_t curr_cap_ = Vec_capacity(vec);                                      \
    if (curr_cap_ <= Vec_length(vec)) {                                        \
      __Vec_grow((vec), __Vec_grow_cap(curr_cap_));                            \
    }                                                                          \
    (vec)[Vec_length(vec)] = (value);                                          \
    __Vec_set_length(vec, Vec_length(vec) + 1);                                \
  } while (0)

#define Vec_insert_at(vec, value, index)                                       \
  do {                                                                         \
    if (vec) {                                                                 \
      assert(index >= 0);                                                      \
      assert(value);                                                           \
      size_t cap__ = Vec_capacity(vec);                                        \
      if (cap__ <= Vec_length(vec)) {                                          \
        __Vec_grow(vec, __Vec_grow_cap(cap__));                                \
      }                                                                        \
      memmove((vec) + (index) + 1, (vec) + (index),                            \
              sizeof(*(vec)) * Vec_length(vec) - (index)-1);                   \
      (vec)[index] = (value);                                                  \
      __Vec_set_length((vec), Vec_length(vec) + 1);                            \
    }                                                                          \
  } while (0)

#endif // __VECTOR_H
