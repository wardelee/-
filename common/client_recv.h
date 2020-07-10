/*************************************************************************
	> File Name: client_recv.h
	> Author: weiwenhu 
	> Mail: 1019162007@qq.com
	> Created Time: Fri 10 Jul 2020 03:17:10 PM CST
 ************************************************************************/

#ifndef _CLIENT_RECV_H
#define _CLIENT_RECV_H
pthread_t recv_t;
extern int sockfd;
void *do_recv(void *arg);
void recv_msg(int fd);
#endif
