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
    //int fd_virt = shm_open(MEMORY_NAME, O_EXCL | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    int fd_virt = shm_open(
        MEMORY_NAME, 
        O_CREAT | O_RDWR, 
        S_IRUSR | S_IWUSR);
    if(fd_virt == -1){
        perror("\n(child)shm_open: here is a problem\n");
        if(shm_unlink(MEMORY_NAME) == -1){
            perror("(child)munmap: Here is a problem");
            _exit(EXIT_FAILURE);
        }
        close(fd_virt);
        _exit(EXIT_FAILURE);
    }
    struct stat statbuf;
    if (fstat(STDIN_FILENO, &statbuf) < 0 ){
        perror("\n(child)Can't get file size\n");
        if(shm_unlink(MEMORY_NAME) == -1){
            perror("(child)munmap: Here is a problem");
            _exit(EXIT_FAILURE);
        }
        close(fd_virt);
        _exit(EXIT_FAILURE);
    }
    if (ftruncate(fd_virt, statbuf.st_size) == -1) {
        perror("\n(child)ftruncate: here is a problem\n");
        if(shm_unlink(MEMORY_NAME) == -1){
            perror("(child)munmap: Here is a problem");
            _exit(EXIT_FAILURE);
        }
        close(fd_virt);
        _exit(EXIT_FAILURE);
    }
    int* virtual = (int*)mmap(
        NULL, 
        statbuf.st_size, 
        PROT_WRITE | PROT_READ, 
        MAP_SHARED, 
        fd_virt, 
        0);
    if(virtual == MAP_FAILED){
        perror("\n(child)mmap: here is a problem\n");
        if(shm_unlink(MEMORY_NAME) == -1){
            perror("(child)munmap: Here is a problem");
            _exit(EXIT_FAILURE);
        }
        close(fd_virt);
        _exit(EXIT_FAILURE);
    }

    int c = '\0';
    int tmp = 0, res = 0;
    int end_of_str = 0;
    char* to_res = "\0";

    int count = 0;

    do{
        //c = getchar(); 
        if(!end_of_str){
            if(c>='0' && c<='9'){
                tmp = tmp*10 + c - '0';
            }
            if(c == ' ' || c == '\n' || c == EOF){
                if(res == 0 && tmp != 0){
                    res = tmp;
                }
                else if(res != 0 && tmp != 0){
                    res /= tmp;
                }
                else if(res == 0 && tmp == 0){
                    end_of_str = 1;
                }
                else if(res != 0 && tmp == 0){
                    //fprintf(stderr, "Division by zero");
                    //printf("%d\n", -1);
                    res = -1;
                    // write(STDOUT_FILENO, &res, sizeof(int));
                    virtual[count] = res;
                    _exit(EXIT_FAILURE);
                }
                tmp = 0;
            }
        }
        if(c == '\n' || c == EOF){
            // printf("%d\n", res);
            
            // write(STDOUT_FILENO, &res, sizeof(int));
            virtual[count] = res;
            count++;
            end_of_str = 0;
            res = 0;
        }
    //}while(c != EOF);
    }while(read(STDIN_FILENO, &c, sizeof(char)) > 0);

    res = -2; // end of work
    
    // write(STDOUT_FILENO, &res, sizeof(int)); 
    virtual[count] = res;   

    return 0;
}
