#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 20


//char argv[50][MAX_SIZE];
//int tot_argv = 0;

struct File{
    char  name[50];
    char  content[1024];
};

struct File files[1024];
int tot_file = 3;

//void test(char input[]){
//    printf("test:%s",input);
//}

char* combine_output(char* a, char* b){
    strcat(a,b);
    return a;
}

void readin(char* input, char* argv, char* argv_in,
            char* out_file_name, char* in_file_name, int* outredirection, int* inredirection){// outputredirection= 1: > 2: >> 0: no;
    // First we read in array of string, including dealing with space
    char argv_line[50][MAX_SIZE];
    int tot_argv = 0;
    char *sect;
    sect = NULL;
    sect = strtok(input, " ");
    while(sect){
        strcpy(argv_line[tot_argv],sect);
        tot_argv++;
        sect = strtok(NULL," ");
    }
    for(int k = 0; k < tot_argv - 1; k++){
        if(strcmp(argv_line[k],">")==0 && strcmp(argv_line[k+1],">")==0){
            strcpy(argv_line[k] , ">>");
            for(int j = k + 1; j < tot_argv; j++){
                strcpy(argv_line[j] , argv_line[j+1]);
            }
            tot_argv--;
        }
    }

    // Now we further split the information from argv
    char temp_argv[50][MAX_SIZE];
    int k = 0;
    for(int j = 0; j < tot_argv;){
        if(strcmp(argv_line[j],">") == 0){
            strcpy(out_file_name , argv_line[j+1]);
            *outredirection = 1;
            j += 2;
        }else if(strcmp(argv_line[j],">>") == 0){
            strcpy(out_file_name , argv_line[j+1]);
            *outredirection = 2;
            j += 2;
        }else if(strcmp(argv_line[j],"<") == 0){
            strcpy(in_file_name , argv_line[j+1]);
            *inredirection = 1;
            j += 2;
            //output = command_cat(argv[j+1]);
        }else{
            strcpy(temp_argv[k++] , argv_line[j++]);
        }
    }
    tot_argv = k;
    strcpy(argv , temp_argv[0]);
    for (int i = 1; i < tot_argv; ++i) {
        if(i == 1){
            strcpy(argv_in, temp_argv[i]);
            strcat(argv_in," ");
        }else{
            strcat(argv_in, temp_argv[i]);
            strcat(argv_in," ");
        }
    }
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
    char* output = (char*)malloc(30 * sizeof(char));
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
            if(cases == 1){
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
        if(cases == 1){
            strcpy(files[tot_file].content,content);
        }else{
            strcat(files[tot_file].content,"\n");
            strcat(files[tot_file].content,content);
        }
        tot_file += 1;
    }
}


