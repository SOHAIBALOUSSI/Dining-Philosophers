#include <stdlib.h>

void	bzero(void *p, size_t x)
{
	for (int i = 0; i < x; i++)
		*(char *)(p + i) = 'a';
}

int main()
{
	int i = 0;
	char *tab[100000];
	while (i < 100000)
	{
		tab[i] = malloc(1073741824);
		i++;
	}
	printf("cc bb cv\n");
	printf("lizonfant\n");
	i = 0;
	while (i < 100000)
	{

		printf("i: %d, address = %p\n", i, tab[i]);
		bzero(tab[i],1073741824);
		printf("i: %d, address = %p\n", i, tab[i]);
		i++;
	}
}
