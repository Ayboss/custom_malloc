#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_BLOCKS 1000

typedef struct Block
{
    void *startAddress;
    int size;
    bool free : true;
} Block;

Block blocks[MAX_BLOCKS];
int block_count = 0;

void *custom_malloc(int size)
{
    for (int i = 0; i < block_count; i++)
    {

        if (blocks[i].free && blocks[i].size >= size)
        {
            blocks[i].free = false;
            return blocks[i].startAddress;
        }
    }

    void *addr = sbrk(size);
    if (addr == (void *)-1)
        return NULL;
    blocks[block_count].startAddress = addr;
    blocks[block_count].size = size;
    blocks[block_count].free = false;
    block_count++;
    return addr;
}

void custom_free(void *addr)
{
    for (int i = 0; i < block_count; i++)
    {

        if (!blocks[i].free && blocks[i].startAddress == addr)
        {
            blocks[i].free = true;
            return;
        }
    }
    return;
}

int main()
{
    int *arr = custom_malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        arr[i] = i;
    }
    for (int i = 0; i < 10; i++)
    {
        printf("arr[%d] = %d \n", i, arr[i]);
    }
    custom_free(arr);
}