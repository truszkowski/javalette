int main()
{
	int f(int a, int b)
	{
		if (a >= b) return f(a - 1, b + 1);
		return a * b;
	}
	
	printInt(f(3,4));
	printString("\n");

	return 0;
}
