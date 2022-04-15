#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "question.h"
#define MAX_LINE_LENGHT 256
#define PORT 8080
#define SA struct sockaddr
#define USERS_FILE "usersResults.txt"
#define QUESTIONS_FILE "questions.txt"
#define QUESTIONS_NUMBER 5
#define USERNAME_MAX_LENGHT 30

Question questions[QUESTIONS_NUMBER];
void loadQuestions();
int saveResult(char username[], int result);
int checkUser(char username[30]);

// Function designed for communication between client and server.
void communicate(int connfd)
{

    char buff[MAX_LINE_LENGHT], username[USERNAME_MAX_LENGHT], answer;
    int n, check, result = 0;

    //Read username from client
    read(connfd, username, sizeof(username));
    
    //check if username exists
    check = checkUser(username);
    if(check == 1){
        check = 1;
        write(connfd, &check, sizeof(int));
        bzero(buff, MAX_LINE_LENGHT);
        strcpy(buff, "You have done this test yet! If you do it again, your result will not be changed! Do you want to continue? [y/n]");
        write(connfd, buff, sizeof(buff));
        bzero(buff, MAX_LINE_LENGHT);
        read(connfd, &answer, sizeof(char));
        if(answer == 'n'){ 
            return;
        }
    }else if(check == -1){
        return;
    }else{
        check = 0;
        write(connfd, &check, sizeof(int));
    }

    //load the questions
    loadQuestions();

    //sends questions and read answers
    for(int i=0; i<QUESTIONS_NUMBER; i++){
        write(connfd, &questions[i], sizeof(Question));

        bzero(buff, MAX_LINE_LENGHT);
        read(connfd, &answer, sizeof(char));
        if(answer == questions[i].correct) result += 2;
    }

    //send result to the user
    write(connfd, &result, sizeof(int));
    
    //save the result if user doesn`t exist
    if(check == 0){
        saveResult(username, result);
    }
}
   
// Driver function
int main()
{
    int sockfd, connfd, len;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    // Function for communicating between client and server
    communicate(connfd);
   
    //close the socket
    close(sockfd);
}

//function to check if the user exists
int checkUser(char username[30]){
    FILE *fp;
    char line[30];
    if((fp=fopen(USERS_FILE, "rw"))==NULL){
        printf("Couldnt open file with users!\n"); 
        return -1;
    }
    while (fscanf(fp, "%[^\n] ", line) != EOF) {
        if(strcmp(line, username) == 0){
            fclose(fp);
            return 1;
        }
    }
    return 0;
    fclose(fp);
}

//function to load the questions from file into array
void loadQuestions(){
    FILE *fp;
    char line[256];
    if((fp=fopen(QUESTIONS_FILE, "r"))==NULL){
        printf("Couldnt open file with users!\n"); 
        exit(1);
    }
    for(int i=0; i<QUESTIONS_NUMBER; i++){
        fgets(questions[i].task, MAX_LINE_LENGHT, fp);
        fgets(questions[i].optionA, MAX_LINE_LENGHT, fp);
        fgets(questions[i].optionB, MAX_LINE_LENGHT, fp);
        fgets(questions[i].optionC, MAX_LINE_LENGHT, fp);
        fgets(questions[i].optionD, MAX_LINE_LENGHT, fp);
        questions[i].correct = fgetc(fp);
        fgetc(fp);
    }
    fclose(fp);
}

//function to save the result into file
int saveResult(char username[], int result){
    FILE* fp;
    if((fp = fopen(USERS_FILE, "a+")) == NULL){
        printf("Couldn`t open results file\n");
        return -1;
    }
    fprintf(fp, "%s\n%d\n", username, result);
    fclose(fp);
    return 1;
}