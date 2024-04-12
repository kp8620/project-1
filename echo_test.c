#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
        fflush(stdout);
        sleep(1); // Sleep for 1 second between arguments
    }
    return 0;
}
