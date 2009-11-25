void p(int x)
{
	printString("x = ");
	printInt(x);
	printString("\n");
	return;
}

int main()
{
	int a = 4 + 5 * 8 - 3;
	int b = 1 - 4;
	int c = 4 / 2 + 9 % 2;
	int d = 1234 % 50 - 13;
	int e = -4 + 45 / 10;
	int f = 5 * -4;
	int g = -3 * -4;
	int h = -13 / 4;
	int i = 13 / -4;
	int j = -13 % -4;
	int k = 13 % 4;
	int l = 13 % -4;
	
	p(a);
	p(b);
	p(c);
	p(d);
	p(e);
	p(f);
	p(g);
	p(h);
	p(i);
	p(j);
	p(k);
	p(l);

	return 0;
}
