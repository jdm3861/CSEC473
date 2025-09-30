#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FIFO_PATH "/tmp/.1CE-UNIX"
#define SERVER_IP "[SERVER IP]"
#define SERVER_PORT 8000
#define BUF_SIZE 1024

int main() {
    int fifo_fd, sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    ssize_t n;

    // Make FIFO if it doesn't exist
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        // continue even if fifo exists
    }

    // Open FIFO for reading
    fifo_fd = open(FIFO_PATH, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open fifo");
    }

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        close(fifo_fd);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }


    // Read from FIFO and send over socket
    while ((n = read(fifo_fd, buffer, BUF_SIZE)) > 0) {
        if (send(sock_fd, buffer, n, 0) == -1) {
            perror("send");
            break;
        }
    }

    if (n == -1) {
        perror("read");
    }

    close(fifo_fd);
    close(sock_fd);
    return 0;
}

