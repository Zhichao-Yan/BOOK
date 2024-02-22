
extern int num;
int glob_add = 100;
int add(int a,int b)
{
    static int cnt = 0;
    cnt++;
    num++;
    int c = glob_add;
    return a + b;
}
