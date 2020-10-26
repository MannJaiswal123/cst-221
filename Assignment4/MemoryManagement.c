#include <stdio.h>
#include <stdlib.h>

char *decimal_to_binary(int);
void decToHexa(int n);
int main()
{
  int n;
  char *p;

  printf("Enter an integer in decimal number system\n");
  scanf("%d", &n);

  p = decimal_to_binary(n);
  printf("Binary string of %d is: %s\n", n, p);
  free(p);
  decToHexa(n);
  return 0;
}

char *decimal_to_binary(int n)
{
  int c, d, t;
  char *p;
  t = 0;
  p = (char*)malloc(32+1);
  if (p == NULL)
    exit(EXIT_FAILURE);
  for (c = 31 ; c >= 0 ; c--)
  {
    d = n >> c;
    if (d & 1)
      *(p+t) = 1 + '0';
    else
      *(p+t) = 0 + '0';
    t++;
  }
  *(p+t) = '\0';
  return  p;
}
void decToHexa(int n)
{
    // char array to store hexadecimal number
    char hexaDeciNum[100];
    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;
        // storing remainder in temp variable.
        temp = n % 16;
        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
        n = n/16;
    }

    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--)
        //cout << hexaDeciNum[j];
        printf("%d",hexaDeciNum[j]);
}

