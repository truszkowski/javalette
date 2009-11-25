/* Test arithmetic and comparisons. */

int main() {
    int x = 56;
    int y = -23;
    printInt(x+y);printString("\n");
    printInt(x-y);printString("\n");
    printInt(x*y);printString("\n");
    printInt(45/2);printString("\n");
    printInt(78%3);printString("\n");
    double z = -9.3;
    double w = 5.1;
    printBool(z+w > z-w);printString("\n");
    printBool(z/w <= z*w);printString("\n");
    return 0 ;
}

void printBool(boolean b) {
  if (b) {
    printString("true");
    return;
  } else {
    printString("false");
    return;
 }
}
