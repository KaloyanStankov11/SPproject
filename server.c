#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 256
#define PORT 8080
#define SA struct sockaddr
#define usersFile "usersResults.txt"
#define questionsFile "questions.txt"
#define questsNum 5
#define MAX_LINE_LENGTH 30

typedef struct User{
    char username[30];
    int result;
}User;

typedef struct Question{
    char task[MAX];
    char optionA[MAX];
    char optionB[MAX];
    char optionC[MAX];
    char optionD[MAX];
    char correct;
}Question;
Question questions[questsNum];
void loadQuestions();
void saveResult(char username[], int result);
int checkUser(char username[30]);
// Function designed for chat between client and server.
void func(int connfd)
{

    char buff[MAX], username[30], answer;
    int n, check, result = 0;
    loadQuestions();
    printf("%s\n", questions[0].task);
    // infinite loop for chat
    //for (;;) {
        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
        read(connfd, username, sizeof(username));
        // print buffer which contains the client contents
        if(checkUser(username) == 1){
            check = 1;
            write(connfd, &check, sizeof(int));
            bzero(buff, MAX);
            strcpy(buff, "You have done this test yet! If you do it again, your result will not be changed! Do you want to continue? [y/n]");
            write(connfd, buff, sizeof(buff));
            bzero(buff, MAX);
            read(connfd, &answer, sizeof(char));
            if(answer == 'n'){ 
                return;
            }
        }else{
            check = 0;
            write(connfd, &check, sizeof(int));
        } 
        loadQuestions();
        for(int i=0; i<questsNum; i++){
            bzero(buff, MAX);
            strcpy(buff, questions[i].task);
            write(connfd, buff, sizeof(buff));

            bzero(buff, MAX);
            strcpy(buff, questions[i].optionA);
            write(connfd, buff, sizeof(buff));

            bzero(buff, MAX);
            strcpy(buff, questions[i].optionB);
            write(connfd, buff, sizeof(buff));

            bzero(buff, MAX);
            strcpy(buff, questions[i].optionC);
            write(connfd, buff, sizeof(buff));

            bzero(buff, MAX);
            strcpy(buff, questions[i].optionD);
            write(connfd, buff, sizeof(buff));

            bzero(buff, MAX);
            read(connfd, &answer, sizeof(char));
            if(answer == questions[i].correct) result += 2;
        }
        write(connfd, &result, sizeof(int));
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
   
    // Function for chatting between client and server
    func(connfd);
   
    // After chatting close the socket
    close(sockfd);
}

int checkUser(char username[30]){
    FILE *fp;
    char line[30];
    if((fp=fopen(usersFile, "rw"))==NULL){
        printf("Couldnt open file with users!\n"); 
        exit(1);
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

void loadQuestions(){
    FILE *fp;
    char line[256];
    if((fp=fopen(questionsFile, "r"))==NULL){
        printf("Couldnt open file with users!\n"); 
        exit(1);
    }
    for(int i=0; i<questsNum; i++){
        fgets(questions[i].task, MAX, fp);
        fgets(questions[i].optionA, MAX, fp);
        fgets(questions[i].optionB, MAX, fp);
        fgets(questions[i].optionC, MAX, fp);
        fgets(questions[i].optionD, MAX, fp);
        questions[i].correct = fgetc(fp);
        fgetc(fp);
    }
    fclose(fp);
}

void saveResult(char username[], int result){
    FILE* fp;
            if((fp = fopen(usersFile, "a+")) == NULL){
                printf("Couldn`t open results file\n");
                exit(1);
            }
            fprintf(fp, "%s\n%d\n", username, result);
            fclose(fp);
}