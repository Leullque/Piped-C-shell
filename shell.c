#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 20


char argv[50][MAX_SIZE];
int tot_argv = 0;
int print = 0;

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

    for(int k = 0; k < tot_argv - 1; k++){
        if(strcmp(argv[k],">")==0 && strcmp(argv[k+1],">")==0){
            strcpy(argv[k] , ">>");
            for(int j = k + 1; j < tot_argv; j++){
                strcpy(argv[j] , argv[j+1]);
            }
            tot_argv--;
        }
    }
    //printf("%d",tot_argv);
//    for(int k = 0; k < tot_argv; k++){
////        printf(argv[k]);
////        printf(" ");
//        test(argv[k]);
//    }
}


char* command_ls(void){
    int i, j, minIndex;
    int index[50];
    int temp = 0;
    char* output = (char*)malloc(30 * sizeof(char));

    //selection sort for the name alphabetically
    for (int k = 0; k < 50; ++k) {
        index[k] = k;
    }
    for (i = 0; i < tot_file - 1; i++){
        minIndex = i;
        for (j = i + 1; j < tot_file; j++) {
            if (strcmp(files[index[j]].name, files[index[minIndex]].name) < 0){
                minIndex = j;
            }
        }
        if (minIndex != i) {
            temp = index[i];
            index[i] = index[minIndex];
            index[minIndex] = temp;
        }
    }

    //print
    strcpy(output,files[0].name);
    strcat(output,"\n");
    for(int k = 1; k < tot_file; k++){
        //printf("%s\n",files[index[k]].name);
        strcat(output,files[index[k]].name);
        strcat(output,"\n");
    }
    return output;
}

char* command_echo(void){
    char* output = (char*)malloc(30 * sizeof(char));
    strcpy(output,argv[1]);
    strcat(output," ");
    for(int i = 2; i < tot_argv; i++){
        //printf("%s ",argv[i]);
        strcat(output,argv[i]);
        strcat(output," ");
    }
    return output;
}


