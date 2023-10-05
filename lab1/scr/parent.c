#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

int end_input(int c){
    return (c == EOF || c == '\n');
}

void get_filename(char* name){
    int c = '\0', len = 0;
    do{
        c = getchar();
        
        if(c != ' '){
            len++;
            name = realloc(name, len*sizeof(char));
            name[len-1] = c;
        }
        
    }while(!end_input(c));
}

int main(int argc, char* argv[]){

    if (argc != 2) {
        perror("\nError: no filename\n");
        exit(EXIT_FAILURE);
    }


    /* //I tried
    char* filename = malloc(sizeof(char));
    printf("Type filename: ");
    get_filename(filename);
    
    if(strcmp(filename, "") == 0){
        perror("\nNo file was given!\n");
        _exit(EXIT_FAILURE);
    }
    */
    //printf("\nfilename: %s\n", filename);  

    int fd = open(argv[1], O_RDONLY);
    //int fd = open(filename, O_RDONLY);
    if(fd == -1){
        perror("\nCan't open file\n");
        _exit(EXIT_FAILURE);
    }
    
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1){
        perror("\npipe: Here is a problem\n");
        _exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("\nfork: Here is a problem\n");
        _exit(EXIT_FAILURE);
    }else{
        if(pid == 0){ //child
            close (pipe_fd[0]);
            dup2(fd, STDIN_FILENO);
            dup2(pipe_fd[1], STDERR_FILENO);
            char* args[] = {"./child", NULL};
            if (execv(args[0], args) == -1) {
                fprintf(stderr, "Unable to exec\n");
                _exit(EXIT_FAILURE);
            }
        }else{ //parent
            close(pipe_fd[1]);
            wait(0);
            char ch_status[17];
            while(read(pipe_fd[0], ch_status, 1)){
                if(strcmp(ch_status, "") != 0){
                    printf("\nDivision by zero\n");
                    _exit(EXIT_FAILURE);
                }
            }
        }
    }

    return 0;
}
