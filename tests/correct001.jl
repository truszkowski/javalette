int f(int x) { 
	int y = x * x; 
	return y; 
}

int g(int a, int b) 
{
	int c = a + b; 
	return c; 
}

int main()
{
	int fx = f(23);
	int gab = g(12, 23);
	
	printString("fx: ");
	printInt(fx);
	printString("\n");
	printString("gab: ");
	printInt(gab);
	printString("\n");

	return 0;
}

