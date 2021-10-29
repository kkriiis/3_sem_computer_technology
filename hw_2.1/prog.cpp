#include <stdio.h>
#include <stdin.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s filename text-to-writte\n", argv[0]);
		return RESULT_BAD_ARG;
	}

	int f = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (!f) {
		perror("Failed write to file for writing\n");

