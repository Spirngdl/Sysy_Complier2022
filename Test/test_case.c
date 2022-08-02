
int fun(int i,int j,int k,int m,int n,int l)
{
    int q =5;
    return i+j+k+m+n+l;
}

int main()
{
    int j = 5;
    int k = 30+j;
    k = fun(j,2,3,4,5,6);
    return 0;
}