/*
    C ECHO client example using sockets
*/
//#include<iostream> //printf
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#define MESSAGESIZE 2000

typedef enum{False,True} mybool; 
 
int main(int argc , char *argv[])
{
    mybool ableto_write = False;
    mybool message_sent = False;  //new!
    int sock;
    int msgtype;
    int size;
    struct sockaddr_in server;
    char message[MESSAGESIZE], draft[MESSAGESIZE] , server_reply[MESSAGESIZE];
    
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
    while(msgtype != 1)
    {
        size = recv(sock , server_reply , MESSAGESIZE , 0);
        printf("OK %d\n", size);
        if(size > 0)
        {
            printf("In if");
            if(server_reply[0] == '0')
            {
                printf("\n SERVER: %s\n", server_reply); //server greetings, asking for user's nickname
                msgtype = 1;
                printf("Entering message...");
                //scanf("%s" , message);

                fgets( draft, sizeof(draft), stdin );
                sscanf(draft, "%s", message );

                printf("Message in!");
                if(send(sock , message , strlen(message) , 0) < 0)   //user sending nickname to server
                {
                    puts("Send failed");
                    return 1;
                }
                printf("Send successfull!");
            }
        }
        memset(&message[0], 0, sizeof(message));
    }    

    
    //keep communicating with server
    while(1)
    {

        if (ableto_write)
        {
            printf("Enter message: ");
            scanf("%s" , message);
         

        //Send some data
            if(send(sock , message , strlen(message) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
        
            //int message_size = (int) sizeof(message);
            //printf("%d\n", message_size);
        }    

        //Receive a reply from the server
        size = recv(sock , server_reply , MESSAGESIZE , 0);    //server reply could be anything, as long it verifies the server-client conection
        //printf("OK %d\n", size);  //not needed
        if(size < 0)
        {
            puts("recv failed");
            break;
        }
        else
        { 
            if(server_reply[0] == '1')  //send acknowledge, server received client's message  (server_reply = 1) ~ou algo parecido
            {
                message_sent = True;
            }
            else if (server_reply[0] == '2' && message_sent != True) //broadcast
            {
                printf("%s\n", server_reply);  // (server_reply = 2 - userN: message)   [this chat can only send once everybody received las sent]
                strcpy(message, "ack");                                             //  [message. Therefore server_reply contains only one message]
                if(send(sock , message , strlen(message) , 0) < 0) //sends acknowledge to server
                {
                    puts("Could not reply to server");
                    return 1;
                }
            }
            else if (server_reply[0] == '3')  // (server_reply = 3) server received all acknowledges
            {
                ableto_write = True;
            }
            else if (server_reply[0] == '4')  // (server_reply = 4) server is yet to received all acknowledges
            {
                ableto_write = False;          
            }

        }
        message_sent = False;
        server_reply[size] = 0;
        printf("\nServer Reply: %s\n", server_reply);
        memset(&message[0], 0, sizeof(message));
        memset(&server_reply[0], 0, sizeof(server_reply));
    }
     
    close(sock);
    return 0;
    
}
    