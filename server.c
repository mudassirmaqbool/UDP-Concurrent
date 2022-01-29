// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 50000 
#define MAXLINE 1024 

// Driver code 
int main() { 
	int sockfd; 
	int newsockfd = 0;
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr, new_servaddr;
	int starting_port = PORT + 1; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	while(1){

		int len = sizeof(struct sockaddr), n; 
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
		buffer[n] = '\0';
		
		// Creating socket file descriptor 
		if ( (newsockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
			perror("socket creation failed"); 
			exit(EXIT_FAILURE); 
		} 

		// Filling server information 
		new_servaddr.sin_family = AF_INET; // IPv4 
		new_servaddr.sin_addr.s_addr = INADDR_ANY; 
		new_servaddr.sin_port = htons(starting_port); 

		// Bind the socket with the server address 
		if ( bind(newsockfd, (const struct sockaddr *)&new_servaddr, sizeof(new_servaddr)) < 0 ) 
		{ 
			perror("bind failed"); 
			exit(EXIT_FAILURE); 
		} 

		int pid = fork();

		if(pid == -1)
			exit(-1);

		else if(pid == 0){

			close(sockfd);
			
			memset(buffer, '\0', MAXLINE);
			n = recvfrom(newsockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
			buffer[n] = '\0';

			printf("Client : %s\n", buffer); 
			sendto(newsockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &cliaddr, len); 
			printf("Hello message sent.\n"); 

			close(newsockfd);
			exit(1);

		}
		else {
			close(newsockfd);
			memset(buffer, '\0', MAXLINE);
			sprintf(buffer, "%d", starting_port);
			printf("%s\n", buffer);
			sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));
			starting_port++;

		}

	}
	return 0; 
} 

