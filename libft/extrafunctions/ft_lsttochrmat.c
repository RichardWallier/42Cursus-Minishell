#include "../libft.h"

char	**ft_lsttochrmat(t_list *node)
{
	char	**mat;
	int		mat_size;
	int		i;

	mat_size = ft_lstsize(node);
	if (!mat_size)
		return (NULL);
	mat = ft_calloc(mat_size + 1, sizeof(char *));
	if (!mat)
		return (NULL);
	mat[mat_size] = NULL;
	i = 0;
	while (node)
	{
		mat[i] = ft_strdup(node->content, 0);
		if (!mat[i])
		{
			ft_free_mat(mat);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (mat);
}