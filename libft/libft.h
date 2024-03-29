/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwallier <rwallier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 16:34:25 by vsergio           #+#    #+#             */
/*   Updated: 2023/06/29 10:22:22 by rwallier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include "get_next_line/get_next_line.h"
# include "get_next_line/get_next_line_bonus.h"
# include "ft_printf/ft_printf.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

int		ft_isalpha(int a);

int		ft_isdigit(int a);

int		ft_isalnum(int a);

int		ft_isascii(int c);

int		ft_isprint(int c);

int		ft_isspace(char c);

size_t	ft_strlen(const char *str);

void	*ft_memset(void *dest, int c, size_t offset);

void	ft_bzero(void *dest, size_t len);

void	*ft_memcpy(void *dest, const void *src, size_t len);

int		ft_atoi(const char *str);

char	*ft_strdup(char *s1, int flag);

int		ft_toupper(int arg);

size_t	ft_strlcat(char *dest, const char *src, size_t maxlen);

char	*ft_strrchr(const char *s, int c);

int		ft_memcmp(const void *str1, const void *str2, size_t size);

size_t	ft_strlcpy(char *dest, const char *src, size_t destsize);

int		ft_tolower(int arg);

char	*ft_strchr(const char *s, int c);

int		ft_strncmp(const char *str1, const char *str2, size_t n);

int		ft_strcmp(const char *s1, const char *s2);

void	*ft_memmove(void *dest, const void *src, size_t len);

char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

void	*ft_calloc(size_t count, size_t size);

char	*ft_substr(char const *str, unsigned int start, size_t len);

char	*ft_strjoin(char *s1, char *s2, int flag);

void	ft_free_matrix(char **mat);

void	ft_putchar_fd(char c, int fd);

void	ft_putstr_fd(char *s, int fd);

char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strtrim2(char const *s1, char const *set);

char	*ft_itoa(int n);

char	*ft_itoa_base(long long int n, char *base);

char	**ft_split(char const *s, char c);

void	ft_putnbr_fd(int n, int fd);

void	ft_putendl_fd(char *s, int fd);

void	*ft_memchr(const void *src, int c, size_t length);

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

void	ft_striteri(char const *s, void (*f)(unsigned int, char *));

int		ft_count_digits_base(long long int number, int base);

t_list	*ft_lstnew(void *content);

void	ft_lstadd_front(t_list **lst, t_list *new);

int		ft_lstsize(t_list *lst);

t_list	*ft_lstlast(t_list *lst);

void	ft_lstadd_back(t_list **lst, t_list *new);

void	ft_lstdelone(t_list *lst, void (*del)(void *));

void	ft_lstclear(t_list **lst, void (*del)(void *));

void	ft_lstiter(t_list *lst, void (*f)(void *));

char	**ft_lsttochrmat(t_list *node);

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

int		ft_lstdelif(t_list **lst, char *ref);

char	**ft_split2(char const *s, char c);

#endif
