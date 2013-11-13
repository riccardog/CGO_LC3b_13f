int main() {

  int x = 30;
  int y = -5;
  int z = y + 1;
  int res;

  if(y < x) 
    res = y + x + z;
  else
    res = x;

if(x < z)
    res = z;
else 
    res = z; 

return res;


}