char* execute(char argv[], char argv_in[], char out_file_name[], char in_file_name[],
              int inredirection, int outredirection, int pip, char input[]){
    char* output = "exe";
    // redirection checking & clean command
    char rest_line[50][MAX_SIZE];
    int tot_argv = 0;
    char *sect;
    sect = NULL;
    char temp_line[1024] = "\0";
    strcpy(temp_line, argv_in);
    sect = strtok(temp_line, " ");
    while(sect){
        strcpy(rest_line[tot_argv],sect);
        tot_argv++;
        sect = strtok(NULL," ");
    }

    if(strcmp(argv, "exit") == 0){
        output = "exit\n";
    }else if(strcmp(argv, "diff") == 0){
        if(inredirection != 1){
            output = "diff: missing operand after 'diff'\n diff: Try 'diff --help' for more information.\n";
        }else{
            output = command_diff(rest_line[tot_argv-1],in_file_name);
        }
    }else if(strcmp(argv, "ls") == 0){
        if(tot_argv == 0) {
            output = command_ls();
        }else{
            char output_1[] = ".\n..\n";
            char* output_2 = command_ls();
            output = combine_output(output_1,output_2);
        }
    }else if(strcmp(argv, "echo") == 0 || strcmp(argv, "yes") == 0){
        strcat(argv_in, "\n");
        output = argv_in;
    }else if(strcmp(argv, "cat") == 0){
        if(inredirection == 1) {
            output = command_cat(in_file_name);
        }
        for(int i = 0; i < tot_argv; ++i){
            if(i == 0){
                output = command_cat(rest_line[i]);
            }
            else{
                output = combine_output(output,command_cat(rest_line[i]));
            }
        }
    }else if(strcmp(argv, "head") == 0){
        if(inredirection == 1){
            output = command_head(in_file_name);
        }
        for(int i = 0; i < tot_argv; ++i){
            if(i == 0){
                output = command_head(rest_line[i]);
            }
            else{
                strcat(output,command_head(rest_line[i]));
            }
        }
    }else if(strcmp(argv, "grep") == 0){
        output = command_grep(input,rest_line[0]);
    }else if(strcmp(argv, "sleep") == 0){
        unsigned int duration = (unsigned int) strtoul(argv_in,NULL,10);
        sleep(duration);
        pip = 1;
    }else{
        output = "Error: Undefined command!\n";
        pip = 0;
    }

    if(outredirection != 0){
        command_output_redirect(out_file_name,output,outredirection);
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
    printf("mumsh $ ");
    while(1){
        //printf("mumsh $ ");//prompt waiting for the input
        char input[1024];
        //char ch;
        int i = 0;

        //deal with > < and space + read in command line
//        while(scanf("%c",&ch) && ch != '\n'){
//            if(ch == '>' || ch == '<'){
//                input[i++] = ' ';
//                input[i++] = ch;
//                input[i++] = ' ';
//            }else{
//                input[i++] = ch;
//            }
//        }
//        input[i] = '\0';
        int result = scanf("%1023[^\n]", input);
        if(result == EOF) {
            break;
        }else if(result == 0) {
            scanf("%*c");
        }
        else{
            char rest_input[1024];
            int pip = 0;
            for (int j = 0; j < i; ++j) {
                if(input[j] == '|'){
                    pip = 1;
                    input[j++] = '\0';
                    for(int r = 0; r < i; r++){
                        if(r+j < i){
                            rest_input[r] = input[r+j];
                            input[r+j] = '\0';
                        }else{
                            rest_input[r] = '\0';
                        }
                    }
                }
            }
//        printf("input: %s ",input);
//        printf("rest_input: %s ",rest_input);

            char *argv = (char *) malloc(20);
            char *argv_in = (char *) malloc(1024);
            char* out_file_name = (char *) malloc(30);
            char* in_file_name = (char *) malloc(30);
            strcpy(argv, "\0");
            strcpy(argv_in, "\0");
            strcpy(out_file_name, "\0");
            strcpy(in_file_name, "\0");
            int outredirection = 0;
            int inredirection = 0;
            readin(input, argv, argv_in,
                   out_file_name, in_file_name, &outredirection, &inredirection);
            //printf("read in input: %s",input);
            char std_in[1024] = "\0";
            char std_out[1024] = "\0";


            char *argv_2 = (char *) malloc(20);
            char *argv_in_2 = (char *) malloc(1024);
            char* out_file_name_2 = (char *) malloc(30);
            char* in_file_name_2 = (char *) malloc(30);
            strcpy(argv_2, "\0");
            strcpy(argv_in_2, "\0");
            strcpy(out_file_name_2, "\0");
            strcpy(in_file_name_2, "\0");
            int outredirection_2 = 0;
            int inredirection_2 = 0;
            readin(rest_input, argv_2, argv_in_2,
                   out_file_name_2, in_file_name_2, &outredirection_2, &inredirection_2);

            if(pip == 0){
                strcpy(std_out, execute(argv, argv_in, out_file_name, in_file_name,
                                        inredirection, outredirection, pip, std_in));
            }else{
                if(strcmp(argv, "exit") == 0 || strcmp(argv, "sleep") == 0 ||
                   strcmp(argv_2, "exit") == 0 || strcmp(argv_2, "sleep") == 0){
                    strcpy(std_out, execute(argv, argv_in, out_file_name, in_file_name,
                                            inredirection, outredirection, pip, std_in));
                }else{
                    strcpy(std_out, execute(argv, argv_in, out_file_name, in_file_name,
                                            inredirection, outredirection, pip, std_in));
                    //printf("std_out is:%s",std_out);
                    strcpy(std_in, std_out);
                    //printf("std_in is:%s",std_in);
                    strcpy(std_out, "\0");
                    strcpy(std_out, execute(argv_2, argv_in_2, out_file_name_2, in_file_name_2,
                                            inredirection_2, outredirection_2, 0, std_in));
                    //printf("std_out is:%s",std_out);
                }
            }

            free(argv);
            free(argv_in);
            free(out_file_name);
            free(in_file_name);
            if(strcmp(std_out, "exit\n") == 0){
                return 0;
            }
            printf("mumsh $ ");

        }
    }

    return 0;
}

