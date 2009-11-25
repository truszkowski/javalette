double f(double a, double b) 
{
	// W taki sposob beda pogrupowane if-y

	if (b == 12) 
		if (b == 23)
			if (b == 45) 
				return 56;
			else
				if (a == 78)
					if (a == 89) 
						if (a == 90)
							return 12;
						else 
							return 23;
					else
						return 34;
	
	return 0;
}

int main()
{
	double s = f(12, 456);

	if (s != 234) 
		if (s != 123) 
			if (s != 456)
				printString("-a-b-c");
			else 
				printString("C");
		else 
			printString("B");
	else 
		printString("A");

	return 0;
}

