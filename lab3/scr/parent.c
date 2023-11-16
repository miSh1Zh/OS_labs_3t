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

int main(){

    char* filename = malloc(sizeof(char));
    struct stat statbuf;

    if(!scanf("%s", filename)){
        perror("\nScan file problem!\n");
        _exit(EXIT_FAILURE);
    }

    int fd = open(filename, O_RDONLY);
    if(fd == -1){
        perror("\nCan't open file\n");
        _exit(EXIT_FAILURE);
    }

    if ( fstat(fd, &statbuf) < 0 ){
        perror("\nCan't get file size\n");
        _exit(EXIT_FAILURE);
    }

    int res_fd;
    if ((res_fd = open("proc.txt", O_RDWR| O_CREAT, 0777)) == -1)
    {
        perror("\nError: can not open a file for mmapping.\n");
        exit(EXIT_FAILURE);
    }
    int* virtual_file;
    if((virtual_file = (int*)mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, res_fd, 0)) == MAP_FAILED){
        perror("\nCan't make mapped file\n");
        _exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("\nfork: Here is a problem\n");
        _exit(EXIT_FAILURE);
    }
    else if(pid == 0){ //child
        dup2(fd, STDIN_FILENO);
        dup2(res_fd, STDOUT_FILENO);
        char* args[] = {"./child", NULL};
        if (execv(args[0], args) == -1){
            fprintf(stderr, "Unable to exec\n");
            _exit(EXIT_FAILURE);
        }
    }else{ //parent
        wait(0);
        int result = 0;
        for(int i = 0; i < statbuf.st_size; i++){
            if(virtual_file[i] == -1){
                printf("Division by zero\n");
                _exit(EXIT_FAILURE);
            }
            if(virtual_file[i] == -2){
                break;
            }
            printf("%d\n", virtual_file[i]);
        }
    }

    if(munmap(virtual_file, statbuf.st_size) == -1){
        perror("munmap: Here is a problem");
        _exit(EXIT_FAILURE);
    }
    close(res_fd);
    remove("proc.txt");
    return 0;
}
