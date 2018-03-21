#include<cstdio>
long long g,p,a,b,apublic,bpublic,s;
long long fme(long long a, long long b){
    if(b == 1)
        return a;
    else if(b % 2 == 0)
        return (fme(a, b / 2) * fme(a, b / 2) ) % p;
    else
        return ((a * fme(a, b / 2) ) % p * fme(a, b / 2) ) % p;
}
int main(){
    g = 5;
    p = 393344617;
    apublic = 379063742;
    bpublic = 317383473;
    long long cp = 1;
    for(a = 0; a <= p; a ++){
        if(cp == apublic)
            break;
        else
            cp = (cp * g) % p;
    }
    cp = 1;
    for(b = 0; b <= p; b ++){
        if(cp == bpublic)
            break;
        else
            cp = (cp * g) % p;
    }
    printf("%lld\n%lld\n",a,b);
    s = fme(apublic, b);
    printf("%lld\n",s);
    s = fme(bpublic, a);
    printf("%lld\n",s);
    return 0;
}
