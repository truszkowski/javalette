// argumenty w funkcji musza sie inaczej nazywac

int main()
{
	int f(int a, int a) { return a + a; }

	return f(1,2);
}
