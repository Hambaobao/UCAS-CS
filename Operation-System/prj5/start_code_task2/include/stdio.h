#ifndef INCLUDE_STDIO_H_
#define INCLUDE_STDIO_H_

/* kernel printf */
int printk(const char *fmt, ...);
int my_printf(const char *fmt, ...);


/* user printk */
int printf(const char *fmt, ...);

// prj-4
void scanf(int *n);

#endif
