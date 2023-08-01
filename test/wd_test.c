#include "watchdog.h"
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>


extern int MakeMeImmortal(int argc, char *argv[], size_t interval, size_t max_misses);

extern int DoNotResuscitate(void);

int main(int argc, char *argv[])
{
    MakeMeImmortal(argc, argv, 3, 5);
    
    puts("START TO BE IMMORTAL");
    
    while(1)
    {
        sleep(1);
    }    
    
    DoNotResuscitate();

    return 0;
}
