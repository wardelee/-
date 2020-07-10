/*************************************************************************
	> File Name: udp_epoll.h
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Thu 09 Jul 2020 04:40:49 PM CST
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
extern int port;
extern struct User *rteam;
extern struct User *bteam;
extern int repollfd, bepollfd;

void add_to_sub_reactor(struct User *user);
int find_sub(struct User *team);
void del_event(int epollfd, int fd);
void add_event_ptr(int epollfd, int fd, int events, struct User *user);
int udp_accept(int fd, struct User *user);
int udp_connect(struct sockaddr_in *client);
int check_online(struct LogRequest *request);
void log_out(int signum);

#endif
