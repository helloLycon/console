/**
 * @brief   A method of printing with several levels(refering to `printk`) and logging messages into log-file.
 *
 * @author  lycon
 * @blog    https://lk361115629.github.io/
 * @github  https://github.com/lk361115629
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/file.h>
#include "console.h"



static void console_init(const char * log_file, const char * level_file);
static int console_print(const char *fmt, ...);
static int console_log(const char *file, int line,const char *fmt, ...);
static const char * console_time(char *timestr);



Console console = {
    .init  = console_init,
    .print = console_print,
    .log   = console_log,
    .time  = console_time
};


static void console_init(const char * log_file, const char * level_file) {
    console.log_file = log_file;
    console.level_file = level_file;
}

static int console_read_level_file(const char * level_file){
    FILE *fp;
    char level;
    int retv = LEVEL_DEFAULT;

    if((!console.level_file) || (!(fp = fopen(level_file, "r"))) ){
        return LEVEL_DEFAULT;
    }
    if( (fread(&level, 1, 1, fp) > 0) && isdigit(level)){
        retv = level - '0';
    }
    fclose(fp);
    return retv;
}

static int console_read_level_fmt(const char *fmt, const char ** new_fmt){
    if('<' == fmt[0] && isdigit(fmt[1]) && '>' == fmt[2]){
        *new_fmt = fmt + 3;
        return fmt[1] - '0';
    }
    *new_fmt = fmt;
    return LEVEL_DEFAULT;
}

static int console_print(const char *fmt, ...){
    const char * new_fmt;
    int retv = 0;
    va_list ap;
    va_start(ap, fmt);

    if( console_read_level_fmt(fmt,&new_fmt) 
        <= console_read_level_file(console.level_file))
    {
        retv = vprintf(new_fmt, ap);
    }
    
    va_end(ap);
    return retv;
}

static const char * console_time(char *timestr){
    time_t now = time(NULL);

    strftime(timestr, 1024, 
             "%y-%m-%d,%H:%M:%S",
             localtime(&now));
    return timestr;
}

static int console_log(const char *file, int line, const char *fmt, ...){
    FILE *fp;
    int retv;
    char timestr[256];
    va_list ap;
    va_start(ap, fmt);

    retv = printf("[LOG] ") + vprintf(fmt, ap);
    if(!console.log_file){
        return retv;
    }
    fp = fopen(console.log_file, "a");
    if(fp){
        flock(fileno(fp), LOCK_EX);
        fseek(fp, 0, SEEK_END);

        fprintf(fp, "[%s][%s:%d] ", console_time(timestr),file,line);
        vfprintf(fp, fmt, ap);

        flock(fileno(fp), LOCK_UN);
        fclose(fp);
    }
    
    va_end(ap);
    return retv;
}


