#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "../working_with_files_lib.h"

int main() {

    DIR *dir = opendir(".");
    if (!dir) {
        fprintf(stderr, "Can't open current directory");
        perror("Can't open current directory");
        return 0;
    }

    //определение типа и права доступа, вывод в консоль
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char entry_type = dtype_char(entry->d_type);

        struct stat sb;
        assert((lstat(entry->d_name, &sb)) == 0);

        //определяем права доступа к файлу и выводим
        printf("[%s] ", print_access_rights(sb.st_mode));

        //определяем тип файла
        if (entry_type == '?') {
            entry_type = file_mode(sb.st_mode);
        }

        //выводим тип файла и имя
        printf("%c  %s\n", entry_type, entry->d_name);
    }

    closedir(dir);
    return 1;
}