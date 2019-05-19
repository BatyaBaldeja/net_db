#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#define BUF_LEN 512

int  write_to_server (int fd);
int  read_from_server (int fd, char* buf);

int main() {
    int sock;
    struct sockaddr_in addr;
    char buf[BUF_LEN];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(2);
    }
    for(;;){
        if(write_to_server(sock) >= 0){
            while(read_from_server(sock, buf) >= 0){
                printf("%s", buf);
                if(!strcmp(buf, "end")){
                    break;
                }
            }
        }
    }
    close(sock);

    return 0;
}

int read_from_server(int fd, char* buf){
    int nbytes, len, i;
    if(read(fd, &len, sizeof(int)) != (int) sizeof(int)){
        perror("read length");
        return -1;
    }
    for(i = 0; len > 0; i+= nbytes, len -= nbytes){
        nbytes = read (fd, buf + i, len);
        if(nbytes < 0){
            perror("read");
            return -1;
        }
        else
            if(nbytes == 0){
                perror("read truncated");
                return -1;
            }
#ifdef DEBUG
        printf("read %d bytes of %s\n", i, ((i > 0) ? buf : "empty"));
#endif
    }
    return 1;

}
int write_to_server(int fd){

    int nbytes, i;
    char  buf[BUF_LEN];

    fgets(buf,BUF_LEN,stdin);
    buf[strlen(buf)-1] = 0;
    int len = strlen(buf) + 1;
    if(write(fd, &len, sizeof(int)) != (int) sizeof(int)){
        perror("write length");
        return -1;
    }
    for(i = 0; len > 0; i+= nbytes, len -= nbytes){
        nbytes = write (fd, buf + i, len);
        if(nbytes < 0){
            perror("write");
            return -1;
        }
        else
            if(nbytes == 0){
                perror("write truncated");
                return -1;
            }
#ifdef DEBUG
        printf("write %d bytes of %s\n", i, ((i > 0) ? buf : "empty"));
#endif
    }
    return 1;

}
