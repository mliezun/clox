#ifndef clox_memory_h
#define clox_memory_h

#define ALLOCATE(type, count) \                       
    (type *)reallocate(NULL, sizeof(type) * (count))

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity)*2)

#define GROW_ARRAY(previous, type, oldCount, count) \
    (type *)reallocate(previous, sizeof(type) * (count))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, 0)

void *reallocate(void *previous, size_t newSize);

#endif
