#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <wait.h>

const char *fifo_path = "./fifo"; // Ruta del FIFO

void sender(int id) {
    // Abrir el FIFO solo para escritura
    int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        std::cerr << "Error al abrir el FIFO para escritura." << std::endl;
        exit(1);
    }

    // Crear el mensaje a enviar
    std::string message = "Mensaje del proceso " + std::to_string(id) + "\n";
    
    // Escribir el mensaje en el FIFO
    write(fd, message.c_str(), message.size());
    
    // Cerrar el descriptor del FIFO
    close(fd);
    exit(0); // Finaliza el proceso hijo
}

void receiver() {
    // Abrir el FIFO solo para lectura
    int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error al abrir el FIFO para lectura." << std::endl;
        exit(1);
    }

    char buffer[128];
    int bytesRead;

    // Leer todos los mensajes hasta que no haya más datos
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate el buffer
        std::cout << "Receptor recibió: " << buffer;
    }

    // Cerrar el descriptor del FIFO después de la lectura
    close(fd);
}

int main() {
    // Crear el FIFO (named pipe)
    if (mkfifo(fifo_path, 0666) == -1) {
        std::cerr << "Error al crear el FIFO." << std::endl;
        return 1;
    }

    // Array para almacenar los PIDs de los hijos
    pid_t child_pids[3];

    // Crear 3 procesos hijos secuenciales
    for (int i = 0; i < 3; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << "Error al crear el proceso hijo." << std::endl;
            return 1;
        } else if (pid == 0) {
            // Código del proceso hijo
            sender(i + 1); // Los IDs serán 1, 2 y 3
        } else {
            // Guardar el PID del hijo
            child_pids[i] = pid;
            
            // Esperar a que el hijo termine antes de continuar al siguiente
            waitpid(child_pids[i], nullptr, 0);
        }
    }

    // Cerrar el FIFO en el proceso principal para que no bloquee el receptor
    int fd = open(fifo_path, O_WRONLY);
    close(fd);

    // Código del proceso receptor
    receiver();

    // Eliminar el FIFO
    unlink(fifo_path);

    return 0;
}
