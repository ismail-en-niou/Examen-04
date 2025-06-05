#include <stdio.h>
#include <stdlib.h>

void print_subset(int *path, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d ", path[i]);
    printf("\n");
}

void    backtrack(int *tab , int size , int index, int *sub , int len_sub , int sum ,int target)
{
    if (target == sum)
    {
        print_subset(sub , len_sub);
        return ;
    }
    if (sum > target || index >= size)
        return ;
    sub[len_sub] = tab[index];
    backtrack(tab , size , index + 1 , sub , len_sub + 1 , sum + tab[index] , target);
    backtrack(tab , size , index + 1, sub , len_sub , sum , target);
}

int main(int ac , char *av[])
{
    if (ac < 3 )
        return 0;
    int target = atoi(av[1]);
    int size = ac - 2;
    int tab[ac - 2];
    int *path = malloc(sizeof(int) * size);
    for (int i = 0 ; i < size ; i++)
        tab[i] = atoi(av[i + 2]);
    backtrack(tab , size ,0 , path , 0 , 0 , target);
    return 0;
}

