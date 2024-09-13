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
#include <fcntl.h>

using namespace std;

volatile sig_atomic_t signal_received = 0;

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    printf("Soy el observador\n");

    // int hay_jugadores= 1;
    int cantidad_jugadores = N;
    while(cantidad_jugadores > 1){

        key_t key = 69420;  // Clave para la memoria compartida de jugadores

        int shmid = shmget(key, N * sizeof(int), 0666 );

        int* jugadores = static_cast<int*>(shmat(shmid, nullptr, 0));
        if (jugadores == (void*)-1) {
            std::cerr << "Error al adjuntar la memoria compartida\n";
            exit(1);
        }

        // arreglo de eliminados en memoria compartida
        key_t keyelm = 666;  // Clave para la memoria compartida de los eliminados

        // Crear un segmento de memoria compartida de tamaño N * sizeof(int)
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

        srand (time(NULL)+ getpid());

        int tiempo_de_juego = (rand() % 10) + 1;  // observador 

        cout << "La musica sono por: " << tiempo_de_juego << " segundos" << endl;

        for (int i = 0; i < N; i++)
        {
            if(cantidad_jugadores > 1) kill(jugadores[i], SIGUSR1);
        }

        sleep(5);
        map<int, int> freq;
        
        map<int, int> para_no_leer_eliminados;
        for (int i = 0; i < N-1; i++)
        {
            if(eliminados[i] != 0){
                para_no_leer_eliminados[eliminados[i]] = 1;
                
            } 
        }
        for (int i = 0; i < N; i++){
            const string fifo_path = "/tmp/my_fifo" + to_string(jugadores[i]);
            int fd;
            if(para_no_leer_eliminados[jugadores[i]]){
                continue;
            }
            fd = open(fifo_path.c_str(), O_RDONLY);
            int signal;
            if (fd == -1){
                perror("Error al abrir el fifo para leer");
                exit(1);
            }    
            read(fd, &signal, sizeof(signal));

            // create a map to store the frequency of each signal
            freq[signal]++;

            close(fd);

            unlink(fifo_path.c_str());
        }
        // find the signal with the highest frequency
        int max_freq = 0;
        int max_signal = 0;
        for (auto it = freq.begin(); it != freq.end(); it++){
            if (it->second > max_freq){
                max_freq = it->second;
                max_signal = it->first;
            }
        }
        cout << "El jugador " << max_signal << " se va amurrar :(" << endl;
        
        pid_t pid = max_signal;

        kill(pid, SIGUSR2);

        for (int i = 0; i < N-1; i++){
            if(eliminados[i] == 0){
                eliminados[i] = pid;
                break;
            }
        }

        for (int i = 0; i < N-1; i++){
            cout << "Eliminados: " << eliminados[i] << endl;
        }
        
        // for (int i = 0; i < N-1; i++){
        //     if(eliminados[i] == 0){
        //         hay_jugadores = 1;
        //     }
        //     else{
        //         hay_jugadores = 0;
        //     }
        // }
        

        cantidad_jugadores--;

        if (shmdt(jugadores) == -1) {
            perror("Error al desadjuntar la memoria compartida");
            exit(1);
        }
    }


    exit(0);
    return 0;
}
