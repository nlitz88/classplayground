#ifndef HELPER_INCLUDED
#define HELPER_INCLUDED

typedef struct CACHE_LINE {
    struct CACHE_LINE *prev;
    struct CACHE_LINE *next;
    char data[1024];
} CACHE_LINE;

#endif