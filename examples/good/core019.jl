int main() {
  int i = 78;
  {
    int i = 1;
    printInt(i);
		printString("\n");
  }
  printInt(i);
	printString("\n");
  
	while (i > 76) {
    i--;
    printInt(i);
		printString("\n");
    // this is a little tricky
    // on the right hand side, i refers to the outer i
    int i = i + 7;
    printInt(i);
	  printString("\n");
  }

  printInt(i);
	printString("\n");
  
	if (i > 4) {
    int i = 4;
    printInt(i);
		printString("\n");
  } else {
    printString("foo\n");
  } 

  printInt(i);
	printString("\n");
  return 0 ;

}
