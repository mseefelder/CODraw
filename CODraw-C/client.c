/*
    C ECHO client example using sockets
*/
//#include<iostream> //printf
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    int sock;
    int msgtype;
    int size;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");

    msgtype = 0;
    while(msgtype != 2)
    {
        size = recv(sock , server_reply , 2000 , 0);
        printf("OK %d\n", size);
        if(size > 0)
        {
            printf("OK msg\n");
            if(server_reply[0] == '1' || server_reply[0] == '2')
            {
                printf("\n SERVER: %s\n", server_reply);
                msgtype++;
            }
        }
    }
    
    //keep communicating with server
    while(1)
    {


        printf("Enter message: ");
        scanf("%s" , message);
         
        //Send some data
        if(send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        int message_size = (int) sizeof(message);
        printf("%d\n", message_size);

        //Receive a reply from the server
        size = recv(sock , server_reply , 2000 , 0);
        printf("OK %d\n", size);
        if(size < 0)
        {
            puts("recv failed");
            break;
        }
        
        server_reply[size] = 0;
        printf("\nServer Reply: %s\n", server_reply);
        memset(&message[0], 0, sizeof(message));
        memset(&server_reply[0], 0, sizeof(server_reply));
    }
     
    close(sock);
    return 0;
}