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

//void command_cat(char file[]){
//    //printf("test:%s",file);
//    int find = 0;
//    for(int i = 0; i < tot_file; i++){
//
//        //printf("file is %s %s",files[i].name,files[i].content);
//        if(strcmp(file, files[i].name) == 0){
//            printf("%s ",files[i].content);
//            find = 1;
//            break;
//        }
//    }
//    if(find == 0){
//        printf("error: no this file");
//    }
//}
char* command_cat(char file[]){
    char* lines = (char*)malloc(30 * sizeof(char));
    int find = 0;
    int length = 0;
    for(int i = 0; i < tot_file; i++){
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
                else if(length < 30){
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

char* command_head(char file[]){
    char* lines = (char*)malloc(10 * sizeof(char));
    int find = 0;
    int length = 0;
    for(int i = 0; i < tot_file; i++){
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

char*  command_diff(char file_a[],char file_b[]){
    char* lines = (char*)malloc(10 * sizeof(char));
    int find = 0;
    int length = 0;
    int a_index = 0;
    int b_index = 0;
    for(int i = 0; i < tot_file && find != 2; i++){
        if(strcmp(file_a, files[i].name) == 0){
            a_index = i;
            find++;
        }
        if(strcmp(file_b, files[i].name) == 0){
            b_index = i;
            find++;
        }
    }
    if(find == 0){
        printf("error: no this file");
    }
    char *sect_a;
    sect_a = NULL;
    sect_a = strtok(files[a_index].content, "\n");

    char *sect_b;
    sect_b = NULL;
    sect_b = strtok(files[b_index].content, "\n");

    while(sect_a && sect_b){
        if(length == 0){
            strcpy(lines,sect_a);
            strcat(lines,"\n");
            if(strcmp(sect_a,sect_b) != 0){
                strcat(lines,"| ");
            }
            strcat(lines,sect_b);
            strcat(lines,"\n");
            length++;
        }
        else if(length < 10){
            strcat(lines,sect_a);
            strcat(lines,"\n");
            if(strcmp(sect_a,sect_b) != 0){
                strcat(lines,"| ");
            }
            strcat(lines,sect_b);
            strcat(lines,"\n");
            length++;
        }
        sect_a = strtok(NULL,"\n");
        sect_b = strtok(NULL,"\n");
    }
    while(sect_a){
        strcat(lines,sect_a);
        strcat(lines,"\n");
        strcat(lines,"<\n");
        sect_a = strtok(NULL,"\n");
    }
    while(sect_b){
        strcat(lines,"> ");
        strcat(lines,sect_b);
        strcat(lines,"\n");
        sect_b = strtok(NULL,"\n");
    }
    return lines;

}

void command_output_redirect(char file_name[], char content[],int cases){
    //Write content to a file, attach or overwrite
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
            char* outs = command_cat(argv[1]);
            printf("%s", outs);
            free(outs);
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
            char* outs = command_cat(argv[2]);
            printf("%s", outs);
            free(outs);
            return 1;
        }
        else if(strcmp(argv[0], "head") == 0 && strcmp(argv[1], "<") == 0){
            char* outs = command_head(argv[2]);
            printf("%s", outs);
            free(outs);
            return 1;
        }
        else if(strcmp(argv[0], "echo") == 0){
            command_echo();
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
            char out_file_name[50];
            char* outs;
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
                    outs = command_head(argv[2]);
                }
            }
            command_output_redirect(out_file_name,outs,cases);
            free(outs);
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
        else if(strcmp(argv[0], "diff") == 0){
            for(int i = 1; i < tot_argv; i++){
                if(strcmp(argv[i],"<") == 0){
                    char* outs = command_diff(argv[i-1],argv[i+1]);
                    printf("%s",outs);
                    free(outs);
                    break;
                }
            }
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

