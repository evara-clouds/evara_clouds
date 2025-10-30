/**
 * Example 2: Static Embedded Memory Pool
 * --------------------------------------
 * Demonstrates a fully static, deterministic memory allocator
 * with no calls to malloc() or dynamic allocation.
 *
 * Each block's first byte acts as an allocation flag (0 = free, 1 = used).
 * Predictable, constant memory footprint — ideal for embedded systems.
 */
#include "mem_pool.h"
#include <stdio.h>

#define POOL_SIZE   128
#define BLOCK_SIZE   8

int main(void) {
    uint8_t buffer[POOL_SIZE];
    MemPool mp;
    mem_init(&mp, buffer, POOL_SIZE, BLOCK_SIZE);

    printf("=== Static Memory Pool Test ===\n");
    printf("Initial free blocks: %zu\n", mem_get_free_blocks(&mp));

    void *a = mem_alloc(&mp);
    mem_alloc(&mp);  // second allocation
    printf("Allocated two blocks. Free now: %zu\n", mem_get_free_blocks(&mp));

    mem_free(&mp, a);
    printf("Freed one block. Free now: %zu\n", mem_get_free_blocks(&mp));

    mem_alloc(&mp);
    printf("Reallocated. Free now: %zu\n", mem_get_free_blocks(&mp));

    return 0;
}

