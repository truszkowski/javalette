// nie mozna zrzutowac na string <- w ogole nie ma takiego typu

int main()
{
	int abc = (int)(double)(string)(boolean)5.6;
	return abc;
}
