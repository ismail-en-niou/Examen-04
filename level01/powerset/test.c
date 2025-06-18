#include <stdio.h>
#include <stdlib.h>

void print_subset(int *tab , int len)
{
    for (int i = 0 ; i < len ; i++)
    {
        printf("%d",tab[i]);
        if (i != len -1 )
            printf(" ");
    }
    printf("\n");
}

void backtrack(int *tab , int size , int index , int *set ,int set_len ,int target , int sum)
{
    if (target == sum)
    {
        print_subset(set , set_len);
        return;
    }
    if (sum > target || index >= size)
        return ;
    set[set_len] = tab[index];
    backtrack(tab , size , index + 1 ,set, set_len + 1 , target , sum + tab[index]);
    backtrack(tab , size , index + 1 ,set, set_len , target , sum);
}


int main(int ac , char *av[])
{
    if (ac < 3 )
        return 1;
    int target = atoi(av[1]);
    int tab[ac -2];
    int *set = malloc ((ac -2) * sizeof(int));
    for (int i = 0 ; i < ac - 2 ; i++)
        tab[i] = atoi(av[i + 2]);
    backtrack(tab , ac - 2 , 0 , set , 0 , target , 0);
    return 0;
}

