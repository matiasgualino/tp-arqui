
#include <stdarg.h>
#include <types.h>
#include <syscalls.h>
#include <libc.h>

void* malloc(int len) {

	return sys_malloc(len);

}

void* calloc(int len) {

	return sys_calloc(len);

}

void free(void* m) {
	sys_free(m);
}

int strpos(char* s, char n) {

	for (int pos = 0; s[pos] != 0; pos++) {
		if (s[pos] == n) {
			return pos;
		}
	}

	return -1;


}

static void vfprintf(FD fd, char* fmt, va_list ap) {

	char* str = calloc(MAX_PRINTF_LEN);
	int i = 0;
	int j = 0;

	// i: posicion en el fmt
	// j: posicion en el str
	while (fmt[i] != 0 && i < MAX_PRINTF_LEN - 1) {

		if (fmt[i] == '%') {

			bool flag_zero = FALSE;
			uint32_t width = 0;

			i++;
			if (fmt[i] == 0) {
				//lo que le sigue al % es el final de la cadena
				str[j] = fmt[i];
				break;

			} else if (fmt[i] == '%') {

				str[j] = fmt[i];
				j++;
				i++;

			} else {
				// hay que procesar el siguiente caracter y actuar acorde
				bool flag;
				do {

					flag = FALSE;
					switch (fmt[i]) {
					case 's': {
						//lo que se desea es imprimir unca cadena
						char* arg = va_arg(ap, char*);
						int k = 0;

						//k: posicion en el argumento

						while (arg[k] != 0) {
							str[j] = arg[k];
							j++;
							k++;
						}

						i++;
						break;
					}

					case 'i': {

						int arg = va_arg(ap, int);

						char* number = itoc(arg);

						int k = 0;

						int numlen = strlen(number);

						if (numlen < width) {

							char chartowrite;
							int numtowrite = width - numlen;

							if (flag_zero) {
								chartowrite = '0';

							} else {
								chartowrite = ' ';
							}

							for (int i = 0; i < numtowrite; i++) {
								str[j] = chartowrite;
								j++;
							}

						}

						//k: posicion en el argumento

						while (number[k] != 0) {
							str[j] = number[k];
							j++;
							k++;
						}

						i++;
						break;

					}

					case 'c': {

						char arg = (char)va_arg(ap, int);

						str[j] = arg;
						j++;
						i++;
						break;

					}
					case '0': {
						if (!flag_zero) {
							flag_zero = TRUE;
							i++;
							flag = TRUE;
							break;
						}
					}

					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9': {
						width = fmt[i] - '0';
						i++;
						flag = TRUE;
						break;
					}

					}

				} while (flag);

			}

		} else if (fmt[i] != 0) {
			str[j] = fmt[i];
			j++;
			i++;
		} else {
			str[j] = fmt[i];
			break;
		}

	}

	//TODO: sacar antes de entregar
	//va_end(ap);

	sys_write(fd, str, j);

}

void fprintf(FD fd, char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	vfprintf(fd, fmt, ap);

	va_end(ap);

}

void printf(char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	vfprintf(FD_STDOUT, fmt, ap);

	va_end(ap);

}

void putchar(char c) {

	sys_write(FD_STDOUT, &c, 1);
}

int strlen(char* str) {
	int size;
	for (size = 0; *str != '\0' ; str++) {
		size++;
	}

	return size;
}

int getchar() {

	static char buffer[2] = {0};

	char read = sys_read(FD_STDOUT, buffer, 1);

	if (read == EOF) {
		return EOF;
	}

	return (int) buffer[0];
}

int scanf(char* c, int len) {

	char read = sys_read(FD_STDOUT, c, len);

	return read;
}

int strcmp(const char* s1, const char* s2) {

	while (*s1 && *s1 == *s2) {
		s1++;
		s2++;
	}

	return *s1 - *s2;
}

char* strcpy(char* dest, char* src) {

	char* bk = dest;

	while (*src != 0) {
		*dest = *src;
		src++;
		dest++;
	}

	*dest = 0;

	return bk;
}

char* strcat(char* str1, char* str2){
	int len1=strlen(str1);
	int len2=strlen(str2);
	char* ans=calloc((len1+len2) * sizeof(char));
	strcpy(ans,str1);
	strcpy(ans+len1,str2);
	return ans;

}

time_t* time() {

	time_t* t = (time_t*)calloc(sizeof(time_t));

	sys_rtc_get(t);

	return t;

}

void setTime(time_t * t){
	sys_rtc_set(t);
}

color_t get_color() {
	return sys_get_color();
}

void set_color(vga_color fg, vga_color bg) {
	color_t t = BUILD_COLOR(fg, bg);

	sys_set_color(t);

}

char* itoc(int number) {

	int i = 0;
	int j = 0;
	int cnt = 0;

	char* c = malloc(10);

	if (number < 0) {
		number = -number;
		c[i++] = '-';
	}

	while (number >= 10 ) {
		int dig = number % 10;
		number /= 10;
		c[i++] = dig + '0';
		cnt++;
	}
	c[i] = number + '0';

	while (cnt >= j) {
		char aux;
		aux = c[cnt];
		c[cnt--] = c[j];
		c[j++] = aux;
	}

	return c;

}

bool stringNumeric(char* str){
	int len=strlen(str);
	for (int i = 0; i < len; i++)
	{
		if(!isNumeric(str[i])){
			return FALSE;
		}
	}
	return TRUE;
}

bool isNumeric(char c){
	return(c>='0' && c<='9');
}

int ctoi(char* c) {
	int ans = 0;
	int len = strlen(c);
	for (int i = 0; i < len; i++)
	{
		ans = ans * 10 + c[i] - '0';
	}
	return ans;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while (length--)
		dst[length] = chr;

	return destiation;
}

void clear_screen(){
	sys_clear_screen();
}

void exit(){
	sys_exit();
}
