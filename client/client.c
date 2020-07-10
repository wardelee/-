/*************************************************************************
	> File Name: client.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Wed 08 Jul 2020 04:32:12 PM CST
 ************************************************************************/

#include "head.h"

int server_port = 0;
char server_ip[20] = {0};
char *conf = "./football.conf";
int sockfd = -1;

void logout(int signum)
{
    struct ChatMsg msg;
    msg.type = CHAT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    //send logout msg to all clients in the chat room

    close(sockfd);
    printf(L_GREEN"\nLogout! Bye!\n"NONE);
    exit(1);
}

int main(int argc, char **argv)
{
    int opt;
    struct LogRequest request;
    struct LogResponse response;
    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));

    while ((opt = getopt(argc, argv, "h:p:t:m:n:")) != -1) {
        switch (opt) {
            case 't':
                request.team = atoi(optarg);
                break;
            case 'h':
                strcpy(server_ip, optarg);
                break;
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'm':
                strcpy(request.msg, optarg);
                break;
            case 'n':
                strcpy(request.name, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s [-hptmn]!\n", argv[0]);
                exit(1);
        }
    }
    

    if (!server_port) {
        server_port = atoi(get_conf_value(conf, "SERVERPORT"));
    }
    if (!request.team) {
        request.team = atoi(get_conf_value(conf, "TEAM"));
    }
    if (!strlen(server_ip)) {
        strcpy(server_ip, get_conf_value(conf, "SERVERIP"));
    }
    if (!strlen(request.name)) {
        strcpy(request.name, get_conf_value(conf, "NAME"));
    }
    if (!strlen(request.msg)) {
        strcpy(request.msg, get_conf_value(conf, "LOGMSG"));
    }

    DBG(" <"RED"IP"NONE"> : %s\n",server_ip);
    DBG("<"L_RED"PORT"NONE"> : %d\n", server_port);
    DBG("<"BLUE"NAME"NONE"> : %s\n", request.name);
    DBG("<"GREEN"TEAM"NONE"> : %s\n", request.team ? "BLUE" : "RED");
    DBG("<"YELLOW"LOGMSG"NONE"> : %s\n", request.msg);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    socklen_t len = sizeof(server);

    if ((sockfd = socket_udp()) < 0) {
        perror("socket_udp()");
        exit(1);
    }
    
    sendto(sockfd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&server, len);
    
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    int ret_val;
    if ((ret_val = select(sockfd + 1, &rfds, NULL, NULL, &tv)) < 0) {
        perror("select()");
        exit(1);
    } else if (ret_val > 0) {
        int ret = recvfrom(sockfd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&server, &len);
        if (ret != sizeof(response) || response.type == 1) {
            DBG(RED"Error"NONE" : The Game Server refused your login.\n\tThis may be helpful: %s\n", response.msg);
            exit(2);
        }
    } else {
        DBG(RED"Error"NONE"The Game Server is out of service!\n");
        exit(1);
    }86
    DBG(GREEN"Server"NONE" : %s\n", response.msg);

    int retval;
    if ((retval = connect(sockfd, (struct sockaddr *)&server, len)) < 0) {
        perror("connect()");
        exit(1);
    }

    //char buff[512] = {0}4-;
    //sprintf(buff, "芜湖, 上电视!");
    //send(sockfd, buff, strlen(buff), 0);
    //bzero(buff, sizeof(buff));
    //recv(sockfd, buff, sizeof(buff), 0);
    //DBG(RED"Server Info"NONE" : %s\n", buff);

    pthread_t recv_t;
    pthread_create(&recv_t, NULL, do_recv, NULL);

    signal(SIGINT, logout);
    struct ChatMsg msg;
    while (1) {
        bzero(&msg, sizeof(msg));
        msg.type = CHAT_WALL;
        printf(RED"Please Input : \n"NONE);
        scanf("%[^\n]s", msg.msg);
        getchar();
        if (strlen(msg.msg)) {
            if (msg.msg[0] == '@') {
                msg.type = CHAT_MSG;
            } else if (msg.msg[0] == '#') {
                msg.type = CHAT_FUNC;
            }
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        }
    }
    return 0;
}
