#include <stdio.h>
#include <stdlib.h> // change this to <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;



node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//...

node *parse_mul(char **s);
node    *parse_expr(char **s);
node *parse_string(char **s);


node *parse_string(char **s)
{
    node *res = NULL;
    node tmp;

    if (**s == '(')
    {
        (*s)++;
        res = parse_expr(s);
        if (!res || **s != ')')
        {
            destroy_tree(res);
            unexpected(**s);
            return NULL;
        }
        (*s)++;
        return res;
    }
    if (isdigit(**s))
    {
        tmp.val = **s - '0';
        tmp.type = VAL;
        (*s)++;
        res = new_node(tmp);
        return res;
    }
    unexpected(**s);
    return NULL;
}

node    *parse_mul(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_string(s);
    if (!left)
        return (NULL);
    while (**s == '*')
    {
        (*s)++;
        right = parse_string(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        tmp.type = MULTI;
        tmp.l = left;
        tmp.r = right;
        left = new_node(tmp);
    }
    return (left);
}



node    *parse_expr(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_mul(s);
    if (!left)
        return (NULL);
    while (**s == '+')
    {
        (*s)++;
        right = parse_mul(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        tmp.type = ADD;
        tmp.l = left;
        tmp.r = right;
        left = new_node(tmp);
    }
    return (left);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int check_balance(char *s)
{
	int flag = 0;
	int i = 0;

	while (s[i])
	{
		if(s[i] == '(')
			flag++;
		if(s[i] == ')')
		{
			flag--;
			if(flag < 0)
				return -1;
		}
		i++;
	}
	return flag;
}


int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    if (check_balance(argv[1]) == -1)
        return(printf("Unexpected token ')'\n"), 1);
    node *tree = parse_expr(&argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}