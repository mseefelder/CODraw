#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

#define USERNAME_SIZE 20;
#define MESSAGE_SIZE 2000;
 
void *connection_handler(void *);
void broadcast(int);
 
int main(int argc , char *argv[])
{ 
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created");
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Bind Failed. Error");
        return 1;
    }
    printf("Bind Done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    printf("Waiting for Incoming Connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        printf("Conexão Aceita");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("Could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        printf("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}

void *connection_handler(void *socket_desc)
{
    int sock = *(int*)socket_desc;
    char username[USERNAME_SIZE], server_message[MESSAGE_SIZE], client_message[MESSAGE_SIZE];

    strcpy(server_message, "Welcome to the CatSkin Messenger! Enter your Username.\n");
    send(sock, server_message, strlen(server_message), 0);

    while(1);
    {
        if(read_size = recv(sock, client_message, MESSAGE_SIZE, 0) > 0)
        {

        }

        memset(&server_message[0], 0, MESSAGE_SIZE);
        memset(&client_message[0], 0, MESSAGE_SIZE);

    }
}

void





















