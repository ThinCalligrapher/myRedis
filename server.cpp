// I need to take down some notes on man socket.2
// this is the manual from the terminal (as its a syscall) for C/C++
// .2 is the extension for the syscalls of the methods
// the others (none, 7) are realated to other things (overview, shell)
// man read2, socket2, etc is what I want

// also check out man ip.7 for creating tcp/udp sockets

#include <sys/socket.h> // man socket2
#include <netinet/in.h> // man sockaddr
#include <arpa/inet.h> // man htons
#include <iostream> // I am just tossing this in here
#include <unistd.h> // for close()
#include <stdio.h> // for printf()
#include <string.h> // for strlen()

// creating our die function that is apparently not in the book
// I am not a C writer and this is C code I am copying, with C?? strings

void die(const char *msg) { // a null terminated C string?
    perror(msg); // prints "bind()" so far
    exit(1); 
}

static void do_something(int connfd) { // static means only local linkage, cant be used outside of this file!!
    char rbuf[64] = {}; // 0 initialized character array
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1); // I wonder why I am taking 1 off of the size? appending null terminator?
    if (n < 0) {
        die("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world"; // null terminated C string
    write(connfd, wbuf, strlen(wbuf)); // so read and write take in the fd of the client, the buffer as a pointer 
    // to an array (auto decays), and the length of the buffer
    // we can replace read/write with send/recv but these take flags that we dont need
}

int main() {

    // obtain a socket handle
    // AF_INET is IPv4 SOCK_STREAM is for TCP and 0 is always (forget why but check man socket2 for why)
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // set socket options
    // the integer is needed for setting our options to true
    // sol_socket is set to the level, TCP, at which we are manipulating our options
    // resueaddr means we dont have to time_wait when the server goes down, we can reuse socket and port right away
    int val {1};
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));




    // binding our socket to an address!!
    // for this tutorial we are using the wildcard address 0.0.0.0:1234 - ip port combo
    // this bind is just a parameter for listen()

    // this is the struct defined in the sys/sockets.h lib, and its 0 initialized
    struct sockaddr_in addr = {}; //alternative for IPv6 is sockaddr_in6 and xxxx6 for all struct variables
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);    // port host to network short
    addr.sin_addr.s_addr = htonl(0);  // wildcard IP 0.0.0.0 - htonx converts into big endian host to network long
    // big endian is called network byte order

    // binding socket to port
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    // so we pass in our file d?? (destination?), a pointer to our addr struct, and its length
    if (rv) { die("bind()"); }

    // step 4: listen
    // so far we have just been passing parameters. tcp handshakes will be handled by the OS
    // and place established connections in a queue, and the application can accept them with accept()
    // listen
    rv = listen(fd, SOMAXCONN); // first argument is our file directory, and second is the size of the queue 4096 on Linux
    if (rv) { die("listen()"); }
    // so our second argument doesnt matter because accept() is not a bottleneck

    // step 5: Accept connections
    // The server enters a loop that accepts and processes each client connection
    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen); // ok now we pass the address of the length as well??
        if (connfd < 0) {
            continue; // error - break loop beause its 0 for true and -1 for false
        }

        // do_something(connfd);
        printf("Cleint connected on fd=%d\n", connfd);
        close(connfd); // close function from unistd close man!! fd = file descriptor!
    } // so the accept call returns the peers IP address which is why addrlen is input, and output size! so it needs to be an address

    // step 6: read and write
    // for now just 1 read and 1 write


    return 0;




    



}