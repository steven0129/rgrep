#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include"unicmp.h"

int min(int, int);
int unistrcmp(char*, char*, int);

int main(int argc, char *argv[]) {
    if (!setlocale(LC_ALL, "zh_TW.UTF-8")) {
        fprintf(stderr, "Error:Please check LANG, LC_CTYPE, LC_ALL.\n");
        return 1;
    }

    FILE* file = fopen("bible.tsv", "r");
    char* pattern = "願主耶穌的恩惠";
    char line[1000];
    while(fgets(line, sizeof(line), file)) {
        int err = unistrcmp(line, pattern, 2);
        if(err != -1) printf("%d    %s", err, line);
    }
}

int unistrcmp(char* text, char* pattern, int k) {
    int A[strlen(pattern)], B[strlen(pattern)];

    for(int i = 0; i < strlen(pattern); i++) B[i] = i;

    for(int i = 0; i < strlen(text) - 1;) {
        int jj = 1;

        for(int j = 0; j < strlen(pattern) - 1;) {
            A[0] = 0;
            int error = 0;
            if(is_ascii(text[i]) && !is_ascii(pattern[j])) {
                j+=3;
                error = 1;
            } else if(!is_ascii(text[i]) && is_ascii(pattern[j])) {
                j++;
                error = 1;
            } else if(is_ascii(text[i]) && is_ascii(pattern[j])) {
                error = !asciichrcmp(text[i], pattern[j]);
                j++;
            } else if(!is_ascii(text[i]) && !is_ascii(pattern[j])) {
                char t[3], p[3];
                for(int k=0; k<3; k++) {
                    t[k] = text[i+k];
                    p[k] = pattern[j+k];
                    
                }

                error = !unichrcmp(t, p);
                j+=3;
            }

            A[jj] = min( min(A[jj-1] + 1, B[jj] + 1) , B[jj-1] + error );
            jj++;
        }

        if(A[jj - 1] <= k) return A[jj - 1];
        for(int j=0; j < jj; j++) B[j] = A[j];
        
        if(is_ascii(text[i])) i++;
        else i+=3;
    }

    return -1;
}

int min(int a, int b) {
    return a<b ? a : b;
}