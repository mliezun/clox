#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
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

    return 0;
}
