#include <stdio.h>
#include <string.h>

int main(){

    char text1[30], text2[30];
    //printf("Enter text 1: ");
    //scanf("%s", text1);
    printf("Enter text 2: ");
    fgets(text2, 30, stdin);

    size_t len = strlen(text2);
    if (len > 0 && text2[len-1] == '\n') {
        text2[--len] = '\0';
    }

    printf("%s", text2);
    //printf("%s", text2);
    return 0;
}