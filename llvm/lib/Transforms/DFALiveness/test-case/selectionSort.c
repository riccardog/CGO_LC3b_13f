

int main() {

  int N = 50;
  int a[N];
  int x = 32;
  int i;
  i=0;
  while(i < N - 1) {
    a[i] = a[i+1]-i;
    i++;
  }

  i=0;
  while(i < N) {
    int j=i;
    int min_val = a[i];
    int min_index = i;
    while(j < N) {
      if(a[j] < min_val) {
	min_val = a[j];
	min_index = j;
      }
      j++;
    }
    int temp = a[i];
    a[i] = a[min_index];
    a[min_index] = temp;
    i++;
  }

  i = 10;
  return a[13];



}
