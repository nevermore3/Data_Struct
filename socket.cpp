// service.cpp

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

using namespace std;

#define  BUFFER_SIZE  4
#define  MAX_QUEUE_NUM  5
#define  PORT   6000

#define  FILE_NAME_MAX  512
#define  SOCKADDR  sockaddr_in
#define  S_FAMILY  sin_family

#define  SERVER_AF   AF_INET

void select_loop(int server_s);
int process_requests(int server_s);


int main(int argc, char* argv[])
{
	cout<<"Max fd_set  "<<sizeof(fd_set)<<endl;
	int sockfd;
	int sin_size = sizeof(struct sockaddr);
	struct sockaddr_in  server_sockaddr, client_sockaddr;
	int i = 1;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cout<<"socket"<<endl;
		exit(1);
	}
	cout<<"Socket id = "<<sockfd<<endl;

	// set  sockaddr_in 's arg 
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero), 8);

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	// bind
	if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr)) == -1) {
		cout<<"bind"<<endl;
		exit(1);
	}
	cout<<"bind success!"<<endl;

	// listen
	if (listen(sockfd, MAX_QUEUE_NUM) == -1) {
		cout<<"listen"<<endl;
		exit(1);
	}
	cout<<"listening..."<<endl;

	select_loop(sockfd);
	return 0;
}

fd_set block_read_fdset;
int max_fd;

#define BOA_FD_SET(fd, where) \
	{ \
		FD_SET(fd, where); \
		if (fd > max_fd) { \
			max_fd = fd;\
		} \
	}

void select_loop(int server_s)
{
	FD_ZERO(&block_read_fdset);
	max_fd = server_s + 1;
	while(1) {
		BOA_FD_SET(server_s, &block_read_fdset);
		// no readable filedesc, bolck it
		if (select(max_fd+1, &block_read_fdset, NULL, NULL,NULL) == -1) {
			if (errno == EINTR) {
				continue;
			} else if (errno != EBADF) {
				cout<<"select"<<endl;
			}
		}
		if (FD_ISSET(server_s, &block_read_fdset)) {
			process_requests(server_s);
		}
	}
}

int process_requests(int server_s)
{
	int fd; //socket
	struct sockaddr_in  remote_addr; //address
	int remote_addrlen = sizeof(struct SOCKADDR);
	size_t len;
	char buff[BUFFER_SIZE];
	bzero(buff, BUFFER_SIZE);

	fd = accept(server_s, (struct sockaddr *)&remote_addr, (socklen_t *)&remote_addrlen);

	if (fd == -1) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			cout<<"accept"<<endl;
		}
		return -1;
	}
	int len1 = 0;


	len1 = recv(fd, buff, BUFFER_SIZE-1, 0);
	cout<<"len1 is :"<<len1<<endl;
	cout<<"Recvice From Client: "<<buff<<endl;

	//buff is small ,repeat recive it 
	/*  分包接收
	while(len1 = recv(fd, buff, BUFFER_SIZE-1, 0))
	{
		if (len1 < 0) {
			cout<<"error in recv"<<endl;
			exit(1);
		}
		cout<<"Recvice From Client: "<<string(buff)<<endl;
		memset(buff, 0, BUFFER_SIZE);
	}
	*/

	/*
	int bytes = read(fd, buff, BUFFER_SIZE);
	if (bytes < 0) {
		if (errno == EINTR) {
			bytes = 0;
		} else {
			return -1;
		}
	}
	*/
	//cout<<"Recvice From Client: "<<buff<<endl;
	return 0;
}

//client.cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <iostream>
#define PORT 6000
#define BUFFER_SIZE 4096
#define FILE_NAME_MAX 512

using namespace std;

int main(int argc, char* argv[])
{
	int sockfd;
	struct hostent *host;
	struct sockaddr_in serv_addr;

	if (argc != 2) {
		cout<<"Usage: ./client Hostname "<<endl;
		exit(1);
	}
	//address 解析
	if ((host = gethostbyname(argv[1])) == NULL) {
		cout<<"gethostbyname"<<endl;
		exit(1);
	}
	//create socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cout<<"socket"<<endl;
		exit(1);
	}
	bzero(&serv_addr, sizeof(serv_addr));
	//set sockaddr_in's arg
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr = *((struct in_addr*) host->h_addr); //get ip address
	bzero(&(serv_addr.sin_zero), 8);

	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		cout<<"connect"<<endl;
		exit(1);
	}

	char buff[BUFFER_SIZE] = "this is a message , i send to you ,pls accept it";
	int count;
	count = send(sockfd, buff, strlen(buff), 0);
	if (count < 0) {
		cout<<"send file info"<<endl;
		exit(1);
	}
	cout<<"Client Send ok Count = "<<count<<endl;
	return 0;
}
