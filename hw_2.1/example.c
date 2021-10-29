#include "stdio.h"

int main(int argc, char** argv) {
    char* a= argv[1];
    char* b = argv[2];


    printf("%c %c %d", *a, *b, argc);
    return 0;


}