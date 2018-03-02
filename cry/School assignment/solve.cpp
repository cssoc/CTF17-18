#include<cstdio>
#include<cstring>
using namespace std;
int len;
char encrypted[1010]={'Y','H','X','R',' ','V',' ','N','Z',' ','L','B','H','E',' ','S','N','G','U','R','E'};
char flag[1010];
int main(){
    //ROT13 is a simple substitution cypher, a special case of the Caesar
    //cypher, only that a letter is replaced by the 13th letter after it
    //It is really insecure, as one just needs to apply it again in order to
    //decipher it.
    len = strlen(encrypted);
    for(int i=0;i<len;i++){
        if(encrypted[i] != ' '){
            int pos = encrypted[i] - 'A' + 13;
            if(pos >= 26)
                pos -= 26;
            flag[i] = pos + 'A';
        }
        else
            flag[i] = ' ';
    }
    printf("%s",flag);




    return 0;
}
