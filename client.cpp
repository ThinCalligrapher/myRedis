// just gonna copy and paste the libraries for now
#include <sys/socket.h> // man socket2
#include <netinet/in.h> // man sockaddr
#include <arpa/inet.h> // man htons
#include <iostream> // I am just tossing this in here
#include <unistd.h> // for close()
#include <stdio.h> // for printf()
#include <string.h> // for strlen()

void die(const char *msg) { // a null terminated C string?
    perror(msg); // prints "bind()" so far
    exit(1); 
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET; // so setting family to IPv4
    addr.sin_port = ntohs(1234); // network to host 1234
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1 which is always self!
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("connect()");
    }

    char msg[] = "hello";
    write(fd, msg, strlen(msg));

    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        die("read");
    }

    printf("server says: %s\n", rbuf);
    close(fd);

    return 0;
}