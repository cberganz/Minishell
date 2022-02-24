#include "libft.h"

char	*ft_strinsert(char **src, char *to_insert, char *begin, char *end)
{
	int		start;
	int		stop;
	char	*nstr;

	start = 0;
	stop = 0;
	while (!ft_strnequ(*src + start, begin, ft_strlen(begin)))
		start++;
	while (!ft_strnequ(*src + start + stop, end, ft_strlen(end)))
		stop++;
	if (mem_alloc(ft_strlen(*src) + ft_strlen(to_insert) - stop + 3, (void **)&nstr))
		return (NULL);
	nstr[0] = '\0';
	ft_strlcat(nstr, *src, start + 1);
	ft_strlcat(nstr + start, to_insert, ft_strlen(to_insert) + 1);
	ft_strlcat(nstr + stop, *src + start + stop, ft_strlen(*src));
	mem_remove(*src);
	*src = nstr;
	return (nstr);
}
//
//#include <stdio.h>
//
//int	main(void)
//{
//	char *src;
//
//	src = "$TEST";
//
//	printf("%s\n", ft_strinsert(&src, "blabla", "$", " "));
//	return (0);
//}
