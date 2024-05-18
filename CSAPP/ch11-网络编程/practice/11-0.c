#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    // 1 is low bytes, 0 is high bytes
    int a = 1; // 4 bytes: 0x00000001
    // uint32_t a = 0x00ffff01;
    // a = htonl(a);
    char c = *((char*)(&a)); // read one byte from the lowest addresss of a
    if(c == 1)
        printf("small end\n");
    else
        printf("big end\n");
}