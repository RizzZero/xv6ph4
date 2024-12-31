#include "types.h" 
#include "user.h"
#include "stat.h"
#include "fcntl.h"

int
main(int argc, char* argv[]){
    int fd = open("textFile.txt",O_CREATE|O_WRONLY);
    for (int i = 0 ; i < 5  ; i++){
        int pid = fork();
        if(pid == 0){
            volatile long long int temp = 0 ;
            while((open("anotherFile.txt",O_CREATE|O_WRONLY)) < 0){
                temp++ ;
            }
            write(fd,"hello there!",13);
            unlink("anotherFile.txt");
            exit();
        }
    }
    while(wait() != -1) ;
    close(fd);
    countSyscallsCount();
    exit();
}