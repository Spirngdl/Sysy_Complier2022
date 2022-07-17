#include "Common/hash_set.h"

int main()
{
    HashSet *set_lhs = HashSetInit();
    HashSetAdd(set_lhs, (void *)(intptr_t)1);
    HashSetAdd(set_lhs, (void *)(intptr_t)2);
    HashSetAdd(set_lhs, (void *)(intptr_t)3);
    HashSetAdd(set_lhs, (void *)(intptr_t)4);
    bool res = HashSetFind(set_lhs, (void *)(intptr_t)1);

    return 0;
}