int main()
{
	int f(int a, int b)
	{
		printInt(a);
		printString(" + ");
		printInt(b);
		printString(" = ");
		return a + b;
	}
	
	printInt(f(3,4));
	printString("\n");

	return 0;
}
