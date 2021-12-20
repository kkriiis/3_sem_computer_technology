#include <linux/limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h> // flock
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define N_LENGTH sizeof("1000000000")

int main(int argc, char *argv[]) {

    int f = open("counter.txt", O_RDWR | O_CREAT, 0666);
    if (f < 0) {
        fprintf(stderr, "Failed in open counter.txt");
        return 0;
    }

    char buf[N_LENGTH];
    memset(buf, 0, sizeof(buf));
    int counter = 1;

    //блокируем чтение и запись файла
    flock(f, F_LOCK);

    long int bytes_read = read(f, buf, N_LENGTH);
    if (bytes_read < 0) {
        fprintf(stderr, "Failed in read");
        flock(f, F_LOCK);
        close(f);
        return 0;
    }

    sleep(5);

    char number[bytes_read > 0 ? (bytes_read +1) : 2];
    memset(number, 0, sizeof(number));

    counter += atoi(buf);

    printf("Program was executed %d times\n", counter);
    sprintf(number, "%d\n", counter);

    if (pwrite(f, number, sizeof(number), 0) < 0) {
        fprintf(stderr, "Failed to write");
        flock(f, F_LOCK);
        close(f);
        return 0;
    }

    //разблокируем и закроем файл 
    flock(f, F_LOCK);
    close(f);
    return 1;
}