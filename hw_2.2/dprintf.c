#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "../working_with_files_lib.h"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s filename text-to-writte\n", argv[0]);
		return 0;
	}

	//открываем файл, для пользователя чтение и печать, для остальных только чтение
	int f = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	//проверка открытого файла
	if (!f) {
		perror("Failed write to file for writing\n");
		return 0;
	}

	//запись в файл и проверка записи на ошибки
	if (!dprintf(f, "%s", argv[2])) {
		perror("Failed write to file");
		close(f);
		return 0;
	}

	//закрываем файл
	if (!close(f)) {
		perror("Failed close file");
		return 0;
	}

	return 1;
}
