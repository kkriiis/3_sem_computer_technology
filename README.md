# 3_sem_computer_technology
## Task 1 
`stat`

Реализация пользовательского аналога команды stat.

- **stat** выводит содержимое полей дескриптора файла или статус файловой системы  
- **lstat** идентична **stat**, но в случае символьных сылок она возвращает информацию о самой ссылке, а не о файле, на который она указывает.
- **fstat** идентична **stat**, только возвращается информация об открытом файле, на который указывает filedes (возвращаемый open(2)), а не о file_name.

Программа принимает на вход имя файла и выводит информацию о нем
```
./a.out <filename>
```

## Task 2.1 
`write`

Программа принимает на вход имя файла и строку, которую нужно записать в файл
```
./a.out <file name> <str>
```

## Task 2.2
`dprintf`

**dprintf** напрваляет введённые данные в заданный файл.

Программа принимает на вход имя файла и строку, которую нужно записать в файл
```
./a.out <file name> <str>
```

## Task 3
`copy file`

Программа прининимает на вход имя файла, откуда нужно скопировть (file_from), и имя файла, куда нужно будет скопировать данные (file_to)
```
./a.out <file_from> <file_to>
```

## Task 6.1
`dirent.h`

 - **dirfd** - возвращает файловый дескриптор, связанный с потоком каталога.
 - **opendir** - открывает поток каталога, с соответствующим именем, и возвращает указатель на этот поток. Устанавливается на первой хаписи в каталоге.
 - **readdir** - вохвращает указатель на следующую запись каталога в структуре *dirent*, прочитанную из потока каталога. Функция возвращает NULL при достижении последней записи или при ошибке.
 - **assert** - прекращает работу программы при ложном утверждении.

 Программа выводит в консоль список всех файлов, тип и права доступа этих файлов. Также программа выводит скрытые файлы, родительский и текущифй каталоги

 ```
 ./a.out
 ```

## Task 7.1
 `copy dir`

 - **openat** - идентично **open** кроме того, что аргумент пути интерпретируется от начальной точки, подразумеваемой первым аргументом (int fildes), если этот аргумент имеет специальное значение AT_FDCW, то аргумент относительного пути (const char *path) будет разрешен относительно текущего рабочего каталога, если же аргемнт пути будет абсолютным, то  аргумент (int fildes) игнорируется.
 Программа копирует директорию *dir-from* в *dir-to*
 - **mkdirat** - работает точно так же, как mkdir, за исключением следующего: если путь относительный(второй аргумент), то он интерпретируется относительно каталога, на который ссылается файловый дескриптор (int dirft)(первый аргумент), а не относительно рабочего каталога. Если имя пути относительно, а переменная файлового дескриптора имеет специальное значение AT_FDCWD, то имя пути интерпретируется относительно текущего рабочего каталога
 - **fstatat** - аналогичный функциям **stat()**, **lstat()**, **fstat()**. Есои аргумент пути(второй аргумент) является относительным, он разрешается относительно аргумента (int fildes)(первого аргумента), а не текущего каталога. Если путь абсолютный, то первый аргумент игнорируется. Если (int fildes) имеет специальное значение AT_FDCWD, то относительные пути определяются относительно текущего рабочего каталога. Если на (int flag)(четвертый аргумент) установлено AT_SYMLINK_NOFOLLOW, то функция ведет себя как **lstat()**  и не следует автоматически по символическим ссылкам

 Программа получает на вход путь к директории, относительно текущего рабочего каталога, и создает его копию
 ```
 ./a.out <path to dir>
 ```
 [!] По каким-то причинам при любом вводе выдает ошибку о невозможности открыть *dir-from*

 ## Task 9
 `get file system statistics `

 - **statvfs** - возвращает информацию о файловой системе. Первый аргумент (const char *path) - это путь к любому файлу в системе, а второй аргумент (struct statvfs *buf) - это указатель на структуру statvfs, которая содержит информацию о размерах блоков данных, свободных блоках, флагах монтирования и тд.
 - **statfs** - аналогично **statvfs**, только структура statfs несет в себе более полную информацию о памяти системы

 Программа получает на вход путь к файлу, который находится в запущенной системе, затем выводит на экран значение полей структуры statvfs

 ## Task 10
 `monitoring file system events`
 
 **inotify** - предоставляет механизм для мониторинга событий файловой системы. Его можно использовать для мониторинга отдельных файлов или каталогов, когда каталог отслеживается, inotify будет возвращать союытия для самого каталога и для файлов внутри

 ## Task 11
 `flock`

 **flock** - Устанавливает или снимает advisory (мягкую, носящую рекомендательный характер) блокировку на открытый файл, заданный файловым дескриптором fd.


 ## Task 18
 `интегрирование`

 Программа интегрирует функцию (2/sqrt(pi))*exp(-x*x) тремя способами: методом Симпсона, Ромберга и по правилу трапеций. Записывает значения получившейся после интегрирования функции в файлы. После этого мы строим график, чтобы убедиться в правильности этих способов (значения накладываются друг на друга, значит все хорошо)

 *жизнь слишком коротка, чтобы писать снова уже написанные программы*

 Эту программу я писала в прошлом семестре в курсе вычислительной физики. Тогда мы изучали библиотеки nr и nrutil (библиотеки для прикладных задач)

 Компиляция и запуск:

 ```
 g++ 2.cpp -o out
 ./out
 ```
