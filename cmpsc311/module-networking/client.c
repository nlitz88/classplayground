
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main() {

    
    // Server information.
    char serverIP[] = "127.0.0.1";
    uint16_t serverPort = 8555;

    // Create socket address structure pointing to server.
    struct sockaddr_in serverAddressStruct;
    serverAddressStruct.sin_family = AF_INET;                           // IPV4 address family.
    serverAddressStruct.sin_port = htons(serverPort);                   // Convert short to network ordered short.
    if(inet_pton(AF_INET, serverIP, &(serverAddressStruct.sin_addr)) == 0) {  // Convert string IP to internet address structure.
        return -1;
    }

    // Create socket.
    int serversockfd = socket(AF_INET, SOCK_STREAM, 0);     // Instantiate a socket with AF_INET address family sockaddr instance, stream socket, and no explicitly specified protocol.
    if(serversockfd == -1) {
        printf("Failed to create client socket.\n");
        return -1;
    }

    // Attempt to establish connection and thereby link the socket to the socket address structure.
    if(connect(serversockfd, (struct sockaddr *)&serverAddressStruct, sizeof(serverAddressStruct)) == -1) {
        printf("Attempt to connect to %s failed.\n", serverIP);
        return -1;
    }

    // Using the successful connection now referenced by the socket for that server, write (send) some data to the server.
    uint32_t data = htonl(6);       // The number 6 is going to be sent. However, this host long must be represented as a network long to be interpretted correctly.
    // If write does not return that the number of bytes sent is not equal to the number of bytes of the data, then the write (send) didn't complete.
    if(write(serversockfd, &data, sizeof(data)) != sizeof(data)) {
        printf("Error writing network data [%d]\n", data);
    }
    printf("Successfully sent %d bytes to server. Data: [%d]\n", (int)sizeof(data), ntohl(data));

    // Then, attempt to read some data from the server. This is a blocking call, so execution will wait until the specified number of bytes
    // has been read.
    if(read(serversockfd, &data, sizeof(data)) != sizeof(data)) {
        printf("Failed to read all %d bytes of data [%d]\n", (int)sizeof(data), data);
    }
    data = ntohl(data);           // Reinterpret the data written into the data buffer from the network read back to host byte ordering.
    printf("Successfully read %d bytes from server. Data: [%d]\n", (int)sizeof(data), data);

    // Then, close the connection using close on the socket.
    close(serversockfd);
    serversockfd = -1;      // For good measure so that it wouldn't accidentally get used again.

    return 0;
}