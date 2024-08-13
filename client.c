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

    static struct addrinfo hints;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    
    int addr_result = getaddrinfo("localhost", "8080", &hints, &address);
    if(addr_result) {
        printf("Error with getaddrinfo: %d", addr_result);
        exit(1);
    }
    
    struct addrinfo* res;
    char addr_buf[64];
    for(res = address; res != NULL; res = res->ai_next) {   
        inet_ntop(AF_INET, &((struct sockaddr_in *)res->ai_addr)->sin_addr, addr_buf, sizeof(addr_buf));
        printf("hostname: %s\n", addr_buf); 
    } 
    
   	if((socketfd = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) <0) {
        printf("Socket error: %d", socketfd);
        exit(101);
    }
 
    if(connect(socketfd, address->ai_addr, address->ai_addrlen) < 0) {
        printf("Connect error: %d\n", errno);
        printf("addrlen: %d\n", address->ai_addrlen);
        
        printf("%s\n", strerror(errno));
        exit(102);
    }
    
    char header[] = "GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
    
    int send_return = send(socketfd, header, strlen(header), 0);
    printf("%d %ld\n", send_return, strlen(header));
    
    char buf[1000];
    int byte_count = recv(socketfd, buf, sizeof buf - 1, 0);
    buf[byte_count] = 0;
    printf("%d %s", byte_count, buf);

    close(socketfd);
    
    freeaddrinfo(address);
    
    
}
