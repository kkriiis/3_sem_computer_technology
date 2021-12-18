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

//функция, удаляющая файл
// void remove_file(const char* filename) {

//     if (unlink(filename) == -1) {
//         fprintf(stderr, "It's not possible to remove this file");
//         perror("It's not possible to remove this file");
//         return;
//     }
//     printf("[+] Successful removing: %s", filename);
//     return;
// }

// //функция для чтения в буфер
// ssize_t writeall(int fd, const void *buf, size_t count) {
//     size_t bytes_written = 0;
//     const uint8_t *buf_addr = (const uint8_t*) buf;

//     while (bytes_written < count) {
//         ssize_t res = write(fd, buf_addr + bytes_written, count - bytes_written);

//         if (res < 0) {
//             return res;
//         }

//         bytes_written += (size_t) res;
//     }
//     return (ssize_t) bytes_written;
// }

// //функция, осуществляющая копирование информации, при использовании обычных файлов
// void copy_file(unsigned from, unsigned to, const char* file_to, struct stat *sb) {
    
//     //создаем буфер для чтения
//     char *buf = (char*)calloc(BUF_SIZE, sizeof(char));
    
//     long long copy_file_size = (long long) sb->st_size;
    
//     //копирование
//     while(copy_file_size > 0){
//         ssize_t read_file = read(from, buf, BUF_SIZE);
//         if (read_file < 0) {
//             fprintf(stderr, "Failed read from file");
//             perror("Failed read from file");
//             remove_file(file_to);
//             close(from);
//             return;
//         }

//         ssize_t write_file = writeall(to, buf, read_file);
//         if (write_file < 0) {
//             fprintf(stderr, "Failed write to file");
//             perror("Failed write to file");
//             remove_file(file_to);
//             close(to);
//             return;
//         }

//         if (write_file != read_file) {
//             fprintf(stderr, "The number of symbols written does't match the number of symbols read");
//             perror("The number of symbols written does't match the number of symbols read");
//             remove_file(file_to);
//             close(to);
//             return;
//         }

//         copy_file_size -= read_file;
//     }

//     free(buf);
//     return;
// }

// //функция, проверяющая права пользователя
// int check_user_access(const char* file_from, const char access, mode_t mode) {
//     switch (access) {
//         case 'r': return (mode & S_IRUSR) ? 1 : 0;
//         case 'w': return (mode & S_IWUSR) ? 1 : 0;
//         case 'x': return (mode & S_IXUSR) ? 1 : 0;
//         default:  return -1;
//     }
// }

// //функция, осуществляющая копирование информации, при использовании ссылок
// void copy_link(const char* file_from, const char* file_to) {
//     char* buf = (char*) calloc(BUF_SIZE, sizeof(char));

//     if (readlink(file_from, buf, BUF_SIZE) < 0) {
//         fprintf(stderr, "Failed to read link");
//         perror("Failed to read link");
//         free(buf);
//         return;
//     }

//     if (symlink(buf, file_to) < 0) {
//         fprintf(stderr, "Failed to create a  new link");
//         perror("Failed to create a  new link");
//         free(buf);
//         return;
//     }
  
//     //успешно!
//     return;
// }

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