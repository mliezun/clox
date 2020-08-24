#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    chunk->last_line = 0;
    chunk->lines_count = 0;
    chunk->lines_capacity = 0;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
    if (chunk->capacity == chunk->count)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(chunk->code, uint8_t,
                                 oldCapacity, chunk->capacity);
    }

    if (chunk->last_line != line)
    {
        if (chunk->lines_capacity == chunk->lines_count)
        {
            int oldCapacity = chunk->lines_capacity;
            chunk->lines_capacity = GROW_CAPACITY(oldCapacity);
            chunk->lines = GROW_ARRAY(chunk->lines, int,
                                      oldCapacity, chunk->lines_capacity);
        }
        chunk->last_line = line;
        chunk->lines[chunk->lines_count] = 0;
        chunk->lines[chunk->lines_count + 1] = line;
        chunk->lines_count += 2;
    }

    chunk->lines[chunk->lines_count - 2]++;
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

int addConstant(Chunk *chunk, Value value)
{
    push(value);
    writeValueArray(&chunk->constants, value);
    pop();
    return chunk->constants.count - 1;
}

void writeConstant(Chunk *chunk, Value value, uint8_t byte, int line)
{
    int constant = addConstant(chunk, value);
    if (constant <= UINT8_MAX)
    {
        writeChunk(chunk, byte, line);
        writeChunk(chunk, constant, line);
    }
    else
    {
        if (byte == OP_CONSTANT)
        {
            byte = OP_CONSTANT_LONG;
        }
        else
        {
            byte = OP_CLOSURE_LONG;
        }
        writeChunk(chunk, byte, line);
        writeChunk(chunk, constant >> 16, line);
        writeChunk(chunk, constant >> 8, line);
        writeChunk(chunk, constant, line);
    }
}
