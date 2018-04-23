#include<stdio.h>
#include<string.h>
#include <iostream>
// This Code is from https://www.geeksforgeeks.org/strcmp-in-c-cpp/

//strcmp() compares the two strings lexicographically means it starts comparison
//character by character starting from the first character until the characters
//in both strings are equal or a NULL character is encountered.

//If first character in both strings are equal, then this function will check
//the second character, if this is also equal then it will check the third and so on

//This process will be continued until a character in either string is NULL or the
//characters are unequal.

//This function can return three different integer values based on the comparison:

int main()
{
    //Zero(0)
    char leftStr[] = "g f g";
    char rightStr[] = "g f g";

    // Using strcmp()
    int res = strcmp(leftStr, rightStr);

    if (res==0)
        printf("Strings are equal");
    else
        printf("Strings are unequal");

    printf("\nValue returned by strcmp() is:  %d" , res);

    //Greater than zero(>0)
    // z has greater ASCII value than g
    char leftStr2[] = "zfz";
    char rightStr2[] = "gfg";

    int res2 = strcmp(leftStr2, rightStr2);

    if (res2==0)
        printf("Strings are equal");
    else
        printf("Strings are unequal");

    printf("\nValue of result: %d" , res2);

    //Less than Zero(<0)
    // b has less ASCII value than g
    char leftStr3[] = "bfb";
    char rightStr3[] = "gfg";

    int res3 = strcmp(leftStr3, rightStr3);

    if (res3==0)
        printf("Strings are equal");
    else
        printf("Strings are unequal");

    printf("\nValue returned by strcmp() is:  %d" , res3);

    return 0;
}
