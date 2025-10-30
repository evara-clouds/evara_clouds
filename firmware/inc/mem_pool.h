#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *pool;
    size_t block_size;
    size_t total_blocks;
    size_t free_blocks;

    // Diagnostics
    size_t alloc_count;     // Total successful allocations
    size_t free_count;      // Total frees
    size_t failed_allocs;   // Allocation attempts when pool full
} MemPool;

void mem_init(MemPool *mp, uint8_t *buffer, size_t pool_size, size_t block_size);
void *mem_alloc(MemPool *mp);
void mem_free(MemPool *mp, void *ptr);
size_t mem_get_free_blocks(const MemPool *mp);

#endif

