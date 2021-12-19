#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/sysmacros.h>

#include "../working_with_files_lib.h"

//основная функция по определению файла и осуществлению копирования
int ftdac(struct stat *sb, char* file_from, char* file_to) {
    unsigned mode = (*sb).st_mode;
    switch (mode & S_IFMT){
        
        //обычный файл
        case S_IFREG: {

            //проверка прав на чтение файла, откуда будем копировать
            if (check_user_access(file_from, 'r', mode) != 1) {
                fprintf(stderr, "This file: %s can't be read", file_from);
                perror("Can't be read");
                return 0;
            }

            //проверка прав на запись в файл, куда будем копировать
            if (check_user_access(file_to, 'w', mode) != 1) {
                fprintf(stderr, "This file: %s can't be write", file_to);
                perror("Can't be write");
                return 0;
            }

            //в случае успешной проверки открываем файлы и удостовериваемся в том, что они открыты
            int from  = open(file_from, O_RDONLY);
            if (!from) {
                fprintf(stderr, "This file: %s failed for open", file_from);
                perror("Can't be open");
                //удаление файла
                //rm_file(file_from);
                return 0;
            }

            int to = open(file_to, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (!to) {
                fprintf(stderr, "This faile: %s failed for open", file_to);
                perror("Can't be open");
                //удаление файла
                //rm_file(file_to);
                return 0;
            }

            //копирование
            copy_file(from, to, file_to, sb);

            //закрываем файлы
            if (close(from) < 0) {
                fprintf(stderr, "Failed close file:%s", file_from);
                perror("Can't close");
                return 0;
            }

            if (close(to) < 0) {
                fprintf(stderr, "Failed close file:%s", file_to);
                perror("Can't close");
                return 0;
            }

            //копирование выполнено успешно!
            return 1;
        }

        //каталог
        case S_IFDIR: {
            if (mkdir(file_to, mode) < 0) {
                fprintf(stderr, "Failed fo crate directory:%s", file_to);
                perror("Failed to create dir");
                return 0;
            }
            fprintf(stderr, "This is directory!");
            return 0;
        }

        //fifo-файл 
        case S_IFIFO: {
            if (mkfifo(file_to, mode) < 0) {
                fprintf(stderr, "Failed to create FIFO file: %s", file_to);
                perror("Failed to create FIFO");
                return 0;
            }
            fprintf(stderr, "This is FIFO-file!!");
            return 0;
        }

        //ссылка
        case S_IFLNK: {
            copy_link(file_from, file_to);

            return 1;
        }

        //устройство
        case S_IFCHR: {
            if (mknod(file_to, mode, (*sb).st_rdev) < 0) {
                fprintf(stderr, "Failed to create new character device");
                perror("Failed to create device");
                return 0;
            }
            fprintf(stderr, "This is device!");
            return 0;
        }

        //блочное устройство?????
        case S_IFBLK: {
            fprintf(stderr, "This is block device!");
            return 0;
        }

        default: {
            fprintf(stderr, "Sorry, I can't copy this type of file");
            return 0;
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s filename-for-copy filename-to-written", argv[0]);
        return 0;
    }

    struct stat sb;

    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        return 0;
    }

    if (ftdac(&sb, argv[1], argv[2])) {
        printf("\n success");
    } else {
        return 0;
    }
    return 1;
}