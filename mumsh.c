#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_SIZE 50

void command_exit(void){
    printf("exit\n");
    exit(0);
}

void readin(char line_in[], char* argv, char* argv_in, int* argc,
char* out_file_name, char* in_file_name, int* outredirection, int* inredirection){
    //outputredirection= 1: > 2: >> 0: no;
   // First we read in array of string, including dealing with space
   char argv_line[MAX_SIZE][MAX_INPUT_SIZE/2];
   char input[MAX_INPUT_SIZE]; 
   memset(argv_line, '\0', sizeof(argv_line));
   memset(input, '\0', sizeof(input));
   int index_input = 0;
   for (int j = 0; j < (int) strlen(line_in);) {
        if (line_in[j] == '>' || line_in[j] == '<'){
            input[index_input++] = ' ';
            input[index_input++] = line_in[j++];
            input[index_input++] = ' ';
        } else {
            input[index_input++] = line_in[j++];
            }
    }

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
           for(int j = k + 1; j < tot_argv - 1; j++){
               strcpy(argv_line[j] , argv_line[j+1]);
           }
           tot_argv--;
       }
   }

   // Now we further split the information from argv
   char temp_argv[MAX_SIZE][MAX_INPUT_SIZE/2];
   memset(temp_argv, '\0', sizeof(temp_argv));
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
   *argc = tot_argv;
   strcpy(argv , temp_argv[0]);
   for (int i = 0; i < tot_argv; ++i) {
       if(i == 0){
        strcpy(argv_in,temp_argv[i]);
        strcat(argv_in," ");
       } else {
        strcat(argv_in,temp_argv[i]);
        strcat(argv_in," ");
       }
           
   }
   //argv_in[*argc] = NULL;
}

void exe_normal(char* args[]){
    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("execvp"); // Print an error message if execvp fails
        exit(1);
    } else if (child_pid > 0) {
        // Parent process
        int status;
        waitpid(child_pid, &status, 0); // Wait for the child to finish
    } else {
        perror("fork"); // Print an error message if fork fails
    }   
}

void exe_input_redirection(char* args[], char in_file_name[]){
    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        int fd = open(in_file_name, O_RDONLY);
        if (fd == -1) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
                }
        if (dup2(fd, STDIN_FILENO) == -1) {
                perror("Error redirecting input");
                exit(EXIT_FAILURE);
            }
        close(fd);

        // Execute the command
        execvp(args[0], args);

        perror("Error executing command");
        exit(EXIT_FAILURE);
        } else if (child_pid > 0){
            // Parent process
            int status;
            waitpid(child_pid, &status, 0);
        } else {
        perror("fork"); // Print an error message if fork fails
        }     
}

void exe_inoutput_redirection(char* args[], char in_file_name[], char out_file_name[], int outredirection){
    pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        int fd_in;
        fd_in = open(in_file_name, O_RDONLY);
        if (fd_in == -1) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
                }
        if (dup2(fd_in, STDIN_FILENO) == -1) {
                perror("Error redirecting input");
                exit(EXIT_FAILURE);
            }
        close(fd_in);

        int fd_out = 0;
        switch (outredirection)
            {
            case 1:
                fd_out = open(out_file_name, O_CREAT|O_RDWR|O_TRUNC,0664);
            case 2:
                fd_out = open(out_file_name, O_CREAT|O_RDWR|O_APPEND,0664);
            default:
                break;
            }

        
        if (fd_out == -1) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
                }
        dup2(fd_out, STDOUT_FILENO);
        execvp(args[0], args);
        if (fd_out != STDOUT_FILENO) {
                close(fd_out);
            }    
        printf("%s: Syntax error\n", args[0]);
        exit(1);
        } else if (child_pid > 0){
            // Parent process
            int status;
            waitpid(child_pid, &status, 0);
        } else {
        perror("fork"); // Print an error message if fork fails
        }
}

void exe_output_redirection(char* args[], char out_file_name[], int outredirection){
        pid_t child_pid = fork();
    if (child_pid == 0) {
        // Child process
        int fd = 0;
        switch (outredirection)
            {
            case 1:
                fd = open(out_file_name, O_CREAT|O_RDWR|O_TRUNC,0664);
            case 2:
                fd = open(out_file_name, O_CREAT|O_RDWR|O_APPEND,0664);
            default:
                break;
            }

        
        if (fd == -1) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
                }
        dup2(fd, STDOUT_FILENO);
        execvp(args[0], args);
        if (fd != STDOUT_FILENO) {
                close(fd);
            }    
        printf("%s: Syntax error\n", args[0]);
        exit(1);
        } else if (child_pid > 0){
            // Parent process
            int status;
            waitpid(child_pid, &status, 0);
        } else {
        perror("fork"); // Print an error message if fork fails
        }
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_INPUT_SIZE/2];
    char argv[50];
    char argv_in[1024];
    char out_file_name[50];
    char in_file_name[50];

    printf("mumsh $ ");
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on EOF (Ctrl-D)
        }
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            command_exit();
        }

        // char *argv = (char *) malloc(20);
        // char *argv_in = (char *) malloc(1024);
        // int argc = 0;
        // char* out_file_name = (char *) malloc(30);
        // char* in_file_name = (char *) malloc(30);

        int argc = 0;
        int outredirection = 0;
        int inredirection = 0;

        // memset(argv, '\0', sizeof(argv));
        // memset(argv_in, '\0', sizeof(argv_in));
        // memset(in_file_name, '\0', sizeof(in_file_name));
        // memset(out_file_name, '\0', sizeof(out_file_name));


        readin(input, argv, argv_in, &argc, out_file_name, in_file_name, &outredirection, &inredirection);

        // printf("read in input: %s\n",input);
        // printf("read in argv: %s\n",argv);
        // printf("read in argv_in: %s\n",argv_in);
        // printf("read in argc: %d\n",argc);
        // printf("read in out_file_name: %s\n",out_file_name);
        // printf("read in in_file_name: %s\n",in_file_name);
        // Somehow we need to change the format of argv_in into args

        int argc_c = 0;
        char temp[1024];
        strcpy(temp, argv_in);
        char *token = strtok(temp, " ");
        while (token != NULL) {
            args[argc_c++] = token;
            token = strtok(NULL, " ");
        }
        args[argc_c] = NULL; // Null-terminate the argument list

        // Execute the command using execvp
        if (argc > 0) {
            if (inredirection != 0 && outredirection != 0) {
                exe_inoutput_redirection(args, in_file_name, out_file_name, outredirection);
            } else if (inredirection != 0) {
                exe_input_redirection(args, in_file_name);
            } else if (outredirection != 0) {
                exe_output_redirection(args, out_file_name, outredirection);
            } else{
                exe_normal(args);
            }
        }
        
        memset(input, '\0', sizeof(input));
        memset(argv, '\0', sizeof(argv));
        memset(argv_in, '\0', sizeof(argv_in));
        memset(in_file_name, '\0', sizeof(in_file_name));
        memset(out_file_name, '\0', sizeof(out_file_name));
        memset(args, '\0', sizeof(args));

        // for (int s = 0; s < MAX_INPUT_SIZE/2; s++) {
        // args[s] = NULL;
        // }
        printf("mumsh $ ");
    }

    return 0;
}
