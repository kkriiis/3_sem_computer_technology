//BUF_SIZE - размер буфера для форматного вывода в консоль (четверть блока)
#define BUF_SIZE 256

char* get_UTC_time(char* str, const time_t* s_time);

const char* print_file_tipe(unsigned mode);

char* print_access_rights(unsigned mode);

void remove_file(const char* filename);

ssize_t writeall(int fd, const void *buf, size_t count);

int check_user_access(const char* file_from, const char access, mode_t mode);

void copy_file(unsigned from, unsigned to, const char* file_to, struct stat *sb);

void copy_link(const char* file_from, const char* file_to);

char dtype_char(unsigned char dtype);

char file_mode(unsigned mode);

int copy_dir(int cp_cp, int cp, const char* copy_file_name);