#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "../working_with_files_lib.h"

//функция, выдающая новое имя файла на основе старого
const char* copy_file_name(const char *file_name) {
    char *new_file_name;
    new_file_name = (char*)malloc(sizeof(char)+sizeof("copy_"));
    return strcat(new_file_name, file_name);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s base dir name-for-written, cp dir name-to-copy", argv[0]);
        perror("Data entry errors");
        return 0;
    }

    //открываем родительскую дерикторию(там находится копируемый каталог)
    int base_dir = open(argv[1], O_RDONLY, O_DIRECTORY);
    if (!base_dir) {
        fprintf(stderr, "Can't open directory %s \n", argv[1]);
        perror("Can't open current directory");
        return 0;
    }
    
    //открываем копируемую дерикторию
    int cp_dir = openat(base_dir, argv[2], O_RDONLY);
    if (cp_dir < 0) {
        fprintf(stderr, "Can't open directory %s \n", argv[2]);
        perror("Can't open current directory");
        return 0;
    }

    struct stat dir_sb;
    assert((fstatat(base_dir, argv[2], &dir_sb, AT_SYMLINK_NOFOLLOW)) == 0);

    //создаем в родительской дериктории каталог-копию с правами исходника
    if (mkdirat(base_dir, copy_file_name(argv[2]), dir_sb.st_mode) < 0) {
        fprintf(stderr, "Failed to create directory\n");
        perror("Failed to create directory");
        return 0;
    }

    //открываем каталог-копию
    int cp_cp_dir = openat(base_dir, copy_file_name(argv[2]), O_RDONLY);
    if (cp_cp_dir == -1) {
        fprintf(stderr, "Can't open copy_file_dir directiry\n");
        perror("Can't open copy_dir directory");
        return 0;
    }

    //копируем
    if (copy_dir(cp_dir, cp_cp_dir, argv[2]) != 1) {
        fprintf(stderr, "Failed in copy");
        perror("Failed in copy");
        return 0;
    }

    //не закрываем cp_dir. т.к. в функции copy_dir мы уже хакрыли соответсвующий дескриптор
    close(cp_cp_dir);
    close(base_dir);

    assert(errno == 0);
    //успех!
    return 1;
}