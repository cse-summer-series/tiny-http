#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFSIZE 65536 /* larger = more efficient */
#define VERSION 25

int main(int argc, char **argv) {
    int socketfd;
    socklen_t length;
    static struct sockaddr_in cli_addr;
    static struct sockaddr_in serv_addr;
    static struct addrinfo* address;

    int addr_result = getaddrinfo("www.example.com", "80", NULL, &address);
    if(addr_result) {
        printf("Error with getaddrinfo: %d", addr_result);
        exit(1);
    }
    
   	if((socketfd = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) <0) {
        printf("Socket error: %d", socketfd);
        exit(101);
    }
    
    
    if((connect(socketfd, address->ai_addr, address->ai_addrlen))) {
        printf("Connect error: %d\n", errno);
        printf("addrlen: %d\n", address->ai_addrlen);
        
        printf("%s\n", strerror(errno));
        exit(102);
    }
    
    freeaddrinfo(address);
    
    char header[] = "GET /index.html HTTP/1.1\r\nHOST: www.example.com\r\n\r\n";
    
    send(socketfd, header, strlen(header), 0);
    
    char buf[1000];
    int byte_count = recv(socketfd, buf, sizeof buf, 0);
    buf[byte_count] = 0;
    printf("%d %s", byte_count, buf);

    close(socketfd);
    
    
}
