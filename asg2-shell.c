/*
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


/*Global Variable*/
char currentDirectory[1024] = "";


/* Function Declaration */
int getUserInput(char *input);

void tokenizeInput(char *input);

void gofolder(char *args0[255],int argSize);

/* Functions */
int main(int argc, char *argv[]) {
    int isExit = 0;

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
    return 0;
}

/*
  tokenizeInput()
  - Tokenize the string stored in parameter, delimiter is space
  - In given code, it only prints out the token.
  Please implement the remaining part.
*/

void tokenizeInput(char *input) {
    char buf[255];
    strcpy(buf, input);
    char *tokens[255];

    char *token = strtok(buf, " ");
    int argSize = 0;
    while (token != NULL) {
        tokens[argSize] = token;
        //    printf("%s\n", tokens[count]);
        token = strtok(NULL, " ");
        argSize++;
    }




    if (strcmp(tokens[0], "gofolder") == 0) {
        gofolder(tokens,argSize);
    } else {

    }

    return;
}

void gofolder(char *args[255],int argSize) {
    if (args[1] != NULL) {
        if (args[1][0] == '\0' ||argSize>2) {
            printf("%s\n", "gofolder: wrong number of arguments");
            return;
        } else{
            DIR* dir = opendir(args[1]);
            if (dir)
            {
                /* Directory exists. */
                closedir(dir);
                chdir(args[1]);
                getcwd(currentDirectory, sizeof(currentDirectory));
            }
            else if (ENOENT == errno)
            {
                /* Directory does not exist. */
                printf("{%s}: cannot change directory\n",args[1]);
            }
        }
    } else{
        printf("%s\n", "gofolder: wrong number of arguments");
        return;
    }
}
