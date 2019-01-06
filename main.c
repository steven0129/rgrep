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

    FILE* file;
    char* filename = "";
    char* pattern = "";
    char* neg = "";
    char* key = "@content";
    char* delimeter = "@record";
    int k_tolerence = 2;

    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "-e")==0) k_tolerence = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-p")==0) pattern = strdup(argv[i+1]);
        else if(strcmp(argv[i], "-n")==0) neg = strdup(argv[i+1]);
        else if(strcmp(argv[i], "-f")==0) filename = strdup(argv[i+1]);
        else if(strcmp(argv[i], "-k")==0) key = strdup(argv[i+1]);
        else if(strcmp(argv[i], "-d")==0) delimeter = strdup(argv[i+1]);
    }

    if(strcmp(pattern, "")==0) {
        printf("./rgrep -p 'something you want to find' -f 'your file name' -e k_error\n");
        return 0;
    }

    if(strcmp(filename, "")==0) {
        file = stdin;
    } else {
        file = fopen(filename, "r");
    }

    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char rec[4000];
    char* recs[40000];
    char* target[40000];
    int counter = 0, idx = 0, tid = 0;
    while(1) {
        for(int i=0; i<4000; i++)  rec[i] = fgetc(file);
        char* head = strdup(rec);
        
        while(strstr(head, delimeter) != NULL && strstr(head+7, delimeter) != NULL) {
            head = strstr(head, delimeter);
            char* nextRec = strstr(head+7, delimeter);
            char* curr = malloc(nextRec - head + 1);
            strncpy(curr, head, nextRec - head);
            curr[nextRec - head] = '\0';
            counter += strlen(curr);
            head = nextRec;
            recs[idx++] = strdup(curr);
            free(curr);
        }
        
        if(feof(file)) {
            head[fileSize - counter] = '\0';
            recs[idx++] = strdup(head);
            break;
        } else fseek(file, counter, SEEK_SET);
    }

    for(int i=0; i<idx; i++) {
        char* content = strstr(recs[i], key) + strlen(key) + 1;
        int err = unistrcmp(content, pattern, k_tolerence);
        if(err != -1) target[tid++] = strdup(recs[i]);
    }

    if(neg[0] != '\0') {
        int temp = tid;
        tid = 0;
        for(int i=0; i<temp; i++) {
            char* content = strstr(target[i], key) + strlen(key) + 1;
            int err = unistrcmp(content, neg, k_tolerence);
            if(err == -1) target[tid++] = strdup(target[i]);
        }
    }

    for(int i=0; i<tid; i++) {
        printf("%s----------------\n", target[i]);
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

        if(A[jj-1] <= k) return A[jj-1];
        for(int j=0; j < jj; j++) B[j] = A[j];
        
        if(is_ascii(text[i])) i++;
        else i+=3;
    }

    return -1;
}

int min(int a, int b) {
    return a < b ? a : b;
}