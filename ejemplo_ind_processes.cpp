#include <iostream>
#include <cstdlib>  // Para system()

int main() {
    // Ejecutar dos comandos independientes
    std::cout << "Procesos lanzados independientemente.\n";
    
    if (system("ls -l &") < 0) {  // Primer proceso independiente
        std::cerr << "Error al ejecutar el primer comando\n";
    }

    if (system("echo 'ola' &") < 0) {  // Segundo proceso independiente
        std::cerr << "Error al ejecutar el segundo comando\n";
    }

    return 0;
}
