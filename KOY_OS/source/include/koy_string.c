#include "koy_string.h"

void *koy_memcpy(char* dest, const char* src, size_t nb) {
    char temp_dest[nb];
    for (size_t i = 0; i < nb; i++) {
        temp_dest[i] = src[i];
    }
    for (size_t i = 0; i < nb; i++) {
        dest[i] = temp_dest[i];
    }
}

void *koy_memmove(char* dest, const char* src, size_t nb) {
    if (dest < src) {
        for (size_t i = 0; i < nb; i++) {
            dest[i] = src[i];
        }
    } else {
        for (size_t i = nb; i > 0; i--) {
            dest[i - 1] = src[i - 1];
        }
    }
    return dest;
}

void *koy_memset(void* start, int chr, size_t end) {
    unsigned char* ptr = (unsigned char*) start;
    for (size_t i = 0; i < end; i++) {
        ptr[i] = (unsigned char) chr;
    }
    return start;
}

int koy_memcmp(const void* ptr1, const void* ptr2, size_t nb) {
    const unsigned char* p1 = ptr1;
    const unsigned char* p2 = ptr2;

    for (size_t i = 0; i < nb; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

void *koy_memchr(const void* ptr, int value, size_t nb) {
    const unsigned char* p = ptr;
    for (size_t i = 0; i < nb; i++) {
       if (p[i] == (unsigned char)value) {
        return (void *) &p[i];
       }
    }
    return NULL;
}

char *koy_strcpy(char* dest, const char* src) {
    size_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char *koy_strncpy(char* dest, const char* src, size_t nb) {
    size_t i = 0;
    while (i < nb && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    while (i < nb) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

char *koy_strcat(char* dest, const char* src) {
    size_t dest_len = 0;
    while (dest[dest_len] != '\0') {
        dest_len++;
    }
    size_t i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
    return dest;
}

char *koy_strncat(char* dest, const char* src, size_t nb) {
    size_t dest_len = 0;
    while (dest[dest_len] != '\0') {
        dest_len++;
    }
    size_t i = 0;
    while (i < nb && src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
    return dest;
}

int koy_strcmp(const char* str1, const char* str2) {
    size_t i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] == str2[i]) {
            i++;
        } else {
            break;
        }
    }
    return (unsigned char)str1[i] - (unsigned char)str2[i];
}

int koy_strncmp(const char* str1, const char* str2, size_t nb) {
    size_t i =0;
    while (i < nb) {
        unsigned char c1 = (unsigned char)str1[i];
        unsigned char c2 = (unsigned char)str2[i];
        if (c1 != c2) {
            return c1 - c2;
        }
        if (c1 == '\0') {
            return 0;
        }
        i++;
    }
    return 0;
}

size_t koy_strlen(const char* str) {
    size_t i = 0;
    while (*str != '\0') {
        str++;
        i++;
    }
    return i;
}