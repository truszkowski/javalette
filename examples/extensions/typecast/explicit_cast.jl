int main() {
  int x = 5;
  double y = 42.3;
  x = (int)y;
  printInt(x); printString("\n");
  y = (double)x;
  prD(y);
  x = (int)3423.56;
  printInt(x); printString("\n");
  y = (double)4;
  prD(y);
  printInt((int)6.7); printString("\n");
  prD((double)126234);
  prD(453.61233254);
}

void prD (double d) {
  printInt((int)d); printString("\n");
  int j = 0;
  d = d - floor(d);
  printString(".\n");
  while (j < 5) {
    d = d * 10.0;	
    int x = (int)d;
    printInt((int)d); printString("\n");
    d = d - floor(d);
    j++;
  }
}

double floor(double d) {
  return (double)((int)d);
}
