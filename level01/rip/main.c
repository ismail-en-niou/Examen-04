
#include <stdio.h>
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	is_valid(char *str)
{
	int	bal  = 0;
    // puts(str);
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '(')
			bal++;
		else if (str[i] == ')')
		{
			bal--;
			if (bal < 0)
				return (0);
		}
	}
	return (bal == 0);
}

void	backtrack(int index, char *str, int left, int right)
{
	if (left == 0 && right == 0)
	{
		if (is_valid(str))
			puts(str);
		return ;
	}
	for (int i = index; str[i]; i++)
	{
		if (str[i] == ')' && right > 0)
		{
			str[i] = ' ';
			backtrack(i + 1, str, left, right - 1);
			str[i] = ')';
		}
		else if (str[i] == '(' && left > 0)
		{
			str[i] = ' ';
			backtrack(i + 1, str, left - 1, right);
			str[i] = '(';
		}
	}
}

int	main(int ac, char *av[])
{
	if (ac != 2)
		return (1);
	int left, right;
	int size = ft_strlen(av[1]);
	char str[size + 1];
	for (int i = 0; i < size; i++)
		str[i] = av[1][i];
	str[size] = '\0';
	for (int i = 0; i < size; i++)
	{
		if (str[i] == '(')
			left++;
		else if (str[i] == ')')
		{
			if (left > 0)
				left--;
			else
				right++;
		}
	}
	backtrack(0, str , left , right);
	return (0);
}