#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>
#include "DeltaCon.h"

#define MAX_CLIENT 1

typedef struct{
        int sock;
        pthread_t ClientTask;
        int8_t ClientEnd;
}DeltaC;


void error(char *msg);
void ExitApplication(int32_t Signal);
void *vfnThread(void* newsockfd);

volatile uint8_t ExitAppFlag;
DeltaC tClient;

int main(int argc, char *argv[]){

int sockfd, newsockfd, portno;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;

tClient.ClientEnd = 0;


   (void)signal(SIGINT,ExitApplication);
    printf("Application started and signal callback registered\n\r");
    printf("Press CTRL+C to finish\n\r");

    if (argc < 2){
      fprintf(stderr,"ERROR, no port provided\n");
      exit(1);
    }	

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
      error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
       error("ERROR on binding");
    }

    listen(sockfd,-1);
    clilen = sizeof(cli_addr);
    
   while(1){
          
           newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
           if(newsockfd < 0){
             error("ERROR on accept");
           }

	if(tClient.ClientEnd == 0){
            pthread_cancel(tClient.ClientTask);
	   // write(*(int*)newsockfd,ServerMess,1);
	   

	   tClient.ClientEnd = 1;
	   printf("%d\n",tClient.ClientEnd);	
	   tClient.sock = newsockfd;
	   
	   pthread_create(
	      &tClient.ClientTask,
	      NULL,
	      vfnThread,
	      &tClient.sock
	   );
	}else{
	   printf("%d",tClient.ClientEnd);
	 //  write(*(int*)newsockfd,"Robot busy sorry try later",26);	  
 	   close(newsockfd);	
	}


       
        if(ExitAppFlag == 1){
	    printf("Ending Application...\n\r");
	    pthread_cancel(tClient.ClientTask);
	    pthread_exit(vfnThread); 
	    close(sockfd);
            break;
        }
    }
return 0;
}

void *vfnThread(void* newsockfd)
{
    char buffer[256];
    char ReplayBuffer[256];
    int RWStatus;
    
    while(1){
        bzero(buffer,256);
        RWStatus = read(*(int*)newsockfd,buffer,5);
        if(RWStatus < 0){
           error("ERROR reading from socket");
        }else if(RWStatus != 0){
           printf("Here is the message: %d , %d , %d , %d, %d \n",
		buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

           DeltaControl(buffer,ReplayBuffer);
           RWStatus = write(*(int*)newsockfd,ReplayBuffer,255);
        }
	printf("%d",RWStatus);
	if(RWStatus == 0){
           close(tClient.sock);
           tClient.ClientEnd = 0;
           pthread_exit(NULL);
        }
    }

}

void ExitApplication(int32_t Signal){

    ExitAppFlag = 1;

}


void error(char *msg){
    perror(msg);
    exit(1);
}