char* command_cat(char file[]){
    char* output = (char*)malloc(30 * sizeof(char));
    int find = 0;
    int length = 0;
    for(int i = 0; i < tot_file; i++){
        if(strcmp(file, files[i].name) == 0){
            char *sect;
            sect = NULL;
            sect = strtok(files[i].content, "\n");

            while(sect){
                if(length == 0){
                    strcpy(output,sect);
                    strcat(output,"\n");
                    length++;
                }else if(length < 30){
                    strcat(output,sect);
                    strcat(output,"\n");
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
    return output;
}

char* command_head(char file[]){
    char* output = (char*)malloc(10 * sizeof(char));
    int find = 0;
    int length = 0;
    for(int i = 0; i < tot_file; i++){
        if(strcmp(file, files[i].name) == 0){
            char *sect;
            sect = NULL;
            sect = strtok(files[i].content, "\n");

            while(sect){
                if(length == 0){
                    strcpy(output,sect);
                    strcat(output,"\n");
                    length++;
                }else if(length < 10){
                    strcat(output,sect);
                    strcat(output,"\n");
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
    return output;
}

char*  command_diff(char file_a[],char file_b[]){
    char* output = (char*)malloc(10 * sizeof(char));
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
            strcpy(output,sect_a);
            strcat(output,"\n");
            if(strcmp(sect_a,sect_b) != 0){
                strcat(output,"| ");
            }
            strcat(output,sect_b);
            strcat(output,"\n");
            length++;
        }else if(length < 10){
            strcat(output,sect_a);
            strcat(output,"\n");
            if(strcmp(sect_a,sect_b) != 0){
                strcat(output,"| ");
            }
            strcat(output,sect_b);
            strcat(output,"\n");
            length++;
        }
        sect_a = strtok(NULL,"\n");
        sect_b = strtok(NULL,"\n");
    }
    while(sect_a){
        strcat(output,sect_a);
        strcat(output,"\n");
        strcat(output,"<\n");
        sect_a = strtok(NULL,"\n");
    }
    while(sect_b){
        strcat(output,"> ");
        strcat(output,sect_b);
        strcat(output,"\n");
        sect_b = strtok(NULL,"\n");
    }
    return output;
}

void command_output_redirect(char file_name[], char content[],int cases){
    //Write content to a file, attach or overwrite
    int find = 0;
    for(int i = 0; i < tot_file; i++){
        if(strcmp(file_name, files[i].name)==0){
            //printf("find%s",files[i].content);
            if(cases == 0){
                strcpy(files[i].content,content);
            }else{
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
        }else{
            strcat(files[tot_file].content,"\n");
            strcat(files[tot_file].content,content);
        }
        tot_file += 1;
    }
}


char* execute(void){
    char* output = "exe";
    // redirection checking & clean command
    char out_file_name[50];
    char in_file_name[50];
    int cases = 0;
    int outredirection = 0;
    int inredirection = 0;
    char temp_argv[50][MAX_SIZE];
    int k = 0;
    for(int j = 0; j < tot_argv;){
        if(strcmp(argv[j],">") == 0){
            strcpy(out_file_name , argv[j+1]);
            cases = 0;
            outredirection = 1;
            j += 2;
        }else if(strcmp(argv[j],">>") == 0){
            strcpy(out_file_name , argv[j+1]);
            cases = 1;
            outredirection = 1;
            j += 2;
        }else if(strcmp(argv[j],"<") == 0){
            strcpy(in_file_name , argv[j+1]);
            inredirection = 1;
            j += 2;
            //output = command_cat(argv[j+1]);
        }else{
            strcpy(temp_argv[k++] , argv[j++]);
        }
    }
    tot_argv = k;
    for (int i = 0; i < tot_argv; ++i) {
        strcpy(argv[i],temp_argv[i]);
    }
//    for (int i = 0; i < tot_argv; ++i) {
//        printf("%s ",argv[i]);
//    }

    if(strcmp(argv[0], "exit") == 0){
        output = "exit";
    }else if(strcmp(argv[0], "diff") == 0){
        if(inredirection != 1){
            output = "diff: missing operand after 'diff'\n diff: Try 'diff --help' for more information.";
        }else{
            output = command_diff(argv[tot_argv-1],in_file_name);
        }
    }else if(strcmp(argv[0], "ls") == 0){
        if(tot_argv == 1) {
            output = command_ls();
        }else{
            output = ".\n..";
            strcat(output, command_ls());
        }
    }else if(strcmp(argv[0], "echo") == 0){
        output = command_echo();
    }else if(strcmp(argv[0], "cat") == 0){
        if(inredirection == 1) {
            output = command_cat(in_file_name);
        }
        for(int i = 1; i < tot_argv; ++i){
            if(i == 1){
                output = command_cat(argv[i]);
                strcat(output,"\n");
            }
            else{
                strcat(output,command_cat(argv[i]));
                strcat(output,"\n");

            }
        }
    }else if(strcmp(argv[0], "head") == 0){
        if(inredirection == 1){
            output = command_head(in_file_name);
        }
        for(int i = 1; i < tot_argv; ++i){
            if(i == 1){
                output = command_head(argv[i]);
                strcat(output,"\n");
            }
            else{
                strcat(output,command_head(argv[i]));
                strcat(output,"\n");
            }
        }
    }

    if(outredirection == 1){
        command_output_redirect(out_file_name,output,cases);
    }
    else{
        printf("%s",output);
    }
    return output;
}



int main(void){
    strcpy(files[0].name , "driver");
    strcpy(files[0].content , "driver");
    strcpy(files[1].name , "mumsh");
    strcpy(files[2].name , "mumsh_memory_check");
    while(1){
        printf("mumsh $ ");//prompt waiting for the input
        char input[1024];
        char ch;
        int i = 0;
        while(scanf("%c",&ch) && ch != '\n'){
            if(ch == '>' || ch == '<'){
                input[i++] = ' ';
                input[i++] = ch;
                input[i++] = ' ';
            }else{
                input[i++] = ch;
            }
        }
        input[i] = '\0';
        readin(input);

        char* std_in = NULL;
        char* std_out = NULL;
        for(int j = 0; j < tot_argv; j++){
            if(strcmp(argv[j],"|") == 0){
                char rest_argv[50][MAX_SIZE];
                strcpy(argv[j],"\0");
                j++;
                for(int r = 0; r < tot_argv; r++){
                    if(r+j < tot_argv){
                        strcpy(rest_argv[r],argv[r+j]);
                        strcpy(argv[r+j],"\0");
                    }else{
                        strcpy(rest_argv[r],"\0");
                    }
                }
                std_out = execute();
                for(int k = 0; k < tot_argv; k++){
                    strcpy(argv[k],rest_argv[k]);
                }
                break;
            }
        }
        std_out = execute();
        //printf("result: %s",std_out);
        if(strcmp(std_out, "exit") == 0){
            return 0;
        }
        // clear all
        for(int j = 0; j < tot_argv; j++){
            strcpy(argv[j],"\0");
        }
        tot_argv = 0;
    }
    return 0;
}

