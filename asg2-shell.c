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


/*Global Variable*/
char currentDirectory[1024]="";
char *tokens[255];


/* Function Declaration */
int getUserInput(char *input);

void tokenizeInput(char *input);

void gofolder(char *pString[255]);

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

    char *token = strtok(buf, " ");
    int count=0;
    while (token != NULL) {
        tokens[count]= token;
    //    printf("%s\n", tokens[count]);
        token = strtok(NULL, " ");
        count++;
    }
    int i;

    if(strcmp(tokens[0],"gofolder")==0){
        gofolder(tokens);
    }else{

    }

    return;
}

void gofolder(char *args[255]) {
//    if(args[1][0]=='\0'){
//        printf("%d","gofolder: wrong number of arguments\n");
//    }
}
