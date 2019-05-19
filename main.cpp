#include "command_do.h"

#define DEBUG
#define PORT 5555

int read_from_socket(int fd, char* buf);
int write_to_socket(int fd, const char* buf);

int main(int argc, char* argv[]){

	command_do cd;
	if (argc == 2) {
		FILE *fp;
		if (!(fp = fopen(argv[1], "r"))) {printf("Cannot open file: %s\n", argv[1]); return 0;}
		cd.read_base(fp);
		fclose(fp);
	}
	
    int i, err, opt = 1;
    int sock, new_sock;
    fd_set active_set, read_set;
    struct sockaddr_in addr;
    socklen_t size;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Server cannot create socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char* ) &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
    if(err < 0){
        perror("server cannot bind socket");
        exit(EXIT_FAILURE);
    }
    err = listen (sock, 3);
    if(err < 0){
        perror("server listen queue failure");
        exit(EXIT_FAILURE);
    }
    FD_ZERO (&active_set);
    FD_SET (sock, &active_set);
    for(;;){
        read_set = active_set;
        if(select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0){
            perror("server select failure");
            exit(EXIT_FAILURE);
        }
        for(i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &read_set)){
                if(i == sock){
                    struct sockaddr_in client;
                    size = sizeof(client);
                    new_sock = accept(sock, (struct sockaddr*) &client, &size);
                    if(new_sock < 0){
                        perror("server cannot accept socket");
                        exit(EXIT_FAILURE);
                    }
#ifdef DEBUG
                    printf("server connect from host %s port %d\n", inet_ntoa(client.sin_addr), (unsigned int)ntohs(client.sin_port));
#endif
                    FD_SET(new_sock, &active_set);
                }
                else{
                    char buf[LEN];
                    if(read_from_socket(i, buf) < 0){
                        close(i);
                        FD_CLR(i, &active_set);
                    }
                    else{
                        if(cd.get_com() -> command_read(buf)){

                                                cd.get_com() -> print_command();
                            if(cd.get_com() -> get_type() != QUIT){
                                if(cd.get_com() -> get_type() == STOP){
                                    close(sock);
                                    return 0;
                                }
                                else{
                                    cd.only_cmd(i);
                                    write_to_socket(i, "end\n");
                                }
                            }
                            else{
                                close(i);
                                FD_CLR(i, &active_set);
                            }
                        }
                        else
                            write_to_socket(i, "end\n");

                    }
                }

            }
        }
    }
}



int read_from_socket(int fd, char* buf){
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
    }
    return 1;

}
int write_to_socket(int fd, const char* buf){

    int nbytes, i;
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
    }
    return 1;

}
