#include <stdio.h>
#define printInt(i) printf("%d", i);
#define printString(s) printf("%s", s);

int main()
{
	int f(int a, int b) 
	{
		int g(int a, int b) 
		{
			int f(int a) // tutaj mozemy zadeklarowac znow 'f'
			{
				int f(int a, int b) // i tutaj tez mozemy
				{
					if (a > b) return a - b;
					else return b - a;
				}

				int s = 0;
				while (a > 0) {
					s = s + f(a, -a);
					a--;
				}

				return s;
			}

			return f(a * b);
		}

		int s = 0;

		while (a < b) {
			s = s + g(a, b);
			a++;
		}

		return s;
	}

	int ff = f(100,200);

	printInt(ff);
	printString("\n");

	return 0;
}
