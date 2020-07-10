/*************************************************************************
	> File Name: client_recv.c
	> Author: weiwenhu 
	> Mail: 1019162007@qq.com
	> Created Time: Fri 10 Jul 2020 03:17:15 PM CST
 ************************************************************************/

#include <head.h>

pthread_t recv_t;
extern int sockfd;

void *do_recv(void *arg)
{
    struct ChatMsg msg;
    bzero(&msg, sizeof(msg));
    recv(sockfd, (void *)&msg, sizeof(msg), 0);
    if (strcmp(msg.msg, "Chat Room closed!") == 0) {
        printf(L_RED"Server msg"NONE" : %s\n", msg.msg);
        exit(1);
    } else {
        printf("<"YELLOW"%s"NONE"> ~ %s\n", msg.name, msg.msg);
    }
}

void recv_msg(int fd)
{
    pthread_create(&recv_t, NULL, do_recv, NULL);
}
