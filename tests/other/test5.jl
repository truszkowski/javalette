int main()
{
	int i;

	for (i = 0; i < 10; i++) {
		int n = readInt(); 
		printInt(i);
		printString(": readInt(): ");
		printInt(n);
		printString("\n");
	}
	
	return 0;
}
