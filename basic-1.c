#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>

enum { BUFFER_SIZE = 10 };
int buffer_cwarn[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// ABV.GENERAL
// buffer overrun
void foo()
{
    char a[8]; // holds two 4-byte ints
    for (int i = 0; i < sizeof(a); i++) // flawed
    //for (int i = 0; i < sizeof(a) / sizeof(int); i++) // fixed
    {
        ((int*)a)[i] = i;
    }
}

// CWARN.ALIGNMENT
// pointer mis-alignment
int test1() {
    char *temp = (char *)buffer_cwarn;
    int res = (int) (*(temp+6)); //flawed
    return res;
}

// ---- LOCRET.ARG ----
// returning pointer to local variable
int *func_RET(unsigned n)
{
    int aux;
    int *p;
    if (n == 1) {
        p = &aux;
    } else {
        p = (int *)malloc(n * sizeof(int));
    }
    return p;
}

int main()
{
    printf("Hello, World!\n");
    
    // Cern Secure Code Vulmerability Examples
    // https://security.web.cern.ch/recommendations/en/codetools/c.shtml
    printf("Lets Check Some Cern Examples!!!\n");
    
    // cern #1
    char username[8];
    //int allow = 0;
    printf ("Enter your username, please: ");
    gets(username); // user inputs "malicious"
    printf(username);
    printf("\n");

    // cern #2
    // strcpy checker disabled by kw by default - see link below
    char str1[10];
    char str2[]="abcdefghijklmn";
    strcpy(str1,str2);
    printf(str1);
    printf("\n");

    //cern #3
    // sprintf checker disabled by kw by default - see link below
    char buffer[BUFFER_SIZE];
    int check = 0;
    sprintf(buffer, "%s", "This string is too long!");
    printf ("check: %d", check); /* This will not print 0! */
    printf("\n");

    // cern #1 redux
    // check something nearly identical
    char username_2[8];
    printf ("Enter your username, please: ");
    gets(username_2); // user inputs "malicious"
    printf(username_2);
    printf("\n");

    // klocwork checkers: https://docs.roguewave.com/en/klocwork/current/candccheckerreference
    // in particular note which ones are enable (true) or disabled (false) by default
    printf("Lets Check Some Klockwork Checkers!!!\n");

    // ---- ABV.GENERAL ----
    //example 1
    char fixed_buf[10];
    sprintf(fixed_buf,"Very long format string\n"); // flawed
    // snprintf(fixed_buf, sizeof(fixed_buf), "Very long format string\n"); // fixed

    // example 2
    // see foo() above
    //foo();
    // note: executing foo() seems to prevent further execution of subsequent code

    // ---- CWARN.ALIGNMENT ----
    // see test1() above
    
    test1();

    // ---- LOCRET.ARG ----
    // see func_RET() above
    int *pee;
    unsigned en = 1;
    pee = func_RET(en);
    en = 2;
    pee = func_RET(en);
    int twee = *pee;
    printf ("pee: %d\n", twee); /* This will not print 0! */

    // ---- MLK.MIGHT ----
    int i = 1;
    char* pre = (char*)malloc(10*sizeof(char));
    if(i) {
        pre = 0;
    }
    free(pre);
    printf ("pre: %s\n", pre); 

    // done
    printf("All Done - Better Luck Next Time!!!\n");
    return 0;

}
