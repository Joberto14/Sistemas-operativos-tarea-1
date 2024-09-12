#include <bits/stdc++.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <ctime> 
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

volatile sig_atomic_t signal_received = 0;

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    printf("Soy el observador\n");
    
    key_t key = 69420;  // Clave para la memoria compartida

    int shmid = shmget(key, N * sizeof(int), 0666 );
    int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));
    if (jugadores == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }

    srand (time(NULL)+ getpid());

    int tiempo_de_juego = (rand() % 10) + 1;  // observador 

    cout << "La musica sono por: " << tiempo_de_juego << " segundos" << endl;

    for (int i = 0; i < N; i++)
    {
        kill(jugadores[i], SIGUSR1);
    }
    

    exit(0);
    return 0;
}
