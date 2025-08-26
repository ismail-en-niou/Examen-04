#include <stdlib.h>
#include <stdio.h>

void print_table(int *res , int len_res)
{
    for (int i = 0 ; i < len_res ; i++)
    {
        printf("%d",res[i]);
        if (i != len_res - 1)
            printf(" ");
    }
    printf("\n");
}

void backtrack(int index , int size , int target , int *res , int *tab ,int len_res)
{
    if (size == index)
    {
        int sum = 0;
        for (int i = 0 ; i < len_res ; i++)
            sum += res[i];
        if (sum == target)
            print_table(res , len_res);
        return;
    }
    res[len_res] = tab[index];
    backtrack(index + 1, size, target , res , tab , len_res + 1);
    backtrack(index + 1  , size, target , res , tab , len_res);
}

int main (int ac , char *av[])
{
    if ( ac < 3)
        return 1;
    int target = atoi(av[1]);
    int tab[ac - 2];
    for (int i = 0 ; i < ac - 2 ; i++)
        tab[i] = atoi(av[i + 2]);
    int *res = malloc(sizeof(int) * (ac - 2));
    if (!res)
        return 1;
    backtrack(0 , ac - 2 , target , res , tab , 0);
    free(res);
    return 0;
}