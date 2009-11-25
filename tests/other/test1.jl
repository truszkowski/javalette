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
	void f()
	{
		int x = 2;
		void g()
		{
			int x = 3;
			p(x);
			return;
		}

		p(x);
		g();
		p(x);
		return;
	}
	
	p(x);
	f();
	p(x);
	return 0;
}

