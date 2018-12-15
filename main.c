#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include"unicmp.h"

int main(int argc, char *argv[]) {
    if (!setlocale(LC_ALL, "zh_TW.UTF-8")) {
        fprintf(stderr, "Error:Please check LANG, LC_CTYPE, LC_ALL.\n");
        return 1;
    }

    FILE* file = fopen("bible.tsv", "r");
    char* pattern = "創造";
    char line[1000];
    while(fgets(line, sizeof(line), file)) {
        for(int i = 0; i < strlen(line) - 1;) {
            for(int j = 0; j < strlen(pattern) - 1;) {
                if(is_ascii(line[i]) && !is_ascii(pattern[j])) {
                    i++;
                    j+=3;
                } else if(!is_ascii(line[i]) && is_ascii(pattern[j])) {
                    i+=3;
                    j++;
                } else if(is_ascii(line[i]) && is_ascii(pattern[j])) {
                    if(asciichrcmp(line[i], pattern[j])) {
                        printf("%s", line);
                    }

                    i++;
                    j++;
                } else if(!is_ascii(line[i]) && !is_ascii(pattern[j])) {
                    char t[3], p[3];
                    for(int k=0; k<3; k++) {
                        t[k] = line[i+k];
                        p[k] = pattern[j+k];
                    }
                    
                    if(unichrcmp(t, p)) {
                        printf("%s", line);
                    }

                    i+=3;
                    j+=3;
                }
            }
        }
    }
}