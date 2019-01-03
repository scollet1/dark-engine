#ifndef TYPES_HPP
#define TYPES_HPP

/*
** a node in our doubly linked
** list with a void pointer
** to any data type, described
** by size
*/
typedef struct		s_node
{
	size_t			size;
	void			*data;
	struct s_node	*next;
	struct s_node	*prev;
}					t_node;

/*
** a linked list of length with
** pointers to head and tail
** offering O(1) access with
** appending or popping
** from either side of the
** list, as well as a
** convenient exit condition
** during list traversal
*/
typedef struct		s_list
{
	size_t 			len;
	t_node			*head;
	t_node			*tail;
}					t_list;

typedef struct		s_tree
{
	size_t 			len;
	t_list			*children;
} t_tree;

#endif