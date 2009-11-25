/* test input */

int main() {
  int x = readInt();
  double y = readDouble();

  printInt(x-5);printString("\n");
  
  if (y > 42.0 || y < 43.0) 
    printString("yay!\n");
  else
    printString("nay!\n");
  return 0 ;

}
