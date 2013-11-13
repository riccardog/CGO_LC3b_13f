

int main() {

  int N = 50;
  int a[N];
  int x = 32;
  int i;
  i=0;
  while(i < N - 1) {
    a[i] = a[i+1]+i+i;
    i++;
  }

  i=0;
  while(i < N) {
    int j=i;
    while(0 <= j - 1 && a[j-1] > a[j]) {
      int temp = a[j-1];
      a[j-1] = a[j];
      a[j] = temp;
      j--;
    } 
    i++;
  }

  i = 20;
  return a[21];



}
