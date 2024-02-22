extern int num;
int glob_mult = 100;
int mult(int a,int b)
{
    static int cnt = 0;
    cnt++;
    num++;
    int c = glob_mult;
    return a * b;
}