#include<cstdio>
#include<cstring>
using namespace std;
int len;
char flag[1010]={'L','U','K','E',' ','I',' ','A','M',' ','Y','O','U','R',' ','F','A','T','H','E','R'};
char encrypted[1010];
int main(){
    //ROT13 is a simple substitution cypher, a special case of the Caesar
    //cypher, only that a letter is replaced by the 13th letter after it
    //It is really insecure, as one just needs to apply it again in order to
    //decipher it.
    //printf("%s",flag);
    len = strlen(flag);
    for(int i=0;i<len;i++){
        if(flag[i] != ' '){
            int pos = flag[i] - 'A' + 13;
            if(pos >= 26)
                pos -= 26;
            encrypted[i] = pos + 'A';
        }
        else
            encrypted[i] = ' ';
    }
    printf("%s",encrypted);
    return 0;
}
