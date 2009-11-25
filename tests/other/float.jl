int main()
{
	int pos = 0;

	void expect_false(boolean b)
	{
		pos++;
		printInt(pos);
		
		if (!b) printString(" : Ok\n");
		else { printString(" : Failed\n"); error(); }
		return;
	}

	void expect_true(boolean b)
	{
		pos++;
		printInt(pos);

		if (b) printString(" : Ok\n");
		else { printString(" : Failed\n"); error(); }
		return;
	}

	expect_true(1. == 1.); 
	expect_true(1. != 2.); 
	expect_true(1.  < 2.); 
	expect_true(1. <= 2.); 
	expect_true(2.  > 1.); 
	expect_true(2. >= 1.); 
	
	expect_false(1. == 2.); 
	expect_false(1. != 1.); 
	expect_false(2.  < 1.); 
	expect_false(2. <= 1.); 
	expect_false(1.  > 2.); 
	expect_false(1. >= 2.); 

	expect_false(1. == -1.); 
	expect_false(-1. == 1.); 
	expect_false(-1. != -1.); 
	expect_true(1. != -1.); 
	expect_true(1. != -1.); 
	expect_true(-1. != 1.); 

	expect_false(-1. < -2.); 
	expect_false(0. < -3.); 
	expect_false(0. < 0.); 
	expect_false(-1. < -1.); 
	expect_true(-1. <= -1.); 
	expect_false(-2. > -1.); 
	expect_false(-3. > 0.); 
	expect_false(0. > 0.); 
	expect_false(-1. > -1.); 
	expect_true(-1. >= -1.); 

	return 0;
}
