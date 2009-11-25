void printx(int x)
{
	printString("x = ");
	printInt(x);
	printString("\n");
	return;
}

int main()
{
	int x = 0;
	printx(x);
	
	{
		int x = x + 1;
		printx(x);
		
		{
			int x = x + 2;
			printx(x);
		}
		
		printx(x);

		{
			int x = x;
			printx(x);
		}

		printx(x);
	}	

	printx(x);
	return 0;
}
