#ifndef HTTP_H
#define HTTP_H

typedef struct{
    char name[50];
    char value[100];
}header;

typedef struct{                  
    unsigned long size;     // header list size
    unsigned long count;    // header count
    header* list; // header list
}header_list;

#endif