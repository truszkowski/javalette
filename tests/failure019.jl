// funkcja musi miec return

int main()
{
	int x = 10, y = 5;
	{
		if (x == y) return 1;
		int x, y, z = 10;
		if (x == z) return 2;
	}
	
	if (x != y && z != 10) 
		printString("Brak return\n");	
}
