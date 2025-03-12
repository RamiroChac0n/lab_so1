#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char letras[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

int main(){
    
    printf("%c Soy: %d\n", letras[0], getpid());

    pid_t id = fork();

    if (id == 0)
    {
        for (int i = 2; i < 5; i++)
        {
            id = fork();
            if(id != 0){
                printf("%c Soy: %d. Mi padre es: %d\n", letras[i], id, getpid());          
            }else{
                if (letras[i] == 'C')
                {

                    // Hijos DE C de la F a la H
                    for (int j = 5; j < 8; j++)
                    {
                        id = fork();
                        if (id != 0)
                        {
                            printf("%c Soy: %d. Mi padre es: %d\n", letras[j], id, getpid());
                        }else{
                            break;
                        }                            
                    }
                }
                break;
            }
        }   
    }else{
        printf("%c Soy: %d. Mi padre es: %d\n", letras[1], id, getpid());
    }
    

    return 0;
}