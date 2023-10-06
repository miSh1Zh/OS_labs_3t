#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(){
    int c = '\0';
    int tmp = 0, res = 0;
    int end_of_str = 0;

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
                    write(STDOUT_FILENO, &res, sizeof(int));
                    _exit(EXIT_FAILURE);
                }
                tmp = 0;
            }
        }
        if(c == '\n' || c == EOF){
            //printf("%d\n", res);
            write(STDOUT_FILENO, &res, sizeof(int));
            end_of_str = 0;
            res = 0;
        }
    //}while(c != EOF);
    }while(read(STDIN_FILENO, &c, sizeof(char)) > 0);

    return 0;
}
