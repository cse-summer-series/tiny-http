#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 65536 /* larger = more efficient */
#define VERSION 25


void web(int fd) {
   	static char buffer[BUFSIZE+1]; /* static so zero filled */
   	static char outbuffer[BUFSIZE+1]; /* static so zero filled */

   	long i, ret, len;

	ret = read(fd,buffer,BUFSIZE); 	/* read Web request in one go */
	if(ret == 0 || ret == -1) {	/* read failure stop now */
		exit(105);
	}
	if(ret > 0 && ret < BUFSIZE)	/* return code is valid chars */
		buffer[ret]=0;		/* terminate the buffer */
	else buffer[0]=0;
	if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) ) {
		exit(106);
	}
	for(i=4;i<BUFSIZE;i++) { /* null terminate after the second space to ignore extra stuff */
		if(buffer[i] == ' ') { /* string is "GET URL " +lots of other stuff */
			buffer[i] = 0;
			break;
		}
	}
	len = strlen(buffer);
	(void)sprintf(outbuffer,"HTTP/1.1 200 OK\nServer: tinyweb\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", len, "text/plain"); /* Header + a blank line */
	(void)write(fd,outbuffer,strlen(outbuffer));
	(void)write(fd,buffer,strlen(buffer));
	sleep(1);
	close(fd);
	return;
}

int main(int argc, char **argv) {
    int listenfd, socketfd;
    socklen_t length;
    static struct sockaddr_in cli_addr;
    static struct sockaddr_in serv_addr;
    
   	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)
        exit(101);
    
   	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8080);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0)
		exit(102);
	if( listen(listenfd,64) <0)
		exit(103);

	while(1) {
	    length = sizeof(cli_addr);
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0)
		    exit(104);
		//close(listenfd);
		web(socketfd);
		close(socketfd);
	}
    
}
