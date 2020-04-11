#include <stdlib.h>

#include "common.h"
#include "memory.h"

#ifdef CUSTOM_ALLOC

#include <stdio.h>
#include <errno.h>

#define MINREQ 0x20000

// Big block of memory
void *memory = NULL;
// Position where the last block ends
size_t endpos = 0;

void initMemory()
{
    size_t required = PTRDIFF_MAX;
    while (memory == NULL)
    {
        memory = malloc(required);
        if (required < MINREQ)
        {
            if (memory)
            {
                free(memory);
            }
            printf("Cannot allocate enough memory\n");
            exit(ENOMEM);
        }
        required >>= 1;
    }
}

void freeMemory()
{
    free(memory);
}

struct block_meta
{
    size_t size;
    struct block_meta *next;
    int free;
};

#define META_SIZE sizeof(struct block_meta)

void *first_block = NULL;

struct block_meta *find_free_block(struct block_meta **last, size_t size)
{
    struct block_meta *current = first_block;
    while (current && !(current->free && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
    return current;
}

struct block_meta *request_block(size_t size)
{
    struct block_meta *last = NULL;
    struct block_meta *block = find_free_block(&last, size);
    if (block)
    {
        block->free = 0;
        return block;
    }
    // Append new block to list
    block = memory + endpos;
    endpos += META_SIZE + size;
    if (last)
    {
        last->next = block;
    }
    else
    {
        first_block = block;
    }
    block->free = 0;
    block->next = NULL;
    block->size = size;
    return block;
}

void *my_malloc(size_t size)
{
    struct block_meta *block = request_block(size);
    return block + 1;
}

void my_free(void *ptr)
{
    struct block_meta *block = ptr - META_SIZE;
    block->free = 1;
}

void *my_realloc(void *ptr, size_t size)
{
    if (!ptr)
    {
        return my_malloc(size);
    }
    struct block_meta *block = ptr - META_SIZE;
    if (block->size >= size)
    {
        return block + 1;
    }
    uint8_t *newptr = my_malloc(size);
    size_t i;
    for (i = 0; i < (block->size < size ? block->size : size); i++)
    {
        newptr[i] = ((uint8_t *)ptr)[i];
    }
    block->free = 1;
    return newptr;
}
#endif

void *reallocate(void *previous, size_t newSize)
{
#ifndef CUSTOM_ALLOC
    if (newSize == 0)
    {
        free(previous);
        return NULL;
    }

    return realloc(previous, newSize);
#else
    if (newSize == 0)
    {
        my_free(previous);
        return NULL;
    }

    return my_realloc(previous, newSize);
#endif
}
