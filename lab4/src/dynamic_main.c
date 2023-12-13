#include <stdio.h> 
#include <stdlib.h> 
#include <dlfcn.h>

int contr = 1;

const char* first_name = "prog1_s"; 
const char* second_name = "prog2_s";


void (*Sort)(int* arr, int n) = NULL;
float (*SinIntegral)(float A, float B, float e) = NULL;

void* lib_handle = NULL;

void load_lib() { 
    const char* name;

    if(contr == 1){
        name = first_name; 
    }
        
    else if(contr == 2){
        name = second_name;
    }
    
    lib_handle = dlopen(name, RTLD_LAZY); 

    if (lib_handle == NULL) {
        perror("cannot find library");
        exit(EXIT_FAILURE); 
    }
}

void load_contract() {
    load_lib(); 
    Sort = dlsym(lib_handle, "Sort");
    SinIntegral = dlsym(lib_handle, "SinIntegral"); 
}

void change_contract() { 
    dlclose(lib_handle); 

    if(contr == 1){
        contr = 2;
    }
    else if (contr == 2){ 
        contr = 1; 
    }
        
    load_contract(); 
}

int main() {
    load_contract();
    int command = 0;

    printf("Select the number of command:\n0) Change contract\n1) Call 'float SinIntegral(float A, float B, float e)'\n2) Call 'void Sort(int* arr, int n)'\n");
    do{
        command = getchar();
        if(command == '0'){
            change_contract();
            printf("Contract has been changed\n"); 
            if(contr == 1) {
                printf("Contract is first\n");
            }

            else if(contr == 2){ 
                printf("Contract is second\n");
            } 
        }
        else if(command == '1'){
            float A, B, e;
            printf("Type values of A, B and e (with space between): ");
            if(scanf("%f %f %f", &A, &B, &e)){
                // printf("\nA = %f, B = %f, e = %f\n", A, B, e);
                printf("\nResult is %lf\n", SinIntegral(A, B, e));
            }
            else printf("\nSomething went wrong, try again\n");
        }
        else if (command == '2'){
            int n = 0;
            printf("Type array size: "); 
            if(scanf("%d", &n)){
                int arr[n];
                int correct = 1;
                printf("\nType your array: ");
                for(int i = 0; i < n; i++){
                    int tmp;
                    if(scanf("%d", &tmp)){
                        arr[i] = tmp;
                    }else{
                        correct = 0;
                        break;
                    }
                }
                if(correct){
                    Sort(arr, n);
                    printf("\nHere is your sorted array: ");
                    for(int i = 0; i < n; ++i){
                        printf("%d ", arr[i]);
                    }
                    printf("\n");
                }
                else printf("\nSomething went wrong, try again\n");

            }else{
                printf("\nWrong, try again\n");
            }
        }else if(!(command == ' ' || command == '\n' || command == EOF)) printf("Wrong command\nTry again\n");
    }while (command != EOF);
    return 0; 
}
