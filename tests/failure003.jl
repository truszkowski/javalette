int main()
{
	int a = 5 + 4;
	int b = 4 + 2;
	int c = 3 + 7;
	int d = a + b;
	int e = c + e; // e nie jest tutaj znane
	int f = d + e;
	printInt(f);
	return 0;
}
