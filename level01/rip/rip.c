#include <stdio.h>
#include <unistd.h>

int _strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

int is_valid(char *str)
{
    int bal = 0;
    int i = 0 ;
    while (str[i])
    {
        if (str[i] == '(')
            bal++;
        else if (str[i] == ')')
            bal--;
        if (bal < 0)
            return 0;
        i++;
    }
    return (bal == 0);
}

void    backtrack(char *tab , int index , int left , int right)
{
    if (left == 0 && right == 0)
    {
        if (is_valid(tab))
            puts(tab);
        return;
    }
    for (int i = index ; tab[i] ; i++)
    {

        if (tab[i] == '(' && left > 0)
        {
            tab[i] = ' ';
            backtrack(tab , index + 1 , left - 1 , right);
            tab[i] = '()';
        }
        if (tab[i] == ')' && right > 0)
        {
            tab[i] = ' ';
            backtrack(tab, index + 1 , left , right - 1);
            tab[i] = ')';
        }
    }
}

int main(int ac, char *av[])
{
    if (ac != 2)
        return 1;
    int size = _strlen(av[1]);
    char tab[size];
    int left = 0 , right = 0;
    for (int i = 0 ; i < size ; i++)
        tab[i] = av[1][i];
    tab[size] = '\0';
    for (int i = 0 ; i < size ; i++)
    {
        if (tab[i] == '(')
            left++;
        else if (tab[i] == ')')
        {
            if (left > 0)
                left--;
            else
                right++;
        }
    }
    backtrack(tab , 0 , left , right);
    return 0;
}
