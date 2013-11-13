int main() {

  int x = 30;
  int y = -5;
  int z = 2;
  int res;
  
  if(x < y) 
    res = y;
  else
    res = x;

  if(res < z)
    res = z;
  

  return res;


}
