
int main()
{
	int a = 0;
	
	boolean f()
	{
		a = 1;
		printString("f called\n");
		return true;
	}

	boolean g()
	{
		a = 2;
		printString("g called\n");
		return false;
	}

	printString("a = "); printInt(a); printString("\n");
	
	if (f() || g()) printString("condition 1 is ok\n");
	
	printString("a = "); printInt(a); printString("\n");

	if (g() && f()) printString("condition 2 is ok\n");
	
	printString("a = "); printInt(a); printString("\n");

	return 0;
}
