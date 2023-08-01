# Watchdog Project

The Watchdog Project is a C89 library that offers a robust mechanism for monitoring the liveliness of a program and automatically restarting it if it becomes unresponsive. The client program can link to the shared object called `libwatchdog.so` and utilize two essential functions, `MakeMeImmortal` and `DoNotResuscitate`, to ensure critical code execution is safeguarded.

## Description

The Watchdog Project is designed to provide a safety net for critical code segments within a program. By setting a watchdog, the client program can periodically check for signs of life and take action if the monitored code becomes unresponsive. If the watchdog detects a certain number of consecutive misses, it will automatically restart the program, providing enhanced reliability.

## Directory Structure

Before using the Watchdog Project, ensure you have the following directory structure in your project:

    src
    |- sorted_list.c
    |- dlist.c
    |- p_queue.c
    |- task.c
    |- uid.c
    |- scheduler.c

    include
    |- dlist.h
    |- p_queue.h
    |- scheduler.h
    |- sorted_list.h
    |- task.h
    |- uid.h
    |- utilities.h
    |- watchdog.h

    test
    |- wd_test.c

    makefile

## Building the Watchdog Client

To build the Watchdog client program (`wd_test.out`), utilize the provided `makefile`:

    make

The makefile will compile the source files and link them with the libwatchdog.so shared object.
Usage

To incorporate the Watchdog functionality into your client program, you need to do the following:

    1.Include Header File: Add #include "watchdog.h" to your client program's source file.

    2. Link the Shared Object: In the makefile, the client program (wd_test.out) is linked with libwatchdog.so using the -L and -lwatchdog flags. Make sure the shared object (libwatchdog.so) is present in your project directory.

    3. Extern Functions: In your client program, extern the following functions to access the Watchdog functionality:
        int MakeMeImmortal(int argc, char *argv[], size_t interval, size_t max_misses);
        int DoNotResuscitate(void);

    4. Use the Watchdog: Surround the critical code segments that you want to protect with MakeMeImmortal() calls. This will set up the watchdog to monitor these code sections.

    5. Deactivate Watchdog: When the critical section is complete, call DoNotResuscitate() to disable the watchdog for that portion of the program.

## Example

    #include <stdio.h>
    #include "watchdog.h"
    
    int main(int argc, char *argv[]) 
    {
        // Initialize watchdog to check every 5 seconds and allow 3 misses
        MakeMeImmortal(argc, argv, 5, 3);
    
        // Critical code segment to be safeguarded
        printf("Executing critical code...\n");
    
        // Deactivate watchdog after the critical section is done
        DoNotResuscitate();
    
        // Continue with the rest of the program
        printf("Program execution continues...\n");
    
        return 0;
    }

## Valgrind for Memory Leak Detection

You can run the client program with Valgrind for memory leak detection using the following command:

    make vlg

## Cleaning Up

To clean up the generated files (executable and object files), use the make clean target:

    make clean

## Contributing

Contributions to the Watchdog Project are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## Contact Information

For any inquiries or support, you can contact the author at:

    Email: aviv.jilin2@gmail.com

Enjoy utilizing the Watchdog Project to ensure the reliability of your critical code! 🚀
