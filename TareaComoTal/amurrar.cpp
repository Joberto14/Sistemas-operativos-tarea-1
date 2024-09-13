#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[]) {
    
    int N = getpid();
    cout  << "Soy el proceso: " << N << ", me amurre con exec" << endl;

    exit(0);


    return 0;
}