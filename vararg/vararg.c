// file name : vararg.c
// description : A demo of a function with variable args
// created at: Thu 12 Dec 2013 02:48:21 PM CST

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../dbg//dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer){
        // calloc(数目，每个的大小)
        *out_string = calloc(1, max_buffer + 1);
        check_mem(*out_string);

        char *result = fgets(*out_string, max_buffer, stdin);
        check(result != NULL, "Input error.");

        return 0;

error:
        if(*out_string) free(*out_string);
        *out_string = NULL;
        return -1;
}

int read_int(int *out_int){
        char *input = NULL;
        int rc = read_string(&input, MAX_DATA);
        check(rc == 0,"Failed to read number.");

        *out_int = atoi(input);

        free(input);
        return 0;
error:
        if(input) free(input);
        return -1;
}

int read_scan(const char *fmt, ...){
        //                     ^^^ 表明参数数目为任意的，位置在 fmt 之后。
        int i = 0;
        int rc = 0;
        int *out_int = NULL;
        char *out_char = NULL;
        char **out_string = NULL;
        int max_buffer = 0;

        va_list argp;
        va_start(argp, fmt); // initialize

        for(i = 0; fmt[i] != '\0'; i++){ // 遍历 fmt
                if(fmt[i] == '%'){
                        i++; // 看 % 后面的字符是什么，它表示类型
                        switch(fmt[i]){
                        case '\0':
                                sentinel("Invalid format, you ended with %%.");
                                break;

                        case 'd':
                                out_int = va_arg(argp, int*);
                                rc = read_int(out_int);
                                check(rc == 0, "Failed to read int.");
                                break;

                        case 'c':
                                out_char = va_arg(argp, char *);
                                *out_char = fgetc(stdin);
                                break;

                        case 's':
                                // macro va_arg(argp, TYPE)
                                max_buffer = va_arg(argp, int);
                                out_string = va_arg(argp, char**);
                                rc = read_string(out_string, max_buffer);
                                check(rc == 0, "Failed to read string");
                                break;
                        default:
                                sentinel("Invalid format.");
                        }
                } else {
                        // eat up remaining input.
                        fgetc(stdin);
                }
                // feof() 用来侦测是否读取到了文件尾, 如果已到文件尾则返回非零值, 其他情况返回 0.
                check(!feof(stdin) && !ferror(stdin), "Input error.");
        }

        va_end(argp);
        return 0;

error:
        va_end(argp);
        return -1;
}

int main(){
        char *first_name = NULL;
        char initial = ' ';
        char *last_name = NULL;
        int age = 0;

        printf("What's your first name?");
        int rc = read_scan("%s", MAX_DATA, &first_name);
        check(rc == 0, "Failed first name.");

        printf("What's your initial?");
        rc = read_scan("%c\n", &initial);
        check(rc == 0, "Failed initial.");

        printf("What's your last name?");
        rc = read_scan("%s", MAX_DATA, &last_name);
        check(rc == 0, "Failed last name.");

        printf("How old are you?");
        rc = read_scan("%d", &age); // 之前将类型写成了 %s,导致max_buffer 都为负。
        check(rc == 0, "Failed age.");

        printf("---Result--\n");
        printf("First Name: %s", first_name);
        printf("Initial: %c\n", initial);
        printf("Last Name: %s", last_name);
        printf("Age: %d\n", age);

        free(first_name);
        free(last_name);
        return 0;

error:
        return -1;
}
