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
volatile sig_atomic_t amurrar = 0;

void signal_handler(int signum) {
    signal_received = 1;
}

void amurrar_handler(int signum) {
    std::cout << "Soy el proceso: " << getpid() << " me amurre" << endl;
    execl("./amurrar","amurrar", NULL);
    perror("Error al ejecutar el proceso amurrar");
    exit(1);
}

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    
    key_t key = ftok("/tmp", 'J');  // Clave para la memoria compartida
    
    // Segmento de memoria compartida de jugadores
    int shmid = shmget(key, N * sizeof(int), 0666 );

    int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));

    if (jugadores == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }
    
    key_t keyelm = ftok("/tmp", 'B');  // Clave para la memoria compartida de los eliminados

    // Crear un segmento de memoria compartida de tamaño (N-1) * sizeof(int)
    int shmidelim = shmget(keyelm, (N-1) * sizeof(int), 0666);

    if (shmidelim == -1) {
        std::cerr << "Error al crear el arreglo en memoria compartida de los eliminados\n";
        exit(1);
    }

    int* eliminados = static_cast<int*>(shmat(shmidelim, nullptr, 0));
    if (eliminados == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }

    
    const string fifo_path = "/tmp/my_fifo" + to_string(getpid());
    int fd;
    
    //int hay_jugadores= 1;
    int cantidad_jugadores = N;
    //signal(SIGUSR2, amurrar_handler);

    while(cantidad_jugadores > 1){

        // for (int i = 0; i < N-1; i++){
        //     if(eliminados[i] == 0){
        //         hay_jugadores = 1;
        //     }
        //     else{
        //         hay_jugadores = 0;
        //     }
        // }

        map<int, int> para_no_votar_eliminados;
        for (int i = 0; i < N-1; i++)
        {
            if(eliminados[i] != 0){
                para_no_votar_eliminados[eliminados[i]] = 1;
                
            } 
        }
        
        
        srand (time(NULL)+ getpid());

        // printf("Bienvenido al juego de la silla!\n");


    
        signal(SIGUSR1, signal_handler);

        while (!signal_received);

        signal_received = 0;
    
    
        int voto = rand() % N;
        while(para_no_votar_eliminados[jugadores[voto]]) voto = rand() % N;
        
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
        

        signal(SIGUSR2, amurrar_handler);
        
        pause();

        // for (int i = 0; i < N-1; i++){
        //     if(eliminados[i] == 0){
        //         hay_jugadores = 1;
        //     }
        //     else{
        //         hay_jugadores = 0;
        //     }
        // }
        cantidad_jugadores--;
    }

    //signal(SIGUSR2, amurrar_handler);

    if (shmdt(jugadores) == -1) {
        perror("Error al desadjuntar la memoria compartida");
        exit(1);
    }

    close(fd);

    unlink(fifo_path.c_str());

    std::cout << "Se ha acabado el juego de la silla! Soy el proceso:" << getpid() << " y gane!!!! "<< endl;

    exit(0);
    return 0;
}