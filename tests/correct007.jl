int f(int x, int y)
{
	int z = x + y;
	printInt(z);
	return z;
}

void g(int x, int y, int z)
{
	// rzutowanie - zadziala, dlaczego by nie
	return (void) f(x, y + z);
}

int main()
{
	g(1,2,3);
	return 0;
}
