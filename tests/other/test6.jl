void p(double x)
{
	printString("x = ");
	printDouble(x);
	printString("\n");
	return;
}

int main()
{
	double x = 1.2;

	printDouble(x); // 1
	printString("\n");

	void f()
	{
		double x = 2.34;
		void g()
		{
			x = 3.567;
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

