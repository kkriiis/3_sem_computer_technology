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

    if (argc != 2) {
        fprintf(stderr, "Usage: %s base full path to dir-to", argv[0]);
        perror("Data entry errors");
        return 0;
    }

    //открываем копируемую дерикторию
    int dir_from = openat(AT_FDCWD, argv[2], O_RDONLY);
    if (dir_from < 0) {
        fprintf(stderr, "Can't open directory %s \n", argv[2]);
        perror("Can't open current directory");
        return 0;
    }

    struct stat dir_sb;
    assert((fstatat(AT_FDCWD, argv[2], &dir_sb, AT_SYMLINK_NOFOLLOW)) == 0);

    //создаем копию копируемой директории
    if (mkdirat(AT_FDCWD, copy_file_name(argv[2]), dir_sb.st_mode) == -1) {
        fprintf(stderr, "Failed to create directory\n");
        perror("Failed to create directory");
        return 0;
    }

    //открываем каталог-копию
    int copy_dir_from = openat(AT_FDCWD, copy_file_name(argv[2]), O_RDONLY);
    if (copy_dir_from == -1) {
        fprintf(stderr, "Can't open copy_file_dir directiry\n");
        perror("Can't open copy_dir directory");
        return 0;
    }

    //копируем
    if (copy_dir(dir_from, copy_dir_from) != 1) {
        fprintf(stderr, "Failed in copy");
        perror("Failed in copy");
        return 0;
    }

    assert(errno == 0);
    //успех!
    return 1;
}