/**
 * \author Arthur Tavares Quintao
 */


#include <stdio.h>
#include<unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include "messenger.h"
#include "receiver.h"

#define READ_END 0
#define WRITE_END 1

// global vars
pid_t pid;
int fd[2];


int main(void){

    char wmsg[] = "Greetings";
    send_message(wmsg);
    receive_message();

    return 0;
}