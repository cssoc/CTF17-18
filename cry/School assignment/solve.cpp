#include<cstdio>
#include<cstring>
using namespace std;
int len;
char encrypted[1010];
char flag[1010];
FILE *f;
int main(){
    //ROT13 is a simple substitution cypher, a special case of the Caesar
    //cypher, only that a letter is replaced by the 13th letter after it
    //It is really insecure, as one just needs to apply it again in order to
    //decipher it.
    f = fopen("encrypted.txt","r");
    fscanf(f,"%s",encrypted);
    len = strlen(encrypted);
    for(int i=0;i<len;i++){
        if(encrypted[i] >= 'A' && encrypted[i] <= 'Z'){
            int pos = encrypted[i] - 'A' + 13;
            if(pos >= 26)
                pos -= 26;
            flag[i] = pos + 'A';
        }
        else
            flag[i] = encrypted[i];
    }
    printf("%s",flag);
    fclose(f);



    return 0;
}
