void f(boolean x) {
	printString("f: ");
	printInt(x);
	return;
}

void g(int x) {
	printString("g: ");
	printInt(x);
	return;
}

void h(double x) {
	printString("h: ");
	printDouble(x);
	return;
}

int main() {
	f(true);
	g(123);
	h(123.4);
	return 0;
}
