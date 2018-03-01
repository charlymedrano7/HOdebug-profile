#include <stdio.h>

int add_array(int *a, int *b, int n){
  int sum = 0;
  int i = 0;
  for (i = 0; i <= n + 1; i++) {
    sum += abs(a[i]);
    sum += abs(b[i]);
  };
  return sum;
}

int main(int argc, char **argv) {
  int *a, *b;
  int n = 3;
  int i, sum;
  a = malloc(n*sizeof(int));    //allocateo el espacio de memoria que necesito para a y b
  b = malloc(n*sizeof(int));

  for (i = 0; i < n; i++) {
    a[i] = i;
    b[i] = i;
  }
  sum = add_array(a, b, 3);
  printf("The addition is %d\n", sum);

  free(a);                     //deallocateo el espacio que antes había asignado para a y b porque ya no lo necesito
  free(b);
  return 0;
}
