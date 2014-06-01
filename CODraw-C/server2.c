#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include"lib/linkedlist.h"
#include <semaphore.h>

#define USERNAME_SIZE 20
#define MESSAGE_SIZE 2000

typedef struct
{
    int index;
    int sock;
    int numOfSocks;
    int messageSize;
    char *message;
}messageCall;

//Declaring the Thread Handler and Broadcast Functions 
void *connection_handler(void *);
void broadcast(int *socket, char *message, int messageSize);
void *sendToAll(void *myMessage);
Node *socket_list;

pthread_mutex_t send_mutex;       /*stops other threads from broadcasting when one is already doing it */
pthread_mutex_t broadcast_mutex;  /* to protect threads_sent, blocks when a thread is changing it */

/* semaphores for broadcasting control */
unsigned int threads_sent; /* count threads done sending the message and receiving acknowledgement*/
sem_t done_sending;        /* only unlocks after all broadcasted messages were sent or failed to be sent */
 
int main(int argc , char *argv[])
{ 
    //mutex and semaphore initialization:
    /* initialize mutex */
    printf("Server up!");
    pthread_mutex_init(&send_mutex, NULL);
    pthread_mutex_init(&broadcast_mutex, NULL);
 
    /* initialize semaphores */
    int res = sem_init(&done_sending,  /* pointer to semaphore */
                       0 ,             /* 0 if shared between threads, 1 if shared between processes */
                       0);             /* initial value for semaphore (0 is locked) */
    if (res < 0)
    {
        perror("Semaphore initialization failed");
        exit(0);
    }
    threads_sent = 0;
    //----

    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    //Creates Socket     
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could Not Create Socket");
    }
    printf("Socket Created");
    
    socket_list = newList();

    //Does Some Magic 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    //Stabilishes the Bind 
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind Failed. Error");
        return 1;
    }
    printf("Bind Done");
     
    listen(socket_desc , 3);
     
    //Waiting for Connection Request and Acceptance of Such Requests
    printf("Waiting for Incoming Connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        printf("Connection Accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if(pthread_create(&sniffer_thread, NULL,  connection_handler, (void*)new_sock) < 0)
        {
            perror("Could Not Create Thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL); <---Don't unccoment this! 
        printf("Handler Assigned");
    }
     
    if (client_sock < 0)
    {
        perror("Accept Failed");
        return 1;
    }
     
    return 0;
}

void *connection_handler(void *socket_desc)
{
    //Declaring Socket Descriptor and Some Other Stuff
    int sock = *(int*)socket_desc, read_size;
    char username[USERNAME_SIZE], server_message[MESSAGE_SIZE], client_message[MESSAGE_SIZE];
    User new_user;

    //First Message of Connection
    strcpy(server_message, "0 - Welcome to the Koi Fish Messenger! Enter your Username:\n");
    send(sock, server_message, strlen(server_message), 0);
    
    //Receives 
    if(read_size = recv(sock, username, USERNAME_SIZE, 0) > 0)
    {
        new_user.sock = sock;
        strcpy(new_user.name, username);
        insert(&socket_list, &new_user);
    }

    //Client Message Receiving
    while(1);
    {
        if(read_size = recv(sock, client_message, MESSAGE_SIZE, 0) > 0)
        {
            int messageSize = strlen(client_message);
            //Locks broadcasting for all other threads
            pthread_mutex_lock(&send_mutex);
            broadcast(&sock, &client_message[0], messageSize);//don't know if it returns after
                                                              //all messages were broadcast
                                                              //or after threads were created
            pthread_mutex_unlock(&send_mutex);
        }
        //To proceed, waits until all messages were sent
        sem_wait(&done_sending);
        //Blocks for when another client starts broadcasting
        sem_post(&done_sending);

        //Tells the client that his messages were successfully sent
        strcpy(server_message, "3 - All messages received\n");
        send(sock, server_message, strlen(server_message), 0);

        //Erases the Message Vectors
        memset(&server_message[0], 0, MESSAGE_SIZE);
        memset(&client_message[0], 0, MESSAGE_SIZE);

    }
}
// broadcast(&sock, &client_message[0], messageSize)
void broadcast(int *socket, char *message, int messageSize)
{  
    //Declaring the message element. The struct is defined up in the code
    
    
    int i;
    //for each socket in the socket list: create a thread
    for(i = getSize(socket_list); i > -1; i--)
    {
        //----------------------------------------------------------------------------------------------------------
        messageCall *myMessage;
        myMessage->message = message;
        myMessage->sock = *socket;
        myMessage->messageSize = messageSize;
        myMessage->numOfSocks = getSize(socket_list);
    
        myMessage->index = i;
        pthread_t broadcaster;
        //This function receives a copy of the struct because this struct changes for every for loop;
        pthread_create(&broadcaster, NULL,  sendToAll, (void*) myMessage); //<-----------------------
//----------------------------------------------------------------------------------------------------------------
        //Do we have to join threads? No because it would kill paralelism! (if I got it right, haha)
        //See this for an explanation: http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_join.html
        //pthread_join(&broadcaster, NULL);
    }
    
}


void *sendToAll(void *myMessage) //(messageCall myMessage)
{
    messageCall thisMessage = * (messageCall*) myMessage;
    
    //socket that will receive the message
    int socket = getSock(socket_list, thisMessage.index);
    //number of sockets to receive message. Sender socket doesn't receive
    int number = (thisMessage.numOfSocks) - 1;

    //if socket is the broadcaster, do nothing
    if(socket != thisMessage.sock)
    {
        //send message. Note that myMessage.message is a char*, don't know if will work
        //if not, we have to find a way to make it into a char [MESSAGE_SIZE]
        send(socket, thisMessage.message, thisMessage.messageSize, 0);
        /* 
        entering critical section with  mutex
        critical section to protect threads_sent
        */
        pthread_mutex_lock(&broadcast_mutex);//only proceeds if mutex is unlocked, locking it.
        //This is the last thread to send, it will unlock the done_sending semaphore;
        if (threads_sent == number-1)
        {
            printf("Done filling list. Lifting barrier for acknowledge sending.\n");
            sem_post(&done_sending); // sem_post increments semaphore. Unlocking it
            pthread_mutex_unlock(&broadcast_mutex);
        }
        //still has some messages to go. Increments to show its message is sent
        else
        {
            threads_sent++;
            pthread_mutex_unlock(&broadcast_mutex);
        }
    }
}