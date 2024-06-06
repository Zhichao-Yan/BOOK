#ifndef HTTP_H
#define HTTP_H

typedef struct header{
    char name[50];
    char value[100];
}header;

typedef struct{                  
    unsigned long header_count; // header count
    header* list; // header linklist
}header_list;

#endif