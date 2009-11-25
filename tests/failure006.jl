int f(int x) 
{
	return x * x;
}

int g(int x)
{
	return x * x * x;
}

// printInt jest funkcja wbudowana
void printInt(int x)
{
	printInt(x);
	return;
}

int main()
{
	printInt(f(g(12)));
	return 0;
}
