#include <unistd.h>
#include <stdio.h>

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

int is_valid(char *str)
{
    int bal; 
    for (int i = 0 ; str[i] ;i ++)
    {
        if (str[i] == '(')
            bal ++;
        else if (str[i] == ')')
        {
            bal --;
            if (bal < 0 ) return 0;
        }
    }
    return (bal == 0);
}

void ft_puts(char *s) {
    while (*s) write(1, s++, 1);
    write(1, "\n", 1);
}

void backtrack(char *s, int i, int left, int right) {
    if (left == 0 && right == 0) {
        if (is_valid(s))
            ft_puts(s);
        return;
    }

    for (int j = i; s[j]; j++) {
        if (s[j] == '(' && left > 0) {
            s[j] = '_';
            backtrack(s, j + 1, left - 1, right);
            s[j] = '(';
        } else if (s[j] == ')' && right > 0) {
            s[j] = '_';
            backtrack(s, j + 1, left, right - 1);
            s[j] = ')';
        }
    }
}


int main ( int ac , char *av[])
{
    if (ac != 2)
        return 1;
    int len = ft_strlen(av[1]);
    int left = 0 , right = 0 ;
    char str[len + 1];
    for (int i = 0 ; i < len ; i++)
        str[i] = av[1][i];
    str[len] = '\0';
    for (int i = 0 ; i < len ; i++)
    {
        if (str[i] == '(')
            left++;
        else if (str[i] == ')')
        {
            if (left > 0)
                left --;
            else 
                right ++;
        }
    }
    backtrack(str , 0 , left , right);
}