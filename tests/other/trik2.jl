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
	printx(x); // 0
	
	{
		int x = x + 1;
		printx(x); // 1
		
		{
			int x = x + 2; 
			printx(x); // 3
		}
		
		printx(x); // 1

		{
			int x = (x = 34) + 1;
			printx(x); // 35
		}

		printx(x); // 34
	}	

	printx(x); // 0
	return 0;
}
