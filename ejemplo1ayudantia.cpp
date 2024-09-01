#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>  

int main() {
    const char *fifo_path = "./my_fifo";
    int fd;
    int signal;

    pid_t p = fork();

    mkfifo(fifo_path, 0666);

    fd = open(fifo_path, O_WRONLY);


    printf("Seleccione una opción: \n1. uwu\n2. owo\n");
    scanf("%d", &signal);

    write(fd, &signal, sizeof(signal));

    close(fd);

    unlink(fifo_path);
    if(p > 0){
        wait(NULL);
    }
    return 0;
}