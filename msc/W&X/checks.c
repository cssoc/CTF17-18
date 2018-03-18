#define flag "CSSOC{Who_needs_DEP_anyways?}"
#define flag_len 29

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char check_len(char *input)
{
	return strlen(input) ^ 0xff;
}

char check_char3(char *input)
{
    return input[3] ^ 11;
}

char check_char18(char *input)
{
    return input[18] ^ 122;
}

char check_char15(char *input)
{
    return input[15] ^ 117;
}

char check_char28(char *input)
{
    return input[28] ^ 18;
}

char check_char23(char *input)
{
    return input[23] ^ 37;
}

char check_char21(char *input)
{
    return input[21] ^ 34;
}

char check_char27(char *input)
{
    return input[27] ^ 89;
}

char check_char10(char *input)
{
    return input[10] ^ 90;
}

char check_char0(char *input)
{
    return input[0] ^ 71;
}

char check_char9(char *input)
{
    return input[9] ^ 90;
}

char check_char19(char *input)
{
    return input[19] ^ 10;
}

char check_char7(char *input)
{
    return input[7] ^ 240;
}

char check_char14(char *input)
{
    return input[14] ^ 76;
}

char check_char26(char *input)
{
    return input[26] ^ 76;
}

char check_char2(char *input)
{
    return input[2] ^ 21;
}

char check_char12(char *input)
{
    return input[12] ^ 175;
}

char check_char24(char *input)
{
    return input[24] ^ 76;
}

char check_char1(char *input)
{
    return input[1] ^ 215;
}

char check_char20(char *input)
{
    return input[20] ^ 234;
}

char check_char4(char *input)
{
    return input[4] ^ 3;
}

char check_char5(char *input)
{
    return input[5] ^ 217;
}

char check_char13(char *input)
{
    return input[13] ^ 73;
}

char check_char16(char *input)
{
    return input[16] ^ 217;
}

char check_char22(char *input)
{
    return input[22] ^ 195;
}

char check_char6(char *input)
{
    return input[6] ^ 182;
}

char check_char17(char *input)
{
    return input[17] ^ 189;
}

char check_char8(char *input)
{
    return input[8] ^ 67;
}

char check_char25(char *input)
{
    return input[25] ^ 134;
}

char check_char11(char *input)
{
    return input[11] ^ 11;
}

char solved(void)
{
	puts("That's the flag. What are you waiting for? Submit it!");
	exit(0);
	return 0;
}
