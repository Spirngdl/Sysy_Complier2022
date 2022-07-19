#include "Common/hash_set.h"
#include <stdio.h>
int main()
{
    HashSet *set_lhs = HashSetInit();
    // for (int i = 0; i < 1; i++)
    // {
    //     int *index = (int *)malloc(sizeof(int));
    //     *index = i;
    //     HashSetAdd(set_lhs, (void *)(intptr_t)(*index));
    // }
    for (int i = 0; i < 10; i++)
    {
        HashSetAdd(set_lhs, (void *)(intptr_t)i);
    }
    // HashSetAdd(set_lhs, (void *)(intptr_t)1);
    // HashSetAdd(set_lhs, (void *)(intptr_t)2);
    // HashSetAdd(set_lhs, (void *)(intptr_t)3);
    // HashSetAdd(set_lhs, (void *)(intptr_t)4);
    void *element;
    // HashSetFirst(set_lhs);
    set_lhs->first(set_lhs);
    while ((element = set_lhs->next(set_lhs)) != NULL)
    {
        int i = (int)(long long)element;
        printf("%d ", i);
        // int *i = (int *)element;
        // printf("%d ", (*i));
    }

    bool res = HashSetFind(set_lhs, (void *)(intptr_t)1);
    for (int i = 0; i < 30; i++)
    {
        if (HashSetFind(set_lhs, (void *)(intptr_t)i))
        {
            printf("%d ", i);
        }
    }
    res = HashSetFind(set_lhs, (void *)(intptr_t)1);
    return 0;
}