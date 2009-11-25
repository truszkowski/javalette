// nie mozna deklarowac wielokrotnie funkcji na tym samym poziomie.

int main()
{
	void f() { return; }
	void f() { return; }
	
	f();
	return 0;
}
