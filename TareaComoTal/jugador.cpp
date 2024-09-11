#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h> 
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

int main(int argc, char *argv[]) {
    key_t key = 69420;  // Clave para la memoria compartida
    
    int N = atoi(argv[1]);
    
    srand (time(NULL)+ getpid());

    printf("Bienvenido al juego de la silla!\n");

    // int tiempo_de_juego = rand() % 10 + 1;  // observador 

    printf("existo\n");
    int shmid = shmget(key, N * sizeof(int), 0666 );
    printf("existo\n");
    int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));
    if (jugadores == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }
    printf("existo\n");
    int voto = rand() % N;
    printf("existo\n");
    printf("Jugador %d ha votado por jugador %d\n", getpid(), jugadores[voto]);
    printf("existo\n");

    exit(0);
    return 0;
}