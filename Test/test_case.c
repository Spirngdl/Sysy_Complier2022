int f(int i,int j)
{
    return i+j;
}

int main()
{
    int i = 10;
    int j = 1;
    j=j+1+i;
    i = j *5+3-j;
    i=f(j,3);
    return 0;
}
