int main()
{
	double a = 13.1 + 45.3;
	double b = 13.1 - 45.3;
	double c = 13.1 * 45.3;
	double d = 13.1 / 45.3;
	double e = 13.1 % 45.3;
	
	printString("a = "); printDouble(a); printString("\n");
	printString("b = "); printDouble(b); printString("\n");
	printString("c = "); printDouble(c); printString("\n");
	printString("d = "); printDouble(d); printString("\n");
	printString("e = "); printDouble(e); printString("\n");

	if (13.1 == 45.3) printString("==\n");
	if (13.1 != 45.3) printString("!=\n");
	if (13.1  < 45.3) printString(" <\n");
	if (13.1  > 45.3) printString(" >\n");
	if (13.1 <= 45.3) printString("<=\n");
	if (13.1 >= 45.3) printString(">=\n");

	return 0;
}
