#include "common.h"
#include "chunk.h"
#include "debug.h"

#ifdef CUSTOM_ALLOC
#include "memory.h"
#endif

int main()
{
#ifdef CUSTOM_ALLOC
    initMemory();
#endif

    Chunk chunk;
    initChunk(&chunk);
    int i;
    for (i = 0; i < 257; i++)
    {
        writeConstant(&chunk, 1.29 * i, i + 1);
    }

    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

#ifdef CUSTOM_ALLOC
    freeMemory();
#endif

    return 0;
}
