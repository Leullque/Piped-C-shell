#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 20


char argv[50][MAX_SIZE];
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

char* combine_output(char* a, char* b){
    strcat(a,b);
    return a;
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
    strcpy(output,files[index[0]].name);
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
    strcat(output,"\n");
    return output;
}


char* command_cat(char file[]){
    char* output = (char*)malloc(30 * sizeof(char));
    int find = 0;
    for(int i = 0; i < tot_file; i++){
        if(strcmp(file, files[i].name) == 0){
            strcpy(output,files[i].content);
            find = 1;
            break;
        }
    }
    if(find == 0){
        printf("error: no this file\n");
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
            char temp[1024];
            strcpy(temp, files[i].content);
            sect = strtok(temp, "\n");

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
        printf("error: no this file\n");
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
        printf("error: no this file\n");
    }
    char *sect_a;
    sect_a = NULL;
    char temp[1024];
    strcpy(temp,files[a_index].content);
    sect_a = strtok(temp, "\n");

    char *sect_b;
    sect_b = NULL;
    strcpy(temp,files[b_index].content);
    sect_b = strtok(temp, "\n");

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

char*  command_grep(char input[],char target[]){
    char* output = (char*)malloc(30 * sizeof(char));
    strcpy(output , "\0");
    int length = 0;
    char *sect;
    sect = NULL;
    sect = strtok(input, "\n");

    while(sect){
        char* position = strstr(sect,target);
        if(length == 0 &&  position != NULL){
            strcpy(output,sect);
            strcat(output,"\n");
        }else if(position != NULL){
            strcat(output,sect);
            strcat(output,"\n");
        }
        length++;
        sect = strtok(NULL,"\n");
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


char* execute(char input[], int pip){
    char* output = NULL;
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
        output = "exit\n";
    }else if(strcmp(argv[0], "diff") == 0){
        if(inredirection != 1){
            output = "diff: missing operand after 'diff'\n diff: Try 'diff --help' for more information.\n";
        }else{
            output = command_diff(argv[tot_argv-1],in_file_name);
        }
    }else if(strcmp(argv[0], "ls") == 0){
        if(tot_argv == 1) {
            output = command_ls();
        }else{
            char output_1[] = ".\n..\n";
            char* output_2 = command_ls();
            output = combine_output(output_1,output_2);
            //strcpy(output_1, output_2);
            //output = output_1;
            //printf("%s",output);
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
            }
            else{
                output = combine_output(output,command_cat(argv[i]));
            }
        }
    }else if(strcmp(argv[0], "head") == 0){
        if(inredirection == 1){
            output = command_head(in_file_name);
        }
        for(int i = 1; i < tot_argv; ++i){
            if(i == 1){
                output = command_head(argv[i]);
            }
            else{
                strcat(output,command_head(argv[i]));
            }
        }
    }else if(strcmp(argv[0], "grep") == 0){
        output = command_grep(input,argv[1]);
    }

    if(outredirection == 1){
        command_output_redirect(out_file_name,output,cases);
    }
    else if(pip == 0){
        printf("%s",output);
    }
    return output;
}



int main(void){
    strcpy(files[0].name , "driver");
    strcpy(files[0].content , "driver\n");
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
                strcpy(argv[j++],"\0");
                int rest_num = tot_argv - j;
                for(int r = 0; r < tot_argv; r++){
                    if(r+j < tot_argv){
                        strcpy(rest_argv[r],argv[r+j]);
                        strcpy(argv[r+j],"\0");
                    }else{
                        strcpy(rest_argv[r],"\0");
                    }
                }

                tot_argv = j - 1;
                std_out = execute(std_in,1);
                free(std_in);
                if(strcmp(std_out, "exit\n") == 0){
                    return 0;
                }
                tot_argv = rest_num;
                for(int k = 0; k < tot_argv; k++){
                    strcpy(argv[k],rest_argv[k]);
                }
                break;
            }
        }
//        printf("list argv");
//        for (int k = 0; k < tot_argv; ++k) {
//            printf("%s ", argv[k]);
//        }
        //printf("%s",std_out);
        char* final_output = execute(std_out,0);
        //printf("result: %s",final_output);
        if(strcmp(final_output, "exit\n") == 0){
            return 0;
        }
        // clear all
        for(int j = 0; j < tot_argv; j++){
            strcpy(argv[j],"\0");
        }
        tot_argv = 0;
//        free(final_output);
//        free(std_out);
    }
    return 0;
}

