#include<cstdio>
#include<cstring>
using namespace std;
int len;
char flag[1010];
char encrypted[1010];
FILE *f;
int main(){
    //ROT13 is a simple substitution cypher, a special case of the Caesar
    //cypher, only that a letter is replaced by the 13th letter after it
    //It is really insecure, as one just needs to apply it again in order to
    //decipher it.
    //printf("%s",flag);
    f = fopen("flag.txt","r");
    fscanf(f,"%s",flag);
    len = strlen(flag);
    for(int i=0;i<len;i++){
        if(flag[i] >= 'A' && flag[i] <= 'Z'){
            int pos = flag[i] - 'A' + 13;
            if(pos >= 26)
                pos -= 26;
            encrypted[i] = pos + 'A';
        }
        else
            encrypted[i] = flag[i];
    }
    printf("%s",encrypted);
    fclose(f);
    return 0;
}
