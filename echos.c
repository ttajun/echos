#include <signal.h>
#include <syslog.h>
#include <sys/wait.h>
#include "inet_sockets.h"
#include "common.h"
#include "become_daemon.h"

#define BUF_SIZE 4096

static void removeZombie(int sig)
{
    int savedErrno;             /* waitpid 때문에 errno 변경 방지 */

    savedErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        continue;
    errno = savedErrno;
}

static void handleRequest(int cfd)
{
    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
        if (write(cfd, buf, numRead) != numRead) {
            /* log 는 /var/log/messages */
            syslog(LOG_ERR, "write() failed: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        syslog(LOG_ERR, "Error from read(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int lfd, cfd;               /* Listening and connected sockets */
    struct sigaction sa;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s port\n", argv[0]);

    /* deamonize */
    if (becomeDaemon(0) == -1)
        errExit("becomeDaemon");

    /* signal handler for remove zombie */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = removeZombie;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        fprintf(stderr, "Error from sigaction(): %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    lfd = inetListen(argv[1], 10, NULL);
    if (lfd == -1) {
        fprintf(stderr, "Could not create server socket (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (;;) {
        cfd = accept(lfd, NULL, NULL);  /* Wait for connection */
        if (cfd == -1) {
            fprintf(stderr, "Failure in accept(): %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /* 요청이 올때마다 새로운 child 생성*/
        switch (fork()) {
        case -1:
            fprintf(stderr, "Can't create child (%s)", strerror(errno));
            close(cfd);
            break;

        case 0:                         /* Child */
            close(lfd);                 /* Child 는 listening socket 필요없음 */
            handleRequest(cfd);
            _exit(EXIT_SUCCESS);

        default:                        /* Parent */
            close(cfd);                 /* Parent 는 connected socket 필요없음 */
            break;
        }
    }
}
