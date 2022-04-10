#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 256
#define PORT 8080
#define SA struct sockaddr
#define questsNum 5
void func(int sockfd)
{
    char buff[MAX], username[30], answer;
    int n, check, result;
    printf("Enter username: ");
    scanf("%s", username);
    write(sockfd, username, sizeof(username));
    read(sockfd, &check, sizeof(int));
    if(check == 1){
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s\n", buff);
        bzero(buff, sizeof(buff));
        scanf("%s", buff);
        write(sockfd, buff, sizeof(buff));
    }
    for(int i=0; i<questsNum; i++){
        printf("================================================================================================================\n");
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Question %d: %s", i+1, buff);

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("A: %s", buff);

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("B: %s", buff);

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("C: %s", buff);

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("D: %s", buff);

        while(1){
            printf("Your answer(Upper case only): ");
            scanf(" %c", &answer);
            if((answer == 'A') || (answer == 'B') || (answer == 'C') || (answer == 'D')){
                break;
            }else{
                printf("Invalid input! Try again!\n");
            }
        }
        write(sockfd, &answer, sizeof(char));
    }
    read(sockfd, &result, sizeof(int));
    printf("================================================================================================================\n");
    printf("Test is done!\nYour result is: %d/%d\n", result, questsNum*2);
    
}
   
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
    // function for chat
    func(sockfd);
   
    // close the socket
    close(sockfd);
}