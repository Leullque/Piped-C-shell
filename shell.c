#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <readline/readline.h>

#define MAX_SIZE 20

char* files[1024];
int tot_file = 3;
char argv[4][MAX_SIZE];
int tot_argv = 0;


void readin(char* input){
    char *sect;
    sect = NULL;
    sect = strtok(input, " ");
    while(sect){
        strcpy(argv[tot_argv],sect);
        tot_argv++;
        sect = strtok(NULL," ");
    }
//    for(int k = 0; k < tot_argv; k++){
//        printf(argv[k]);
//        printf(" ");
//    }
}

void command_ls(){
    for(int i = 0; i < tot_file; i++){
        printf("%s\n",files[i]);
    }
}

int execute(int tot_argv, char* argv){
    char* output = NULL;
    output = argv;
    if(tot_argv == 1){
        if(strcmp(&argv[0], "exit") == 0){
            output = "exit";
            printf("%s\n",output);
            return 0;
        }
        else if(strcmp(&argv[0], "ls") == 0){
            command_ls();
            return 1;
        }
        else if(strcmp(&argv[0], "pwd") == 0){
            command_ls();
            return 1;
        }
        else if(strcmp(&argv[0], "ls") == 0){
            command_ls();
            return 1;
        }
    }
    return 1;
}



int main(){
    while(1){
        printf("mumsh $");//prompt waiting for the input
        unsigned long size = 1024;
        //char* input = (char*)malloc(size);
        char input[size];
        //fgets(input,sizeof(input),stdin);
        char ch;
        int i = 0;
        while(scanf("%c",&ch) && ch != '\n'){
            input[i++] = ch;
        }
        input[i] = '\0';
        readin(input);


        files[0] = "driver";
        files[1] = "mumsh";
        files[2] = "mumsh_memory_check";



        int exe = 0;
        exe = execute(tot_argv, *argv);
        for(int j = 0; j < 4; j++){
            strcpy(argv[tot_argv],"\0");
        }
        tot_argv = 0;
        if(exe == 0){
            return 0;
        }


        //free(input);
    }
    return 0;
}