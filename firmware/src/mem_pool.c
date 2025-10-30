#include "mem_pool.h"
#include <string.h>
#include <stdio.h>

void mem_init(MemPool *mp, uint8_t *buffer, size_t pool_size, size_t block_size) {
    mp->pool = buffer;
    mp->block_size = block_size;
    mp->total_blocks = pool_size / block_size;
    mp->free_blocks = mp->total_blocks;
    mp->alloc_count = 0;
    mp->free_count = 0;
    mp->failed_allocs = 0;
    memset(buffer, 0, pool_size);
}

void *mem_alloc(MemPool *mp) {
    for (size_t i = 0; i < mp->total_blocks; ++i) {
        if (mp->pool[i * mp->block_size] == 0) {
            mp->pool[i * mp->block_size] = 1;
            mp->free_blocks--;
            mp->alloc_count++;
            return &mp->pool[i * mp->block_size];
        }
    }
    mp->failed_allocs++;
    return NULL;
}

void mem_free(MemPool *mp, void *ptr) {
    size_t offset = (uint8_t *)ptr - mp->pool;
    size_t index = offset / mp->block_size;
    if (index < mp->total_blocks && mp->pool[index * mp->block_size] == 1) {
        mp->pool[index * mp->block_size] = 0;
        mp->free_blocks++;
        mp->free_count++;
    }
}

size_t mem_get_free_blocks(const MemPool *mp) {
    return mp->free_blocks;
}

void mem_report(const MemPool *mp) {
    printf("\n[MemPool Diagnostics]\n");
    printf("Total Blocks : %zu\n", mp->total_blocks);
    printf("Free Blocks  : %zu\n", mp->free_blocks);
    printf("Alloc Count  : %zu\n", mp->alloc_count);
    printf("Free Count   : %zu\n", mp->free_count);
    printf("Failed Allocs: %zu\n", mp->failed_allocs);
}

