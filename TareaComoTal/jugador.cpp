#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <ctime> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum) {
    signal_received = 1;
}

int main(int argc, char *argv[]) {

    const string fifo_path = "./my_fifo" + to_string(getpid());
    int fd;
    

    key_t key = 69420;  // Clave para la memoria compartida
    
    int N = atoi(argv[1]);
    
    srand (time(NULL)+ getpid());

    printf("Bienvenido al juego de la silla!\n");

    // int tiempo_de_juego = rand() % 10 + 1;  // observador 

    int shmid = shmget(key, N * sizeof(int), 0666 );
    int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));
    if (jugadores == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }

    signal(SIGUSR1, signal_handler);

    while (!signal_received);

    signal_received = 0;

    int voto = rand() % N;

    printf("Jugador %d ha votado por jugador %d\n", getpid(), jugadores[voto]);

    if (access(fifo_path.c_str(), F_OK) == -1) {
        // El archivo no existe, crear el FIFO
        if (mkfifo(fifo_path.c_str(), 0666) == -1) {
            perror("Error al crear el FIFO");
            exit(1);
        }
    }

    fd = open(fifo_path.c_str(), O_WRONLY);

    if (fd == -1){
        perror("Error al abrir el fifo para escribir");
        exit(1);
    }
    write(fd, &jugadores[voto], sizeof(jugadores[voto]));    

    if (shmdt(jugadores) == -1) {
        perror("Error al desadjuntar la memoria compartida");
        exit(1);
    }

    close(fd);

    unlink(fifo_path.c_str());

    exit(0);
    return 0;
}