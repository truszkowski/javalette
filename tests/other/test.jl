void fun(int a, int b, int c)
{
	printString("  fun:");
	printString(" a="); printInt(a);
	printString(" b="); printInt(b);
	printString(" c="); printInt(c);
	printString(" -- Jest\n");
	return;
}

int main()
{
	int x = 100;
	int y = 200, z = 300;

	if (x <= 100)
		printString("x <= 100\n");
	
	boolean b = true;

	if (b) 
		printString("b == true\n");

	printString("-1:-\n");
	printString("  fun: a=1 b=2 c=3 -- Powinno byc\n");
	fun(1, 2, 3);
	printString("-1:-\n");

	printString("-2:-\n");
	printString("  fun: a=100 b=200 c=300 -- Powinno byc\n");
	fun(x, y, z);
	printString("-2:-\n");

	{
		{
			printString("-3:-\n");
			printString("  fun: a=300 b=200 c=100 -- Powinno byc\n");
			fun(z, y, x);
			printString("-3:-\n");
		}
	}

	{
		x = 5;
		{	
			int y = 6;
			{
				int z = 7;
				printString("-4:-\n");
				printString("  fun: a=5 b=6 c=7 -- Powinno byc\n");
				fun(x,y,z);
				printString("-4:-\n");
			}
			printString("-5:-\n");
			printString("  fun: a=5 b=6 c=300 -- Powinno byc\n");
			fun(x,y,z);
			printString("-5:-\n");
		}
		printString("-6:-\n");
		printString("  fun: a=5 b=200 c=300 -- Powinno byc\n");
		fun(x,y,z);
		printString("-6:-\n");
	}	
	
	printString("-7:-\n");
	printString("  fun: a=5 b=200 c=300 -- Powinno byc\n");
	fun(x,y,z);
	printString("-7:-\n");

	void f()
	{
		int x = 900, y = 800;
		
		printString("-8:-\n");
		printString("  fun: a=900 b=800 c=300 -- Powinno byc\n");
		fun(x,y,z);
		printString("-8:-\n");

		void g() 
		{
			z = 700;
			int x = 600;
			void h()
			{
				int x = 1000, y = 2000;

				printString("-9:-\n");
				printString("  fun: a=1000 b=2000 c=700 -- Powinno byc\n");
				fun(x,y,z);
				printString("-9:-\n");
				return;
			}
	
			h();

			printString("-a:-\n");
			printString("  fun: a=600 b=800 c=700 -- Powinno byc\n");
			fun(x,y,z);
			printString("-a:-\n");
			return;
		}

		g();

		printString("-b:-\n");
		printString("  fun: a=900 b=800 c=700 -- Powinno byc\n");
		fun(x,y,z);
		printString("-b:-\n");
		return;
	}

	f();

	printString("-c:-\n");
	printString("  fun: a=5 b=200 c=700 -- Powinno byc\n");
	fun(x,y,z);
	printString("-c:-\n");

	return 13;
}
