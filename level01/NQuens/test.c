#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void print_tab(int *tab , int n)
{
    for (int i = 0 ; i< n ; i++)
    {
        fprintf(stdout , "%d",tab[i]);
        if (i != n -1 )
            fprintf(stdout , " ");
    }
    fprintf(stdout, "\n");
}

int ft_abs(int n)
{
    return n < 0 ? -n : n;
}

int is_safe(int *tab , int col , int row )
{
    for ( int i = 0 ; i < row ; i++)
    {
        if ( tab[i] == col || ft_abs(tab[i] - col ) == ft_abs(i - row))
            return 0;
    }
    return 1;
}

void nq(int size , int *tab , int row )
{
    if (row == size )
    {
        print_tab(tab , size);
        return ;
    }
    for (int i = 0 ; i < size ; i++)
    {
        if (is_safe(tab , i , row))
        {
            tab[row] = i ;
            nq(size , tab , row + 1);
        }
    }
}

int main(int ac , char *av[])
{
    if (ac != 2)
        return 1;
    int n = atoi(av[1]);
    int tab[n];
    nq(n , tab , 0);
    return 0;
}