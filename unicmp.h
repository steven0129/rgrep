int unichrcmp(char* text, char* pat) {
    for(int i = 0; i < 3; i++)
        if((int)text[i] != (int)pat[i])
            return 0;

    return 1;
}

int asciichrcmp(char text, char pat) {
    if(text == pat) return 1;
    else return 0;
}

int is_ascii(char chr) {
    if((int)chr < 256 && (int)chr > 0) return 1;
    else return 0;
}