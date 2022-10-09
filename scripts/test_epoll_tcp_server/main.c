/* author: wenfh2020/2021.06.18
 * desc:   epoll test code, test tcp ipv4 async's server.
 * ver:    Linux 5.0.1
 * test:   make rootfs --> input 's' --> input 'c' */

#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"

#define WORKER_CNT 2       /* fork workers to listen. */
#define SEND_DATA "hello." /* client send to server's test data. */

#define SERVER_PORT 5000      /* server's listen port. */
#define SERVER_IP "127.0.0.1" /* server's ip. */

/* fork a child process to run epoll server. */
void proc(const char *ip, int port);
void proc_busybox(char **arr);

/* fork workers to listen. */
int workers(int worker_cnt, const char *ip, int port);

int main(int argc, char **argv)
{
    int port = SERVER_PORT;
    const char *ip = SERVER_IP;

    if (argc < 2)
    {
        LOG("main error");
        return -1;
    }

    if (strcmp(argv[1], "s") == 0)
    {
        proc(ip, port);
    }
    else if (strcmp(argv[1], "set") == 0)
    {
        set_addr("eth0", "10.0.2.0");
    }
    else if (strcmp(argv[1], "c") == 0)
    {
        proc_client("10.0.2.2", 22, SEND_DATA);
        // proc_client(SERVER_IP, SERVER_PORT, SEND_DATA);
    }
    else if (strcmp(argv[1], "u") == 0)
    {
        proc_udp_client("10.0.2.2", 5000, SEND_DATA);
        // proc_udp_client("10.0.2.0", 5000, SEND_DATA);
    }
    else
    {
        LOG("pls input 's' to run server or 'c' to run client!");
    }

    return 0;
}

void proc_busybox(char **arr)
{
    int pid = fork();
    if (pid == 0)
    {
        if (execv("/busybox", arr) < 0)
        {
            LOG_SYS_ERR("execv error");
        }
    }
}

void proc(const char *ip, int port)
{
    int pid;
    bring_up_net_interface(ip);

    pid = fork();
    if (pid == 0)
    {
        /* child */
        workers(WORKER_CNT, ip, port);
    }
    else if (pid > 0)
    {
        /* parent */
        LOG("pls input 'c' to run client!");
    }
    else
    {
        /* error */
        LOG_SYS_ERR("fork failed!");
        exit(-1);
    }
}

int workers(int worker_cnt, const char *ip, int port)
{
    LOG("workers...");

    int i, pid;
    for (i = 0; i < worker_cnt; i++)
    {
        if (init_server(i, ip, port) < 0)
        {
            LOG("init server failed!");
            return 0;
        }
    }

    for (i = 0; i < worker_cnt; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            /* child. */
            run_server(i);
        }
        else if (pid > 0)
        {
            /* parent */
            LOG("for child pid: %d", pid);
        }
        else
        {
            /* error */
            LOG_SYS_ERR("fork failed!");
            exit(-1);
        }
    }

    return 1;
}
