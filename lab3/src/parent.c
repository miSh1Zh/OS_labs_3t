#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define MEMORY_NAME "/1"

int main(){

    char* filename = malloc(sizeof(char));
    struct stat statbuf;

    if(!scanf("%s", filename)){
        perror("\nScan file problem!\n");
        _exit(EXIT_FAILURE);
    }

    int fd = 0;
    fd = open(filename, O_RDONLY);
    if(fd == -1){
        perror("\nCan't open file\n");
        _exit(EXIT_FAILURE);
    }

    if ( fstat(fd, &statbuf) < 0 ){
        perror("\nCan't get file size\n");
        _exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("\nfork: Here is a problem\n");
        _exit(EXIT_FAILURE);
    }
    else if(pid == 0){ //child
        dup2(fd, STDIN_FILENO);
        char* args[] = {"./child", NULL};
        if (execv(args[0], args) == -1){
            fprintf(stderr, "Unable to exec\n");
            _exit(EXIT_FAILURE);
        }
    }else{ //parent
        wait(0);
        int fd_virt = shm_open(
            MEMORY_NAME, 
            O_RDONLY, 
            S_IRUSR | S_IWUSR);
        if(fd_virt == -1){
            perror("\nshm_open: here is a problem\n");
            _exit(EXIT_FAILURE);
        }
        int* virtual = (int*)mmap(
            NULL, 
            statbuf.st_size, 
            PROT_READ, MAP_SHARED, 
            fd_virt, 
            0);
        if(virtual == MAP_FAILED){
            perror("\nmmap: here is a problem\n");
            _exit(EXIT_FAILURE);
        }
        int result = 0;
        for(int i = 0; i < statbuf.st_size; i++){
            if(virtual[i] == -1){
                printf("Division by zero\n");
                if(munmap(virtual, statbuf.st_size) == -1){
                    perror("munmap: Here is a problem");
                    _exit(EXIT_FAILURE);
                }
                if(shm_unlink(MEMORY_NAME) == -1){
                    perror("munmap: Here is a problem");
                    _exit(EXIT_FAILURE);
                }
                close(fd_virt);
                _exit(EXIT_FAILURE);
            }
            if(virtual[i] == -2){
                break;
            }
            printf("%d\n", virtual[i]);
        }
        if(munmap(virtual, statbuf.st_size) == -1){
            perror("munmap: Here is a problem");
            _exit(EXIT_FAILURE);
        }
        if(shm_unlink(MEMORY_NAME) == -1){
            perror("munmap: Here is a problem");
            _exit(EXIT_FAILURE);
        }
        close(fd_virt);
    }
    return 0;
}
