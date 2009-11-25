void print(int x)
{
	printString(" = ");
	printInt(x);
	printString("\n");
	return;
}

int main()
{
	int a = -34, b = -45, c = 67, d = 78;

	printString("a"); print(a);
	printString("b"); print(b);
	printString("c"); print(c);
	printString("d"); print(d);

	printString("a + b"); print(a+b);
	printString("b + c"); print(b+c);
	printString("c - d"); print(c-d);
	printString("d - a"); print(d-a);

	printString("a * b"); print(a*b);
	printString("b * c"); print(b*c);
	printString("c * b"); print(c*b);
	printString("d * c"); print(d*c);

	printString("a / b"); print(a/b);
	printString("b / a"); print(b/a);
	printString("c / b"); print(c/b);
	printString("b / c"); print(b/c);

	printString("a / 10"); print(a/10);
	printString("b / 10"); print(b/10);
	printString("c / 10"); print(c/10);
	printString("d / 10"); print(d/10);

	printString("1000 / a"); print(1000/a);
	printString("1000 / b"); print(1000/b);
	printString("1000 / c"); print(1000/c);
	printString("1000 / d"); print(1000/d);

	printString("a % b"); print(a%b);
	printString("b % a"); print(b%a);
	printString("c % b"); print(c%b);
	printString("b % c"); print(b%c);

	printString("a % 10"); print(a%10);
	printString("b % 10"); print(b%10);
	printString("c % 10"); print(c%10);
	printString("d % 10"); print(d%10);

	printString("1000 % a"); print(1000%a);
	printString("1000 % b"); print(1000%b);
	printString("1000 % c"); print(1000%c);
	printString("1000 % d"); print(1000%d);

	return 0;
}
