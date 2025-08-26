#include <stdio.h>

// int ft_strlen(char *str)
// {
//     int i =0 ;
//     while(str[i])
//         i++;
//     return i;
// }

// void ft_print (char *str )
// void backtrack(int index , int left , int right)
// {
//     if (left == 0 && right == 0)
//     {
//         if (is_valid())
//             ft_print();
//     }
// }

// int main(int ac , char *av[])
// {
//     int left , right =0;
//     int len = ft_strlen(av[1]);
//     char tab[len + 1];
//     for (int i = 0; i < len ; i++)
//         tab[i] = av[1][i];
//     for (int i = 0 ; i < len ; i++)
//     {
//         if (tab[i] == '(')
//             left++;
//         else if (tab[i] == ')')
//         {
//             if (left > 0)
//                 left--;
//             else
//                 right++;
//         }
//     }
//     backtrack()
// }
int main(int ac , char *av[])
{
    int tab[5] = {1,2,3,4,5};
    int *p = tab + 1;
    printf("%d\n",*p);
    printf("%d\n",*p + 3);
    printf("%d\n",*(p + 3));
    printf("%d\n",*a + 3);
}