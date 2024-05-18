#include "csapp.h"

int main() {
    int fd; 
    char c;
    fd = Open("foobar.txt", O_RDONLY,0);
    if(Fork() == 0) 
    {
        Read(fd, &c, 1);
        printf ("children proccess : c = %c\n", c);
        exit(0);
    }
    Wait (NULL);
    Read (fd, &c, 1);
    printf ("father process: c = %c\n", c);
    exit (0);
}
// children proccess : c = f
// father process: c = o