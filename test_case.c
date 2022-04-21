int b;
int d;
int c;
int main(){
	int a=0;
	d = 1;
	b=3;
	c=4;
	b = a + c;
	while(a>0){	
		b = b+a;
		a = a-1;
		a = a + d;
	}
	return b;
}
