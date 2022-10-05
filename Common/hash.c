/**
 * @file hash.c
 * @author your name (you@domain.com)
 * @brief hash的算法
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include"hash.h"

unsigned HashDjb2(char* key)
{
    unsigned hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
/**
 * @brief 
 * 
 * @param key 
 * @return unsigned 
 */
unsigned HashKey(void* key)
{
    return HashDjb2((char*)key);
}