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

                if (letras[i] == 'E')
                {
                    // Hijo de E, I e hijo de I, J
                    for (int x = 8; x < 10; x++)
                    {
                        id = fork();
                        if (id != 0)
                        {
                            break;
                        }else{
                            printf("%c Soy: %d. Mi padre es: %d\n", letras[x], getpid(), getppid());
                        }                            
                    }
                    if(id > 0) {
                        wait(NULL);
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