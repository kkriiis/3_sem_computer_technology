#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//функция, которая выводит информацию 
void system_file_info(const char *fs_name, struct statvfs *sbfs) {
    printf("Usage: %s\n\n", fs_name);

    printf("Filesystem block size:                          %lu\n", sbfs->f_bsize);
    printf("Fragment size:                                  %lu\n", sbfs->f_frsize);
    printf("Size of filesystem in fragment size units:      %lu\n", sbfs->f_blocks);
    printf("Number of free blocks:                          %lu\n", sbfs->f_bfree);
    printf("Number of free blocks for unprivileged users:   %lu\n", sbfs->f_bavail);
    printf("Number of inodes:                               %lu\n", sbfs->f_files);
    printf("Number of free inodes:                          %lu\n", sbfs->f_ffree);
    printf("Number of free inodes for unprivileged users:   %lu\n", sbfs->f_favail);
    printf("Filesystem ID:                                  %lu\n", sbfs->f_fsid);
    printf("Mount flags:                                    %lu\n", sbfs->f_flag);
    printf("Maximum filename length:                        %lu\n", sbfs->f_namemax);
}

 int main(int argc, char *argv[]) {
     struct statvfs sbfs;
     struct stat sb;

     char *file_name = ".";
     char *fs_name;

     if (argc < 2) {
         fprintf(stderr, "Expected more argunemts. For example: ./a.out  file_system_name  file_name\n");
         return 0;
     }
     else if (argc == 2) {
         file_name = argv[1];
     }
     else {
         fs_name = argv[1];
         file_name = argv[2];
     } 
     
     if (lstat(fs_name, &sb) < 0) {
         fprintf(stderr, "Failed in lstat with %s", fs_name);
         return 0;
     }

     //проверяем fs_name. Если его тип не директория, то выдаем ошибку
     if ((sb.st_mode & S_IFMT) != S_IFDIR) {
         fprintf(stderr, "%s is not file system", fs_name);
         return 0;
     }

     if (statvfs(fs_name, &sbfs) == -1) {
         fprintf(stderr, "Failed in statvfs with %s", fs_name);
         return 0;
     }

     if (strlen(file_name) != 0) {
         if (lstat(file_name, &sb) < 0) {
             fprintf(stderr, "Failed in lstat with %s", file_name);
             return 0;
         }
     }
     else {
         return 0;
     }

     system_file_info(fs_name, &sbfs);

     if ((sbfs.f_favail < 1) || ((long int) (sbfs.f_bavail * sbfs.f_bsize) < sb.st_size)) {
        fprintf(stderr, "\nThe file %s doesn't fit to %s\n", file_name, fs_name);
     }
     else {
       printf("\nThe file %s fits to %s\n", file_name, fs_name);
     }
     return 1;
 }