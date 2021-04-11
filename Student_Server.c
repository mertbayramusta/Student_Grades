#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE sizeof(struct sockaddr_in)

typedef struct{
    char id[10];
    char password[100];
    char grade[10];
}Student;   //Struct for students


int main() {
    FILE *fptr;
    Student students[100];
    int sockfd, maxfd, nread, found, i, j;
    int k = 0, count = 0, index;
    char buf[128], buf2[128], buf3[30]; 
    int sock;
    int sent = 0;

    //For date time
    char datetime[25];
    time_t _now;
    struct tm *now;
    time(&_now);
    now = localtime(&_now);
    strftime(datetime, 25, "%Y-%m-%d %I:%M:%S %p", now);
    
    fd_set fds;
    struct sockaddr_in server = { AF_INET, 2000, INADDR_ANY };
    struct sockaddr_in cli;
    int len;

    //File Reading
    fptr = fopen("grades.txt", "r");
    if(!fptr){
        printf("Error occured while opening the file!\n");
    }
    while(!feof(fptr)){
        fscanf(fptr, "%s%s%s", students[k].id, students[k].password, students[k].grade);
        k++;
        count++;
    }
    fclose(fptr);
    
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Error creating SOCKET\n");
        return (0);
    }
    if (bind(sockfd, (struct sockaddr *) &server, SIZE) == -1) {
        printf("bind failed\n");
        return (0);
    }
    if (listen(sockfd, 10) == -1) {
        printf("listen failed\n");
        return (0);
    }

    for (;;) {
        len = sizeof(cli);
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        sock = accept(sockfd, (struct sockaddr*)&cli, &len); 
        recv(sock, buf, sizeof(buf), 0);
        recv(sock, buf2, sizeof(buf2), 0);
        for(index = 0; index < count; index++){

            if(strcmp(buf, students[index].id) == 0 && strcmp(buf2, students[index].password) == 0){
                send(sock,students[index].grade,sizeof(students[index].grade),0);
                printf("[%s] connection occured at %s \n", datetime, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                printf("[%s] replied grade for %s to %s \n", datetime, students[index].id, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                sent = 1;
                break;
            }else if((strcmp(buf, students[index].id) < 0 || strcmp(buf, students[index].id) > 0) && strcmp(buf2, students[index].password) == 0){
                strcpy(buf3, "-1");
                send(sock,buf3,sizeof(buf3),0);
                printf("[%s] connection occured at %s \n", datetime, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                printf("[%s] incorrect username %s received from %s \n", datetime, students[index].id, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                break;
            }else if(strcmp(buf, students[index].id) == 0 && (strcmp(buf2, students[index].password) < 0 || strcmp(buf2, students[index].password) > 0)){
                strcpy(buf3, "-2");
                send(sock,buf3,sizeof(buf3),0);
                printf("[%s] connection occured at %s \n", datetime, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                printf("[%s] incorrect password %s for %s from %s \n", datetime, students[index].password, students[index].id, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                break;
            }else{
                strcpy(buf3, "Error occured!");
                send(sock,buf3,sizeof(buf3),0);
                printf("[%s] some error occured from %s \n", datetime, inet_ntop(AF_INET, &cli.sin_addr,buf,sizeof(buf)));
                break;
            }

        }
        if(sent == 1){
            break;
        }

    }
}
