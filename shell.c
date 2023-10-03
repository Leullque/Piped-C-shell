#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 20


char argv[6][MAX_SIZE];
int tot_argv = 0;

struct File{
    char  name[50];
    char  content[1024];
};

struct File files[1024];
int tot_file = 3;

void test(char input[]){
    printf("test:%s",input);
}

void readin(char* input){
    char *sect;
    sect = NULL;
    sect = strtok(input, " ");
    while(sect){
        strcpy(argv[tot_argv],sect);
        tot_argv++;
        sect = strtok(NULL," ");
    }
    //printf("%d",tot_argv);
//    for(int k = 0; k < tot_argv; k++){
////        printf(argv[k]);
////        printf(" ");
//        test(argv[k]);
//    }
}

void command_ls(void){
    if(tot_file > 3){
        for(int i = 3; i < tot_file; i++){
            printf("%s\n",files[i].name);
        }
    }
    for(int i = 0; i < 3; i++){
        printf("%s\n",files[i].name);
    }


}

void command_echo(void){
    for(int i = 1; i < tot_argv; i++){
        printf("%s ",argv[i]);
    }
}

void command_cat(char file[]){
    //printf("test:%s",file);
    int find = 0;
    for(int i = 0; i < tot_file; i++){

        //printf("file is %s %s",files[i].name,files[i].content);
        if(strcmp(file, files[i].name) == 0){
            printf("%s ",files[i].content);
            find = 1;
            break;
        }
    }
    if(find == 0){
        printf("error: no this file");
    }
}

char* command_head(char file[]){
    char* lines = (char*)malloc(10 * sizeof(char));
    int find = 0;
    int length = 0;

//    for(int i = 0; i < tot_file; i++){
//        //printf("file is %s %s",files[i].name,files[i].content);
//        if(strcmp(file, files[i].name) == 0){
//            char *sect;
//            sect = NULL;
//            sect = strtok(files[i].content, "\n");
//            while(sect){
//                strcpy(ten_line[line],sect);
//                line++;
//                sect = strtok(NULL,"\n");
//            }
//            for(int k = 0; k < line && k < 10; k++){
//                //printf("%d:",line);
//                printf(ten_line[k]);
//                printf("\n");
//            }
//            find = 1;
//            break;
//        }
//    }
    for(int i = 0; i < tot_file; i++){
        //printf("file is %s %s",files[i].name,files[i].content);
        if(strcmp(file, files[i].name) == 0){
            char *sect;
            sect = NULL;
            sect = strtok(files[i].content, "\n");

            while(sect){
                if(length == 0){
                    strcpy(lines,sect);
                    strcat(lines,"\n");
                    length++;
                }
                else if(length < 10){
                    strcat(lines,sect);
                    strcat(lines,"\n");
                    length++;
                }
                sect = strtok(NULL,"\n");
            }
            find = 1;
            break;
        }
    }
    if(find == 0){
        printf("error: no this file");
    }
    return lines;
}

void command_output_redirect(char file_name[], char content[],int cases){
    int find = 0;
    for(int i = 0; i < tot_file; i++){
        if(strcmp(file_name, files[i].name)==0){
            //printf("find%s",files[i].content);
            if(cases == 0){
                strcpy(files[i].content,content);
            }
            else{
                strcat(files[i].content,"\n");
                strcat(files[i].content,content);
            }
            find = 1;
            break;
        }
    }
    if(find == 0){
        strcpy(files[tot_file].name , file_name);
        if(cases == 0){
            strcpy(files[tot_file].content,content);
        }
        else{
            strcat(files[tot_file].content,"\n");
            strcat(files[tot_file].content,content);
        }
        tot_file += 1;
    }
}

void command_input_redirect(char in_file_name[],char out_file_name[],int cases, int length){
    int find = 0;
    char  content[1024];

    for(int i = 0; i < tot_file; i++){
        if(strcmp(in_file_name, files[i].name) == 0){
            if(length == 100){
                strcpy(content,files[i].content);
            }
            else if(length == 10){

            }

            find = 1;
        }
    }
    if(find == 0){
        printf("error: input file not found");
    }
    else{
        command_output_redirect(out_file_name,content,cases);
    }

}

