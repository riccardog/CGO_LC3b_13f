

int main() {

  int N = 50;
  int a[N];
  int x = 32;
  int i;
  i=1;
  while(i < N) {
    a[i] = a[i-1]+i;
    i++;
  }

  int found=0;
  i=0;
  while(i<N &&!found) {
    if(a[i] == x) 
      found = 1;
    i++;
  }

  return found;


}
