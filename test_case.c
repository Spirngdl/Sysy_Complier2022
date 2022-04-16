
int n;
int a[10];
int main()
{
	int s = a[0];
	a[0] = 20;
	a[3] = 3;
	a[s] = s;
	return s;
}