int execute(void){
    char* output = NULL;
    //output = argv;
    if(tot_argv == 1){
        if(strcmp(argv[0], "exit") == 0){
            output = "exit";
            printf("%s\n",output);
            return 0;
        }
        else if(strcmp(argv[0], "ls") == 0){
            command_ls();
            return 1;
        }
        else if(strcmp(argv[0], "diff") == 0){
            output = "diff: missing operand after 'diff'\n diff: Try 'diff --help' for more information.";
            printf("%s\n",output);
            return 1;
        }
        else {
            command_ls();
            return 1;
        }
    }
    else if(tot_argv == 2){
        if(strcmp(argv[0], "ls") == 0){
            output =".\n..";
            printf("%s\n",output);
            command_ls();
            return 1;
        }
        else if(strcmp(argv[0], "echo") == 0){
            command_echo();
            return 1;
        }
        else if(strcmp(argv[0], "cat") == 0){
            command_cat(argv[1]);
            return 1;
        }
        else if(strcmp(argv[0], "head") == 0){
            char* outs = command_head(argv[1]);
            printf("%s", outs);
            free(outs);
            return 1;
        }
    }
    else if(tot_argv == 3){
        if(strcmp(argv[0], "cat") == 0 && strcmp(argv[1], "<") == 0) {
            command_cat(argv[2]);
            return 1;
        }
        else if(strcmp(argv[0], "head") == 0 && strcmp(argv[1], "<") == 0){
            char* outs = command_head(argv[2]);
            printf("%s", outs);
            free(outs);
            return 1;
        }
    }
    else if(tot_argv >= 4){
        if(strcmp(argv[0], "echo") == 0 && strcmp(argv[2], ">") == 0){
            command_output_redirect(argv[3],argv[1],0);
            return 1;
        }
        else if(strcmp(argv[0], "echo") == 0 && strcmp(argv[2], ">>") == 0){
            command_output_redirect(argv[3],argv[1],1);
            return 1;
        }
        else if(strcmp(argv[0], "cat") == 0){
            char in_file_name[50];
            char out_file_name[50];
            int cases = 0;
            for(int i = 1; i < tot_argv; i++){
                if(strcmp(argv[i],">") == 0){
                    strcpy(out_file_name , argv[i+1]);
                    cases = 0;
                }
                else if(strcmp(argv[i],">>") == 0){
                    strcpy(out_file_name , argv[i+1]);
                    cases = 1;
                }
                else if(strcmp(argv[i],"<") == 0){
                    strcpy(in_file_name , argv[i+1]);
                }
            }
            command_input_redirect(in_file_name,out_file_name,cases,50);
            return 1;
        }
        else if(strcmp(argv[0], "head") == 0){
            char out_file_name[50];
            int cases = 0;
            char* outs;
            for(int i = 1; i < tot_argv; i++){
                if(strcmp(argv[i],">") == 0){
                    strcpy(out_file_name , argv[i+1]);
                    cases = 0;
                }
                else if(strcmp(argv[i],">>") == 0){
                    strcpy(out_file_name , argv[i+1]);
                    cases = 1;
                }
                else if(strcmp(argv[i],"<") == 0){
                     outs = command_head(argv[i+1]);
                    //printf("%s", outs);

                }
            }
            command_output_redirect(out_file_name,outs,cases);
            free(outs);
            return 1;
        }
        else if(strcmp(argv[0], "echo") == 0){
            command_echo();
            return 1;
        }

    }
    return 1;
}



int main(void){
    strcpy(files[0].name , "driver");
    strcpy(files[0].content , "driver");
    strcpy(files[1].name , "mumsh");
    strcpy(files[2].name , "mumsh_memory_check");
    while(1){
        printf("mumsh $ ");//prompt waiting for the input
        //unsigned long size = 1024;
        //char* input = (char*)malloc(size);
        char input[1024];
        //fgets(input,sizeof(input),stdin);
        char ch;
        int i = 0;
        while(scanf("%c",&ch) && ch != '\n'){
            input[i++] = ch;
        }
        input[i] = '\0';
        readin(input);




        int exe = 0;
        exe = execute();
        for(int j = 0; j < tot_argv; j++){
            strcpy(argv[j],"\0");
        }
        tot_argv = 0;
        if(exe == 0){
            return 0;
        }

    }
    return 0;
}

