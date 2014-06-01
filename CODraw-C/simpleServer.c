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

void *connection_handler(void *);
Node *socket_list;

int main(int argc , char *argv[])
{ 
    //mutex and semaphore initialization:
    /* initialize mutex */
    //printf("Server up!");
    //pthread_mutex_init(&send_mutex, NULL);
    //pthread_mutex_init(&broadcast_mutex, NULL);
 
    /* initialize semaphores */
    // int res = sem_init(&done_sending,  /* pointer to semaphore */
    //                    0 ,             /* 0 if shared between threads, 1 if shared between processes */
    //                    0);             /* initial value for semaphore (0 is locked) */
    // if (res < 0)
    // {
    //     perror("Semaphore initialization failed");
    //     exit(0);
    // }
    // threads_sent = 0;
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
    printf("Username added.");

    //Client Message Receiving
    while(1);
    {
        if(read_size = recv(sock, client_message, MESSAGE_SIZE, 0) > 0)
        {
            printf("%s",client_message);
            strcpy(server_message, "3 - All messages received\n");
            send(sock, server_message, strlen(server_message), 0);

            /*int messageSize = strlen(client_message);
            //Locks broadcasting for all other threads
            pthread_mutex_lock(&send_mutex);
            broadcast(&sock, &client_message[0], messageSize);//don't know if it returns after
                                                              //all messages were broadcast
                                                              //or after threads were created
            pthread_mutex_unlock(&send_mutex);*/
        }
        ////To proceed, waits until all messages were sent
        //sem_wait(&done_sending);
        ////Blocks for when another client starts broadcasting
        //sem_post(&done_sending);

        ////Tells the client that his messages were successfully sent
        // strcpy(server_message, "3 - All messages received\n");
        // send(sock, server_message, strlen(server_message), 0);

        //Erases the Message Vectors
        memset(&server_message[0], 0, MESSAGE_SIZE);
        memset(&client_message[0], 0, MESSAGE_SIZE);

    }
}