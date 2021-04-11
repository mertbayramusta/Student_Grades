#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#define SIZE sizeof(struct sockaddr_in)

int main() {
    int sockfd, nread;
    char buf[128], enter, resp;
    char username[30], password[100], grade[20];
    fd_set fds;
    char IP[20];
    struct sockaddr_in server = { AF_INET, 2000 };

    struct sockaddr_in cli;

    printf("\nEnter IP address of the Server\n");
    scanf("%s%c", IP, &enter);
    server.sin_addr.s_addr = inet_addr(IP);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Error creating SOCKET\n");
        return (0);
    }

    if (connect(sockfd, (struct sockaddr *) &server, SIZE) == -1) {
        printf("Connect failed\n");
        return (0);
    }
    connect(sockfd, (struct sockaddr *) &cli, sizeof cli);
    printf("Connected to the server. \n");

    for(;;) {
        

        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        FD_SET(0, &fds);

        printf("Username: \n");
        scanf("%s%c", username, &enter);
        printf("Password: \n");
        scanf("%s%c", password, &enter);

        send(sockfd, username, sizeof(username), 0);
        send(sockfd, password, sizeof(password), 0);
        recv(sockfd, grade, sizeof(grade), 0);
        if(strcmp(grade, "-1") == 0){
            printf("Error Code %s:\nThe username does not exists.\nPlease Try Again.\n", grade);

        }else if(strcmp(grade, "-2") == 0){
            printf("Error Code %s:\nYour password is incorrect.\nPlease Try Again.\n", grade);

        }else if(strcmp(grade, "Error occured!") == 0){
            printf("Error occured!\nPlease Try Again.\n");

        }else{
            printf("Your grade is %s: \n", grade);
            printf("Bye. \n");
            break;

        }
        
    }
    close(sockfd);
    return (0);
}
