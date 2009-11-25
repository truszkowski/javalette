int main()
{
	int x = 10;
	printString("x = "); printInt(x); printString("\n"); // 10

	void f()
	{
		x = 11;
		return printString("first f\n");
	}

	{
		int x = 12;
		f(); // f zmieni wczesniejszego x-a (z 10 na 11)
		printString("x = "); printInt(x); printString("\n"); // 12
		
		void f()
		{
			x = 13;
			return printString("second f\n");
		}

		f(); // aktualny x zmieniony z 12 na 13
		printString("x = "); printInt(x); printString("\n"); // 13
	}
	
	printString("x = "); printInt(x); printString("\n"); // 11
	f();
	printString("x = "); printInt(x); printString("\n"); // 11
	
	return 0;
}
