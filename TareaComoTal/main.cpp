#include <bits/stdc++.h>

using namespace std;

int main(){
    pid_t pid;
    pid = fork();
    if(pid > 0){ // padre 
        execl("./observador", "", NULL);
    }
    else if(pid == 0){ // hijo
        execl("./jugador", "", NULL);
    }
    else{
        cout << "Error" << endl;
    }






    return 0;
}