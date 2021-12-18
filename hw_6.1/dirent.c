#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "../working_with_files_lib.h"

// char file_mode(unsigned mode) {

//     switch (mode & S_IFMT) {
//         case S_IFBLK:  return 'b';   break;
//         case S_IFCHR:  return 'c';   break;
//         case S_IFDIR:  return 'd';   break;
//         case S_IFIFO:  return 'f';   break;
//         case S_IFLNK:  return 'l';   break;
//         case S_IFREG:  return 'r';   break;
//         case S_IFSOCK: return 's';   break;
//         default:       return '?';   break;
//     }

//     return '?';

// }

// //определяем тип файла
// char file_type(unsigned char dtype) {

//     switch (dtype) {
//         case DT_BLK :     return 'b'; break; 
//         case DT_CHR:      return 'c'; break;
//         case DT_DIR:      return 'd'; break;
//         case DT_FIFO:     return 'f'; break;
//         case DT_LNK:      return 'l'; break;
//         case DT_REG:      return 'r'; break;
//         case DT_SOCK:     return 's'; break;
//         case DT_UNKNOWN:  return '?'; break;
//     }

//     return '?';
// }

//записываем в буфер права доступа к файлу
void get_access(mode_t st_mode, char* buf) {
    buf[0] = st_mode & S_IRUSR ? 'r' : '-';
    buf[1] = st_mode & S_IWUSR ? 'w' : '-';
    buf[2] = st_mode & S_IXUSR ? 'x' : '-';
    buf[3] = st_mode & S_IRGRP ? 'r' : '-';
    buf[4] = st_mode & S_IWGRP ? 'w' : '-';
    buf[5] = st_mode & S_IXGRP ? 'x' : '-';
    buf[6] = st_mode & S_IROTH ? 'r' : '-';
    buf[7] = st_mode & S_IWOTH ? 'w' : '-';
    buf[8] = st_mode & S_IXOTH ? 'x' : '-';
}

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