#include<cstdio>
#include<cstdlib>
#include<ctime>
long long a,b,g,p,apublic,bpublic,s;
long long fme(long long a, long long b){
    if(b == 1)
        return a;
    else if(b % 2 == 0)
        return (fme(a, b / 2) * fme(a, b / 2) ) % p;
    else
        return ((a * fme(a, b / 2) ) % p * fme(a, b / 2) ) % p;
}
int main(){
    /*srand(time(0));
    a = (rand() * rand()) % p;
    b = (rand() * rand()) % p;
    printf("%d\n%d",a,b);*/
    g = 5;
    p = 393344617;
    a = 215385513;
    b = 182550661;
    apublic = fme(g, a);
    printf("%lld\n",apublic);
    bpublic = fme(g, b);
    printf("%lld\n",bpublic);
    s = fme(apublic, b);
    printf("%lld\n",s);
    return 0;
}
