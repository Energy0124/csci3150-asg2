/*
 * Author: Ling Leong
 * SID:1155062557
 *
* CSCI3150 Assignment 2 - Writing a Simple Shell
* Feel free to modify the given code.
* Press Ctrl-D to end the program
*
*/

/* Header Declaration */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <wait.h>


/*Global Variable*/
char currentDirectory[1024] = "";
char *history[512][255];
int historySize = 0;


/* Function Declaration */
int getUserInput(char *input);

void tokenizeInput(char *input);

int gofolderCommand(char **args0, int argSize);

int logCommand();

int byeCommand(int size);

int executeSystemCommand(char **args, int size);

int parseCommand(char **tokens, int argSize);

/* Functions */
int main(int argc, char *argv[]) {
    int isExit = 0;

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    //init cwd
    getcwd(currentDirectory, sizeof(currentDirectory));


    do {
        char rawInput[255];
        isExit = getUserInput(rawInput);
        if (isExit) break;

        tokenizeInput(rawInput);


    } while (isExit = 1);
    return 0;
}

/*
  GetUserInput()
  - To parse User Input and remove new line character at the end.
  - Copy the cleansed input to parameter.
  - Return 1 if encountered EOF (Ctrl-D), 0 otherwise.
*/
int getUserInput(char *input) {
    char buf[255];
    char *s = buf;
    printf("[3150 Shell:%s]=> ", currentDirectory);
    if (fgets(buf, 255, stdin) == NULL) {
        putchar('\n');
        return 1;
    }
    // Remove \n
    for (; *s != '\n'; s++);
    *s = '\0';


    strcpy(input, buf);
    if (historySize < 512) {
        strcpy((char *) history[historySize++], buf);
    }
    return 0;
}

/*
  tokenizeInput()
  - Tokenize the string stored in parameter, delimiter is space
  - In given code, it only prints out the token.
  Please implement the remaining part.
*/

void tokenizeInput(char *input) {
    char buf[255] = {'\0'};
    strcpy(buf, input);
    char *tokens[255] = {NULL};
    tokens[0] = "";
    char **part[255] = {tokens};
    int partSize[255] = {0};
    char operators[255] = {0};
    int sizeOfPart = 0;

    char *token = strtok(buf, " ");
    int argSize = 0;
    while (token != NULL) {
        tokens[argSize] = token;
        token = strtok(NULL, " ");
        argSize++;
    }
    tokens[argSize] = NULL;
    part[sizeOfPart] = tokens;
    operators[sizeOfPart] = 0;
    sizeOfPart++;
    int tSize = 0;
    int j;
    for (j = 0; j < argSize; ++j) {
        if (strcmp(tokens[j], "||") == 0) {
            operators[sizeOfPart] = '|';
            part[sizeOfPart] = &tokens[j + 1];
            tokens[j] = NULL;
            partSize[sizeOfPart - 1] = sizeOfPart > 1 ? tSize - 1 : tSize;
            tSize = 0;
            sizeOfPart++;
        } else if (strcmp(tokens[j], "&&") == 0) {
            operators[sizeOfPart] = '&';
            part[sizeOfPart] = &tokens[j + 1];
            tokens[j] = NULL;
            partSize[sizeOfPart - 1] = sizeOfPart > 1 ? tSize - 1 : tSize;
            tSize = 0;
            sizeOfPart++;
        }
        tSize++;
    }
    partSize[sizeOfPart - 1] = sizeOfPart > 0 ? tSize - 1 : tSize;
    tSize = 0;
    sizeOfPart++;

    int status[255] = {0};
    if (part[0] != NULL && *part[0] != NULL) {
        status[0] = parseCommand(part[0], partSize[0]);
    }

    int result = 0;
    if (status[0] == 0) result = 1;
    int n = 0;
    for (n = 1; n < sizeOfPart; n++) {
        if (operators[n] == '&') {
            if (result == 1) {
                if (part[n] != NULL && *part[n] != NULL) {
                    status[n] = parseCommand(part[n], partSize[n]);
                    if (status[n] == 0) result = result && 1;
                    else result = result && 0;
                }
            }
        } else if (operators[n] == '|') {
            if (result == 0) {
                if (part[n] != NULL && *part[n] != NULL) {
                    status[n] = parseCommand(part[n], partSize[n]);
                    if (status[n] == 0) result = result || 1;
                    else result = result || 0;
                }
            }
        }
    }
    return;
}

int parseCommand(char **tokens, int argSize) {
    int status;
    if (strcmp(tokens[0], "gofolder") == 0) {
        status = gofolderCommand(tokens, argSize);
    } else if (strcmp(tokens[0], "log") == 0) {
        status = logCommand();
    } else if (strcmp(tokens[0], "bye") == 0) {
        status = byeCommand(argSize);
    } else {
        status = executeSystemCommand(tokens, argSize);
    }
    return status;
}

int executeSystemCommand(char **args, int size) {

    pid_t parent = getpid();
    pid_t pid = fork();

    if (pid == -1) {
        // error, failed to fork()
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        return status;
    } else {
        // we are the child
        putenv("PATH=/bin:/usr/bin:.");
        if (execvp(args[0], args) == -1) {
            printf("{%s}: command not found\n", args[0]);
        }

        _exit(EXIT_FAILURE);   // exec never returns
    }
}

int byeCommand(int size) {
    if (size > 1) {
        printf("%s\n", "bye: wrong number of arguments");
        return -1;
    } else {
        exit(0);
    }
}

int logCommand() {
    int i;
    for (i = 0; i < historySize; ++i) {
        printf("[%d]: %s\n", i + 1, (char *) history[i]);
    }
    return 0;
}

int gofolderCommand(char **args, int argSize) {
    if (args[1] != NULL) {
        if (args[1][0] == '\0' || argSize > 2) {
            printf("%s\n", "gofolder: wrong number of arguments");
            return -1;
        } else {
            DIR *dir = opendir(args[1]);
            if (dir) {
                /* Directory exists. */
                closedir(dir);
                chdir(args[1]);
                getcwd(currentDirectory, sizeof(currentDirectory));
                return 0;
            } else if (ENOENT == errno) {
                /* Directory does not exist. */
                printf("{%s}: cannot change directory\n", args[1]);
                return -1;
            }
        }
    } else {
        printf("%s\n", "gofolder: wrong number of arguments");
        return -1;
    }
}
