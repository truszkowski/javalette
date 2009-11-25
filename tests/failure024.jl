// warunek while jest void-em

void f(int x)
{
	return;
}

int main()
{
	while (f(1)) {
		return 1;	
	}

	return 0;
}
