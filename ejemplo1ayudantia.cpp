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
    pid_t p;

    mkfifo(fifo_path, 0666);

    fd = open(fifo_path, O_WRONLY);
    
    for (int i = 0; i < 1; i++){
        if(p > 0){
            p = fork();
        }        
    }
    
    if(p > 0){
        wait(NULL);
        //sleep(500);
    }

    printf("Seleccione una opción: \n1. uwu\n2. owo\n");
    scanf("%d", &signal);

    write(fd, &signal, sizeof(signal));


    close(fd);

    unlink(fifo_path);
    return 0;
}