void    *memalloc(size_t size) {
	void	*res;

	res = malloc(size);
	if (!res)
		return (NULL);
	bzero(res, size);
	return (res);
}