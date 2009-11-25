// Sprawdzenie rzutowania

void f(boolean a, boolean b, boolean c)
{
	printInt(a); printString("\n");
	printInt(b); printString("\n");
	printInt(c); printString("\n");
	return;
}

int main()
{
	boolean b = true;
	int i = 10;
	double d = 123.4;

	boolean b2 = (boolean)(double)b;
	int i2 = (int)(double)i;
	double d2 = (double)(int)d;

	boolean b3 = (double)b;
	int i3 = (double)i;
	double d3 = (int)d;

	printInt(b2);
	printString("\n");
	printInt(i2);
	printString("\n");
	printDouble(d2);
	printString("\n");
	
	printInt(b3);
	printString("\n");
	printInt(i3);
	printString("\n");
	printDouble(d3);
	printString("\n");

	printDouble(b2);
	printString("\n");
	printDouble(i2);
	printString("\n");

	f(b2, i2, d2);

	return 0;
}
