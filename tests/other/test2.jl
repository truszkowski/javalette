void p(int x)
{
	printString("x = ");
	printInt(x);
	printString("\n");
	return;
}

int main()
{
	int x = 1;

	printInt(x); // 1
	printString("\n");

	void f()
	{
		int x = 2;
		void g()
		{
			x = 3;
			p(x);
			return;
		}

		p(x); // 2
		g();  // 3
		p(x); // 3
		return;
	}

	p(x); // 1
	f();  // 2, 3, 3
	p(x); // 1

	printString("Bedzie error?\n");
	error();

	return 0;
}

