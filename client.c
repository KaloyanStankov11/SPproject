#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "question.h"
#define MAX 256
#define PORT 8080
#define SA struct sockaddr
#define USERNAME_MAX_LENGHT 30
#define QUESTIONS_NUMBER 5

Question questions[QUESTIONS_NUMBER];

int checkUsernameInput(char username[]);

void communicate(int sockfd)
{
    char buff[MAX], username[USERNAME_MAX_LENGHT], answer;
    int n, check, result;
    while(1){
        printf("Enter username: ");
        fgets(username, USERNAME_MAX_LENGHT, stdin);
        size_t len = strlen(username);
        if (len > 0 && username[len-1] == '\n') {
            username[--len] = '\0';
        }
        if(checkUsernameInput(username) == -1){
            printf("Invalid username! Try again!\n");
        }else{
            break;
        }
    }
    write(sockfd, username, sizeof(username));
    read(sockfd, &check, sizeof(int));
    if(check == 1){
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s\n", buff);
        while(1){
            scanf(" %c", &answer);
            if((answer != 'n') && (answer != 'y')){
                printf("Invalid input! Try again!\n");
            }else if(answer == 'n'){
                write(sockfd, &answer, sizeof(char));
                return;
            }else{
                break;
            }
        }
        write(sockfd, &answer, sizeof(char));
    }
    for(int i=0; i<QUESTIONS_NUMBER; i++){
        printf("================================================================================================================\n");
        read(sockfd, &questions[i], sizeof(Question));
        printf("Question %d: %s", i+1, questions[i].task);
        printf("A: %s", questions[i].optionA);
        printf("B: %s", questions[i].optionB);
        printf("C: %s", questions[i].optionC);
        printf("D: %s", questions[i].optionD);

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
    printf("Test is done!\nYour result is: %d/%d\n", result, QUESTIONS_NUMBER*2);
    
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
   
    // function for communication
    communicate(sockfd);
   
    // close the socket
    close(sockfd);
}

int checkUsernameInput(char username[]){
    if((username[0]<65) || ((username[0]>90) && (username[0]<97)) || (username[0]>122) || (strlen(username)<3) || (strlen(username)>29))
        return -1;
    else return 1;
}