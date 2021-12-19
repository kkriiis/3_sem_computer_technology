#компилятор
C = gcc
CPP = g++

#флаги компиляции
C_FLAGS=-Werror -Wall -Wextra -Wnarrowing
#флаги оптимизации
C_OPTIMIZE_FLAGS=-O2 -flto -fomit-frame-pointer

SRC_PATH_TASK_1 = hw_1/stat.c
SRC_PATH_TASK_2_1 = hw_2.1/write.c
SRC_PATH_TASK_2_2 = hw_2.2/dprintf.c
SRC_PATH_TASK_3 = hw_3/copy.c
SRC_PATH_TASK_6_1 = hw_6.1/dirent.c
SRC_PATH_TASK_7_1 = hw_7.1/copydir.c
SRC_PATH_TASK_9 = hw_9/statvfs.c

all: hw_1 hw_2.1 hw_2.2 hw_3 hw_6.1 hw_7.1 hw_9

hw_1:   ${SRC_PATH_TASK_1} working_with_files_lib.c 
	$(C) -o out ${SRC_PATH_TASK_1} working_with_files_lib.c
hw_2.1: ${SRC_PATH_TASK_2_1} working_with_files_lib.c
	$(C) -o out ${SRC_PATH_TASK_2_1} working_with_files_lib.c
hw_2.2: ${SRC_PATH_TASK_2_2} working_with_files_lib.c
	$(C) -o out ${SRC_PATH_TASK_2_2} working_with_files_lib.c
hw_3:   ${SRC_PATH_TASK_3} working_with_files_lib.c 
	$(C) -o out ${SRC_PATH_TASK_3} working_with_files_lib.c 
hw_6.1: ${SRC_PATH_TASK_6_1} working_with_files_lib.c 
	$(C) -o out ${SRC_PATH_TASK_6_1} working_with_files_lib.c 
hw_7.1: ${SRC_PATH_TASK_7_1} working_with_files_lib.c 
	$(C) -o out ${SRC_PATH_TASK_7_1} working_with_files_lib.c 
hw_9: ${SRC_PATH_TASK_9}
	$(C) -o out ${SRC_PATH_TASK_9} 


clear:
	rm out