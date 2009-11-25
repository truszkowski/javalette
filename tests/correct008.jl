int main()
{
	int i, j, k, n = 10, s = 0;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				s++;
	
	printInt(s);
	printString("\n");

	while (s > n) {
		s = s / n;
		n--;
	}

	printInt(s);
	printString("\n");

	return 0;
}
