
// Эти функции определены, но не объявлены. Вызывая их компилятор C не ругается, а генерирует объявления по умолчанию,
// с возвращаемым значением типа int и какими-то аргументами. В результате получаются ошибки выполнения.
// Без этих объявлений bison не работает с Visual C++ 2013.
// Закомментированные функции тоже не объявлены. Но без их объявлений bison работает.
// По крайней мере корректно собирает всё нужное для Tomita Parser.

char *strndup(char const *s, size_t n);
char *mkdtemp(char *xtemplate);
char *stpcpy(char *dest, const char *src);

void *rawmemchr(const void *s, int c_in);

//off_t ftello(FILE *fp);

//int fseeko(FILE *fp, off_t offset, int whence);
//int fpurge(FILE *fp);
//int wcwidth(wchar_t wc);
int mkstemp(char *xtemplate);

//int obstack_printf(struct obstack *obs, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
//int vasprintf(char **resultp, const char *format, va_list args);
