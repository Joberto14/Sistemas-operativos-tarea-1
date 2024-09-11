#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h> 

using namespace std;

int main( int argc, char *argv[] ) {
    system("g++ -o jugador jugador.cpp");
    int N = atoi(argv[1]);
    
    key_t key = 69420;  // Clave para la memoria compartida

    // Crear un segmento de memoria compartida de tamaño N * sizeof(int)
    int shmid = shmget(key, N * sizeof(int), 0666 | IPC_CREAT);

    if (shmid == -1) {
        std::cerr << "Error al crear la memoria compartida\n";
        exit(1);
    }

    int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));
    if (jugadores == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }



    // system("g++ -o observador observador.cpp && ./observador &"); // ejecuta observador independientemente
    
    pid_t pid = 1;
    
    cout << "Bienvenido al juego de la silla!" << endl;
    cout << "Se han unido " << N << " jugadores." << endl;
    


    for(int i = 0; i < N+1; i++){
        if(pid > 0){
            pid = fork();
            jugadores[i] = pid;
        }
        else if(pid == 0){
            string aux = to_string(N);
            execlp("./jugador", "./jugador", aux, NULL);
        }
        else{
            cout << "Error al crear proceso hijo" << endl;
        }
    }

    


    return 0;
}
