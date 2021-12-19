#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <string.h>

#include "../working_with_files_lib.h"

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


