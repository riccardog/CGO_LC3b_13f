int main () {
	unsigned i; 
	unsigned ii; 
	unsigned j;
	unsigned k;	
	i = 1;
	j = 1;
	k = 0;
	ii = i;

	while ( k < 100 ) {
		if ( j < 20 ) {
			j = ii;
			k = k + 1; 
		} else {
			j = k;
			k = k + 2;
		}
	}

	return j;
}
