int f()
{
	return g();
}

double g()
{
	return h(); // h zwraca void
}

void h()
{
	return printString("hola");
}

int main()
{
	printInt(f());
	return 0;
}
