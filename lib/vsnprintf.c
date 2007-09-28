/*
 * vsnprintf()
 *
 * Poor substitute for a real vsnprintf() function for systems
 * that don't have them...
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "nasmlib.h"

extern efunc nasm_malloc_error;

#define BUFFER_SIZE	65536	/* Bigger than any string we might print... */

static char snprintf_buffer[BUFFER_SIZE];

int vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int rv, bytes;

    if (size > BUFFER_SIZE) {
	nasm_malloc_error(ERR_PANIC|ERR_NOFILE,
			  "snprintf: size (%d) > BUFFER_SIZE (%d)",
			  size, BUFFER_SIZE);
	size = BUFFER_SIZE;
    }

    rv = vsprintf(snprintf_buffer, format, ap);
    if (rv > BUFFER_SIZE) {
	nasm_malloc_error(ERR_PANIC|ERR_NOFILE,
			  "snprintf buffer overflow");
    }

    if (rv < (int)size-1)
	bytes = rv;
    else
	bytes = size-1;

    if (size > 0) {
	memcpy(str, snprintf_buffer, bytes);
	str[bytes] = '\0';
    }

    return rv;
}
