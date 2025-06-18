#include <stdio.h>
#include <stdlib.h>

void    print_table(int *tab , int size)
{
    for (int i = 0 ; i < size ; i++)
    {
        printf("%d",tab[i]);
        if (i != size - 1)
            printf(" ");
    }
    printf("\n");
}

int is_safe(int *tab , int row , int col)
{
    for (int i = 0 ; i < row ; i++)
        if (tab[i] == col || abs(tab[i] - col ) == abs(i - row))
            return 0;
    return 1;
}

void nq(int *tab , int row , int size)
{
    if (row == size)
    {
        print_table(tab , size);
        return;
    }
    for (int col = 0 ; col < size ; col++)
    {
        if (is_safe(tab,row,col))
        {
            tab[row] = col;
            nq(tab , row + 1 , size);
        }
    }
}

int main(int ac , char *av[])
{
    if (ac != 2)
        return 1;
    int size = atoi(av[1]);
    int tab[size];
    nq(tab,0,size);
    return 0;
}