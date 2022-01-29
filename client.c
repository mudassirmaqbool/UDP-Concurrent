// Client side implementation of UDP client-server model 
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
	char buffer[MAXLINE]; 
	char hello[MAXLINE]; 
	char * request = "Connection request";
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	

	int n, len = sizeof(struct sockaddr); 

	memset(buffer, '\0', MAXLINE);
	memset(hello, '\0', MAXLINE);



	sendto(sockfd, (const char *)request, strlen(request), 0, (const struct sockaddr *) &servaddr, sizeof(struct sockaddr)); 
	printf("Hello message sent.\n");

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len); 
	buffer[n] = '\0';

	servaddr.sin_port = htons(atoi(buffer)); 

	printf("Enter message: ");

	scanf("%s", hello);
	
	sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	printf("Hello message sent.\n"); 

	memset(buffer, '\0', MAXLINE);
		
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len); 
	buffer[n] = '\0'; 
	printf("Server : %s\n", buffer); 
	
	
	close(sockfd); 
	return 0; 
} 

