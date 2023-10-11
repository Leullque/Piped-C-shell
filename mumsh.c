#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_SIZE 50

void command_exit(void){
    printf("exit\n");
    exit(0);
}

void command_pwd(void){
    char *buffer = malloc(MAX_INPUT_SIZE);
    getcwd(buffer, 1024);
    printf("%s\n", buffer);
    free(buffer);
}

void command_cd(char argv_in[]){
    char path[1024];
    char *sect;
    sect = NULL;
    sect = strtok(argv_in, "cd ");
    if (sect != NULL){
        strcpy(path,sect);
    }
    int exe = chdir(path);
    if (exe == -1){
        printf("%s: No such file or directory\n",path);
    }
}

void ctrl_c_handler(int sig){
    if(sig == SIGINT){
        command_exit();
    }
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
        if (*outredirection != 0) {
            printf("error: duplicated output redirection\n");
            //exit(-1);
        }
           strcpy(out_file_name , argv_line[j+1]);
           *outredirection = 1;
           j += 2;
       }else if(strcmp(argv_line[j],">>") == 0){
        if (*outredirection != 0) {
            printf("error: duplicated output redirection\n");
            //exit(-1);
        }
           strcpy(out_file_name , argv_line[j+1]);
           *outredirection = 2;
           j += 2;
       }else if(strcmp(argv_line[j],"<") == 0){
        if (*inredirection != 0) {
            printf("error: duplicated input redirection\n");
            //exit(-1);
        }
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

void exe_input_redirection(char in_file_name[]){
    int fd = open(in_file_name, O_RDONLY);
    if (fd == -1) {
        printf("%s: No such file or directory\n",in_file_name);
        //perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("Error redirecting input");
        exit(EXIT_FAILURE);
    }
    close(fd);
}


void exe_inoutput_redirection(char in_file_name[], char out_file_name[], int outredirection){
    int fd_in;
    fd_in = open(in_file_name, O_RDONLY);
    if (fd_in == -1) {
        printf("%s: No such file or directory\n",in_file_name);
        //perror("Error opening input file");
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
        printf("%s: Permission denied\n",out_file_name);
        //perror("Error opening output file");
        exit(EXIT_FAILURE);
        }
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
}

void exe_output_redirection(char out_file_name[], int outredirection){
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
            printf("%s: Permission denied\n",out_file_name);
            //perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
        //execvp(args[0], args);
        if (fd != STDOUT_FILENO) {
            close(fd);
        }  
        close(fd);
}

int main(void) {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_INPUT_SIZE/2];
    char argv[50];
    char argv_in[1024];
    char out_file_name[50];
    char in_file_name[50];

    signal(SIGINT, ctrl_c_handler);

    printf("mumsh $ ");
    fflush(stdout);
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            command_exit();
            // Exit on EOF (Ctrl-D)
        }
        input[strcspn(input, "\n")] = '\0';

        int argc = 0;
        int outredirection = 0;
        int inredirection = 0;

        int pip = 0;
        char *sect;
        sect = NULL;
        char temp_input[1024];
        memset(temp_input, '\0', sizeof(temp_input));
        char rest_input[1024];
        memset(rest_input, '\0', sizeof(rest_input));
        strcpy(temp_input,input);
        sect = strtok(temp_input, "|");
        if (sect != NULL) {
            strcpy(input,sect);
            sect = strtok(NULL,"|");
            if (sect != NULL) {
                pip = 1;
                strcpy(rest_input,sect);
                sect = strtok(NULL,"|");
                if(sect != NULL){
                    printf("error: missing program\n");
                }
            }
        }
        if (strcmp(input, "exit") == 0) {
            command_exit();
        }

        int exe = 0;
        int cd = 0;
        if (strcmp(input, "pwd") == 0) {
            command_pwd();
            exe = 1;
        }
        if (pip == 1){
            if(exe == 1){
                if (strcmp(rest_input, "pwd") == 0) {
                command_pwd();
                } else {
                    pip = 0;
                    exe = 0;
                    strcpy(input, rest_input);
                }
            } else {
                if (strcmp(rest_input, "pwd") == 0) {
                    pip = 0;
                    command_pwd();
                }
            }
        }


       //printf("input: %s",input);
        readin(input, argv, argv_in, &argc, out_file_name, in_file_name, &outredirection, &inredirection);
        int argc_c = 0;
        char temp[1024];
        strcpy(temp, argv_in);
        char *token = strtok(temp, " ");
        while (token != NULL) {
            args[argc_c++] = token;
            token = strtok(NULL, " ");
        }
        args[argc_c] = NULL;
        if(strcmp(argv, "cd") == 0){
            command_cd(argv_in);
            exe = 1;
        }

        
        // Execute the command
        if (argc > 0 && exe == 0) {
            int exec = 0;
            if(pip == 0){
                pid_t child_pid;
                if ((child_pid = fork()) == 0) {
                    if (inredirection != 0 && outredirection != 0) {
                        exe_inoutput_redirection(in_file_name, out_file_name, outredirection);
                        } else if (inredirection != 0) {
                        exe_input_redirection(in_file_name);
                        } else if (outredirection != 0) {
                        exe_output_redirection(out_file_name, outredirection);
                    }
                    exec = execvp(args[0], args);
                    if(exec == -1){
                        printf("non-exist: %s not found\n", args[0]);
                    }
                
                    //perror("execvp");
                    exit(1);
                    } else if (child_pid > 0) {
                    // Parent process
                    int status;
                    waitpid(child_pid, &status, 0);
                    } else {
                    perror("fork");
                    } 
            } else if(pip == 1){
                // Deal with pipeline
                pid_t child_pid = fork();

                if (child_pid == 0) {
                int pipefd[2];
                pipe(pipefd); // Create a pipe
                // child: left_command
                // parent: right_command 
                pid_t ppid = fork();
                if (ppid == 0) {
                close(pipefd[0]); // close read port
                if (outredirection != 0) {
                    printf("error: duplicated output redirection\n");
                    continue;
                    }
                if (inredirection != 0) {
                    exe_input_redirection(in_file_name);
                    }
                dup2(pipefd[1], STDOUT_FILENO);
                exec = execvp(args[0], args);
                if(exec == -1){
                    printf("non-exist: %s not found\n", args[0]);
                }
                close(pipefd[1]); // close write port
                exit(0);
                } else {
                    close(pipefd[1]); // close write port
                    dup2(pipefd[0], STDIN_FILENO);

                    // deal with rest_input
                    memset(argv, '\0', sizeof(argv));
                    memset(argv_in, '\0', sizeof(argv_in));
                    memset(in_file_name, '\0', sizeof(in_file_name));
                    memset(out_file_name, '\0', sizeof(out_file_name));
                    memset(args, '\0', sizeof(args));

                    readin(rest_input, argv, argv_in, &argc, out_file_name, in_file_name, &outredirection, &inredirection);
                    
                    argc_c = 0;
                    memset(temp, '\0', sizeof(temp));
                    strcpy(temp, argv_in);
                    char *rest_token = strtok(temp, " ");
                    while (rest_token != NULL) {
                        args[argc_c++] = rest_token;
                        rest_token = strtok(NULL, " ");
                    }
                    args[argc_c] = NULL; // Null-terminate the argument list

                    if (inredirection != 0) {
                            printf("error: duplicated input redirection\n");
                            continue;
                        }
                    if (outredirection != 0) {
                            exe_output_redirection(out_file_name, outredirection);
                        }
                        
                        exec = execvp(args[0], args);
                        if(exec == -1){
                            printf("non-exist: %s not found\n", args[0]);
                        }
                        close(pipefd[0]); // close write port
                        exit(0);
                    }
                } else {
                    int status;
                    waitpid(child_pid, &status, 0);
                    int err = WEXITSTATUS(status);
                    if (err) { 
                        printf("Error: %s\n", strerror(err));
                    }
                }
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
        fflush(stdout);
        printf("mumsh $ ");
        fflush(stdout);
    }

    return 0;
}
