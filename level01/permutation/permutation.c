#include <stdio.h>
#include <stdlib.h>

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

void swap(char *a , char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void ft_sort(char *str) {
    int len = ft_strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (str[j] > str[j + 1])
                swap(&str[j], &str[j + 1]);
        }
    }
}

void    ft_permutation(int l , int r , char *str ,char *perm ,char *used)
{
    if (l == r)
    {
        perm[r] = '\0';
        puts(perm);
        return ;
    }
    for (int i = 0 ; i < r ; ++i)
    {
        if(!used[i])
        {
            used[i] = 1;
            perm[l] = str[i];
            ft_permutation(l + 1, r , str  ,perm ,  used);
            used[i] = 0;
        }
    }
}

int main(int ac, char **av) {
    if (ac != 2)
        return 1;

    int len = ft_strlen(av[1]);
    char *str = malloc(len + 1);
    if (!str)
        return 1;
    for (int i = 0; i < len; i++)
        str[i] = av[1][i];
    str[len] = '\0';
    char *used = calloc(len , sizeof(char));
    char *perm = calloc(len + 1 , sizeof(char));
    ft_sort(str);
    ft_permutation(0, len ,str , perm, used);
    free(str);
    return 0;
}
