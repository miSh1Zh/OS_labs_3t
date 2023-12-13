#include "lib.h"

int main(){
    int c;
    printf("Select the number of command:\n1) Call 'float SinIntegral(float A, float B, float e)'\n2) Call 'void Sort(int* arr, int n)'\n");
    do{
        c = getchar();
        if(c =='1'){
            float A, B, e;
            printf("Type values of A, B and e (with space between): ");
            if(scanf("%f %f %f", &A, &B, &e)){
                // printf("\nA = %f, B = %f, e = %f\n", A, B, e);
                printf("\nResult is %lf\n", SinIntegral(A, B, e));
            }
            else printf("\nSomething went wrong, try again\n");
        }
        else if(c == '2'){
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
        }
        else if(!(c == ' ' || c == '\n' || c == EOF)){
            printf("Wrong command\nTry again\n");
        }
    }while(c != EOF);
    return 0;
}
