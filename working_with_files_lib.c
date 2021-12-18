#include <stdint.h> // uint8_t
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <assert.h>
#include <utime.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "working_with_files_lib.h"

//функция преобразования времени в строку
char* get_UTC_time(char* str, const time_t* s_time) {
	//для хранения времени создаем структуру
    struct tm *time;

    //считываем системное время и преобразуем системное время в UTC
	time = gmtime(s_time);

	return (strftime(str, BUF_SIZE, "%x %A %X (UTC)", time) > 0) ? str : NULL;
}

//функция, определяющая тип файла(regular, dir, symlink etc)
const char* print_file_tipe(unsigned mode) {

        //маска S_IFMT путем побитового AND позволяет считывать только необходимые биты поля st_mode
	   switch (mode & S_IFMT) {
          	 case S_IFBLK:  return "block dwvice\n";
           	 case S_IFCHR:  return "character device\n";
           	 case S_IFDIR:  return "directory\n";
          	 case S_IFIFO:  return "FIFO/pipe\n";
          	 case S_IFLNK:  return "symlink\n";
          	 case S_IFREG:  return "regular file\n";
           	 case S_IFSOCK: return "socket\n";
	   }
	   return "unknown?\n";
}

//функция, записывающая права доступа к файлу в buf
char* print_access_rights(unsigned mode) {
	char *buf;
	buf = (char*)malloc(sizeof("rrrwwwxxx")*sizeof(char));
	buf[0] = mode & S_IRUSR ? 'r' : '-';
        buf[1] = mode & S_IWUSR ? 'w' : '-';
        buf[2] = mode & S_IXUSR ? 'x' : '-';
        buf[3] = mode & S_IRGRP ? 'r' : '-';
        buf[4] = mode & S_IWGRP ? 'w' : '-';
        buf[5] = mode & S_IXGRP ? 'x' : '-';
        buf[6] = mode & S_IROTH ? 'r' : '-';
        buf[7] = mode & S_IWOTH ? 'w' : '-';
        buf[8] = mode & S_IXOTH ? 'x' : '-';
	return buf;
}

//функция, удаляющая файл
void remove_file(const char* filename) {

    if (unlink(filename) == -1) {
        fprintf(stderr, "It's not possible to remove this file");
        perror("It's not possible to remove this file");
        return;
    }
    printf("[+] Successful removing: %s", filename);
    return;
}

//функция для чтения в буфер
ssize_t writeall(int fd, const void *buf, size_t count) {
    size_t bytes_written = 0;
    const uint8_t *buf_addr = (const uint8_t*) buf;

    while (bytes_written < count) {
        ssize_t res = write(fd, buf_addr + bytes_written, count - bytes_written);

        if (res < 0) {
            return res;
        }

        bytes_written += (size_t) res;
    }
    return (ssize_t) bytes_written;
}

//функция, проверяющая права пользователя
int check_user_access(const char* file_from, const char access, mode_t mode) {
    switch (access) {
        case 'r': return (mode & S_IRUSR) ? 1 : 0;
        case 'w': return (mode & S_IWUSR) ? 1 : 0;
        case 'x': return (mode & S_IXUSR) ? 1 : 0;
        default:  return -1;
    }
}

//функция, осуществляющая копирование информации, при использовании обычных файлов
void copy_file(unsigned from, unsigned to, const char* file_to, struct stat *sb) {
    
    //создаем буфер для чтения
    char *buf = (char*)calloc(BUF_SIZE, sizeof(char));
    assert(buf != NULL);
    
    long long copy_file_size = (long long) sb->st_size;
    
    //копирование
    while(copy_file_size > 0){
        ssize_t read_file = read(from, buf, BUF_SIZE);
        if (read_file < 0) {
            fprintf(stderr, "Failed read from file");
            perror("Failed read from file");
            remove_file(file_to);
            close(from);
            return;
        }

        ssize_t write_file = writeall(to, buf, read_file);
        if (write_file < 0) {
            fprintf(stderr, "Failed write to file");
            perror("Failed write to file");
            remove_file(file_to);
            close(to);
            return;
        }

        if (write_file != read_file) {
            fprintf(stderr, "The number of symbols written does't match the number of symbols read");
            perror("The number of symbols written does't match the number of symbols read");
            remove_file(file_to);
            close(to);
            return;
        }

        copy_file_size -= read_file;
    }

    free(buf);
    return;
}

//функция, осуществляющая копирование информации, при использовании ссылок
void copy_link(const char* file_from, const char* file_to) {
    char* buf = (char*) calloc(BUF_SIZE, sizeof(char));

    if (readlink(file_from, buf, BUF_SIZE) < 0) {
        fprintf(stderr, "Failed to read link");
        perror("Failed to read link");
        free(buf);
        return;
    }

    if (symlink(buf, file_to) < 0) {
        fprintf(stderr, "Failed to create a  new link");
        perror("Failed to create a  new link");
        free(buf);
        return;
    }
  
    //успешно!
    return;
}

char dtype_char(unsigned char dtype) {

    switch (dtype) {
        case DT_BLK :     return 'b'; break; 
        case DT_CHR:      return 'c'; break;
        case DT_DIR:      return 'd'; break;
        case DT_FIFO:     return 'f'; break;
        case DT_LNK:      return 'l'; break;
        case DT_REG:      return 'r'; break;
        case DT_SOCK:     return 's'; break;
        case DT_UNKNOWN:  return '?'; break;
    }

    return '?';
}

char file_mode(unsigned mode) {

    switch (mode & S_IFMT) {
        case S_IFBLK:  return 'b';   break;
        case S_IFCHR:  return 'c';   break;
        case S_IFDIR:  return 'd';   break;
        case S_IFIFO:  return 'f';   break;
        case S_IFLNK:  return 'l';   break;
        case S_IFREG:  return 'r';   break;
        case S_IFSOCK: return 's';   break;
        default:       return '?';   break;
    }

    return '?';
}

//функция, осуществляющая копирование директорий
int copy_dir(int cp_cp, int cp, const char* copy_file_name) {
    DIR *dir = fdopendir(cp);
    struct dirent *entry;

    //читаем записи копируемого каталога и копируем только обычные файлы
    while ((entry = readdir(dir)) != NULL) {
        struct stat sb;
        assert((fstatat(cp, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW)) == 0);

        //проверяем тип файла
        if ((sb.st_mode & S_IFMT) != S_IFREG) {
            fprintf(stderr, "File   %s   has type [%c], can't copy it now", entry->d_name, file_mode(sb.st_mode));
            perror("Can't copy file with this type");
        }
        else {
            //окрываем копируемый файл
            int file_from = openat(cp, entry->d_name, O_RDONLY);
            if (file_from < 0) {
                fprintf(stderr, "Failed for open copy file or this file can't be read");
                perror("Can't open file");
                remove_file(copy_file_name);
                return 0;
            }

            //открываем файл-копию в каталоге /base_dir/cp_dir
            int file_to = openat(cp_cp, entry->d_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file_to < 0) {
                fprintf(stderr, "Failed for open destination file");
                perror("Can't open file");
                remove_file(copy_file_name);
                return 0;
            }

            //проверяем право на чтение файла
            if (check_user_access(entry->d_name, 'r', sb.st_mode) != 1) {
                fprintf(stderr, "This file can't be read\n");
                perror("Can't be read");
                return 0;
            }

            //само копирование
            copy_file(file_from, file_to, entry->d_name, &sb);

            //закрываем файлы
            close(file_from);
            close(file_to);
        }
    }
    closedir(dir);
    return 1;
}