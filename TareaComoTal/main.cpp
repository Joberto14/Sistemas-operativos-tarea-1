#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/stat.h>
#include <cstring>

using namespace std;

int main( int argc, char *argv[] ) {
    system("g++ -o jugador jugador.cpp");
    int N = atoi(argv[1]);
    // arreglo de jugadores en memoria compartida
    key_t key = 69420;  // Clave para la memoria compartida de los jugadores

    // Crear un segmento de memoria compartida de tamaño N * sizeof(int)
    int shmid = shmget(key, N * sizeof(int), 0666 | IPC_CREAT);

    if (shmid == -1) {
        std::cerr << "Error al crear el arreglo en memoria compartida\n";
        exit(1);
    }

    int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));
    if (jugadores == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida de los jugadores\n";
        exit(1);
    }

    // arreglo de eliminados en memoria compartida
    key_t keyelm = 666;  // Clave para la memoria compartida de los eliminados

    // Crear un segmento de memoria compartida de tamaño N * sizeof(int)
    int shmidelim = shmget(keyelm, (N-1) * sizeof(int), 0666 | IPC_CREAT);

    if (shmidelim == -1) {
        std::cerr << "Error al crear el arreglo en memoria compartida de los eliminados\n";
        exit(1);
    }

    int* eliminados = static_cast<int*>(shmat(shmidelim, nullptr, 0));
    if (eliminados == (void*)-1) {
        std::cerr << "Error al adjuntar la memoria compartida\n";
        exit(1);
    }

    memset(eliminados, 0, (N-1) * sizeof(int));

    string observador = "g++ -o observador observador.cpp && ./observador " + to_string(N) + " &"; 

    system(observador.c_str()); // ejecuta observador independientemente
    
    
    
    cout << "Bienvenido al juego de la silla!" << endl;
    cout << "Se han unido " << N << " jugadores." << endl;
    


    for(int i = 0; i < N; i++){
        if(pid_t pid = fork(); pid > 0){
            jugadores[i] = pid;
            cout << "Jugador " << jugadores[i] << " ha entrado al juego." << endl;
        }
        else if(pid == 0){
            string aux = to_string(N);
            
            execl("./jugador", "jugador", aux.c_str(), NULL);
            perror("Error en execl");  // Manejar error si execl falla
            exit(1);
        }
        else{
            cout << "Error al crear proceso hijo" << endl;
            exit(1);
        }
    }

    system("g++ -o amurrar amurrar.cpp");

    
    if (shmdt(jugadores) == -1) {
        perror("Error al desadjuntar la memoria compartida");
        exit(1);
    }

    return 0;
}
