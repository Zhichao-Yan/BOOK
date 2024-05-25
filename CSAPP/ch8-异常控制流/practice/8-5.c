#include <unistd.h>

unsigned int snooze(unsigned int secs)
{
    unsigned int left = sleep(secs);
    printf("Slept for %u of %u secs\n",secs - left,secs);
    return left;
}