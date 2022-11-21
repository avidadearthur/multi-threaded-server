/**
 * \author Arthur Tavares Quintao
 */

#include <string.h>
#include <unistd.h>
#include "messenger.h"
#include "receiver.h"

int main(void){
    char message[BUFSIZ];

    spawn_logger();
    strcpy(message, "Greetings");
    send_message(message);
    strcpy(message, "Hello");
    send_message(message);
    strcpy(message, "Hello my name is Arthur and I am a student at the University of Leuven");
    send_message(message);
    strcpy(message, "That's it, my last message");
    send_message(message);
    kill_child();

    return 0;
}