#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>

#include "../working_with_files_lib.h"

//
 //внесено в библиотеку
// //BUF_SIZE - buffer size for formatted output to the console (quarter block)
// //(In Russia ) //BUF_SIZE - размер буфера для форматного вывода в консоль (четверть блока)
// unsigned int BUF_SIZE = 256;

// //time to string conversion function
// //(In Russia ) //функция преобразования времени в строку
// char* get_UTC_time(char* str, const time_t* s_time) {
// 	struct tm *time;
// 	time = gmtime(s_time);

// 	return (strftime(str, BUF_SIZE, "%x %A %X (UTC)", time) > 0) ? str : NULL;
// }


//функция внесена в библиотеку
// //функция, определяющая тип файла(regular, dir, symlink etc)
// const char* print_file_tipe(unsigned mode) {

//         //маска S_IFMT путем побитового AND позволяет считывать только необходимые биты поля st_mode
// 	   switch (mode & S_IFMT) {
//           	 case S_IFBLK:  return "block dwvice\n";
//            	 case S_IFCHR:  return "character device\n";
//            	 case S_IFDIR:  return "directory\n";
//           	 case S_IFIFO:  return "FIFO/pipe\n";
//           	 case S_IFLNK:  return "symlink\n";
//           	 case S_IFREG:  return "regular file\n";
//            	 case S_IFSOCK: return "socket\n";
// 	   }
// 	   return "unknown?\n";
// }

//внесено в библиотеку
//функция, записывающая права доступа к файлу в buf
// char* print_access_rights(unsigned mode) {
// 	char *buf;
// 	buf = (char*)malloc(sizeof("rrrwwwxxx")*sizeof(char));
// 	buf[0] = mode & S_IRUSR ? 'r' : '-';
//         buf[1] = mode & S_IWUSR ? 'w' : '-';
//         buf[2] = mode & S_IXUSR ? 'x' : '-';
//         buf[3] = mode & S_IRGRP ? 'r' : '-';
//         buf[4] = mode & S_IWGRP ? 'w' : '-';
//         buf[5] = mode & S_IXGRP ? 'x' : '-';
//         buf[6] = mode & S_IROTH ? 'r' : '-';
//         buf[7] = mode & S_IWOTH ? 'w' : '-';
//         buf[8] = mode & S_IXOTH ? 'x' : '-';
// 	return buf;
// }

int main(int argc, char *argv[])
       {
           struct stat sb;

           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (lstat(argv[1], &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
           }

	   //ID consists of two parts, functions major() and minor() return these values
           //(In Russia ) //ID из двух частей, функции major() и minor(),  возвращают эти значения
           printf("ID of containing device:  [%lx,%lx]\n",                (long) major(sb.st_dev), (long) minor(sb.st_dev)); 

           printf("File type: %s               ", print_file_tipe(sb.st_mode));

	   printf("%s\n", print_access_rights(sb.st_mode)); 

           printf("I-node number:            %ld\n", (long) sb.st_ino);

           printf("Mode:                     %lo (octal)\n",
                   (unsigned long) sb.st_mode);

           printf("Link count:               %ld\n", (long) sb.st_nlink);
           printf("Ownership:                UID=%ld   GID=%ld\n",
                   (long) sb.st_uid, (long) sb.st_gid);

           printf("Preferred I/O block size: %ld bytes\n",
                   (long) sb.st_blksize);
           printf("File size:                %lld bytes\n",
                   (long long) sb.st_size);
           printf("Blocks allocated:         %lld\n",
                   (long long) sb.st_blocks);

	   char *str_time = (char*)calloc(BUF_SIZE, sizeof(char));

           printf("Last status change:       %s\n", get_UTC_time(str_time, &sb.st_ctime));
           printf("Last file access:         %s\n", get_UTC_time(str_time, &sb.st_atime));
           printf("Last file modification:   %s\n", get_UTC_time(str_time, &sb.st_mtime));

	   free(str_time);

           return EXIT_SUCCESS;
       }


