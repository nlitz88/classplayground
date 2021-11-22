
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// Here is a simple, single client server example. Doesn't utilize any concurrency or anythign fancy, just covers the basic components used.

int main() {

    // Server information.
    uint16_t serverPort = 8555;

    // First create a socket address structure to be assigned to the listening socket.
    struct sockaddr_in listeningSockAddr;
    listeningSockAddr.sin_family = AF_INET;
    listeningSockAddr.sin_port = htons(serverPort);
    listeningSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);      // This socket address can be anything, as this connection isn't referencing any one computer,,
                                                                // but rather bbeing used to service connection requests of any client out there.

    // Next, create the server's connection listening socket.
    int lsock = socket(AF_INET, SOCK_STREAM, 0);
    if(lsock == -1) {
        printf("Failed to create server socket.\n");
        return -1;
    }

    // Now, bind the socket to the socket address structure, thereby associating the socket with that specified server port.
    if(bind(lsock, (struct sockaddr *)&listeningSockAddr, sizeof(listeningSockAddr)) == -1) {
        printf("Failed to bind socket to port [%d].\n", serverPort);
        return -1;
    }

    // If successfully bound, then use that socket to listen for incoming connection requests. Again, tells the operating system this process is
    // willing to service requests directed at this socket that is bound to this port.
    if(listen(lsock, 5) == -1) {
        printf("Failed to set listening on socket [%d].\n", lsock);
        return -1;
    }

    // If listening status set successfully, then can set up a loop to continually accept new connections.
    // Based on the way we know our clients are set up, we expect them to send some data and then receieve some, so 
    // the server will be set up to first receive and then send just as a basic demonstration.
    // Note here how in this way, the server can only entertain a single client at a time.
    // If more than 5 connection requests come in while a client is being serviced, those requests won't get accepted in enough time
    // and will likely time out.
    int clientsock;                             // New socket for each new client connection.
    struct sockaddr_in clientSockAddr;
    char clientIPString[INET_ADDRSTRLEN];
    uint32_t data;                              // Data buffer used for read and writes.
    uint32_t inet_len;
    while(1) {
        inet_len = sizeof(clientSockAddr);
        // First, call accept (blocking) to scan the listening connection queue for new incoming connnection requests.
        clientsock = accept(lsock, (struct sockaddr *)&clientSockAddr, &inet_len);
        inet_ntop(AF_INET, &(clientSockAddr.sin_addr), clientIPString, sizeof(clientSockAddr));
        if(clientsock == -1) {
            printf("Failed to accept new client at %s\n", clientIPString);
            close(lsock);
            return -1;
        }
        printf("Established connection with new client %s\n", clientIPString);

        // Next, read (reicv) data from the new connection.
        if(read(clientsock, &data, sizeof(data)) != sizeof(data)) {
            printf("Error reading data from %s\n", clientIPString);
            close(lsock);
            return -1;
        }
        data = ntohl(data);
        printf("Received %d bytes from %s. Data: %d\n", (int)sizeof(data), clientIPString, data);
        
        // Then, attempt to send some data back to the client. In this case, we'll just increment the data once and send it back.
        ++data;
        data = htonl(data);     // Need to convert it back to network ordering.
        if(write(clientsock, &data, sizeof(data)) != sizeof(data)) {
            printf("Error sending data to %s\n", clientIPString);
            close(lsock);
            return -1;
        }
        printf("Sent %d bytes to %s.\n", (int)sizeof(data), clientIPString);

        // Finally, close the connection to the client by closing the client socket.
        close(clientsock);

    }

    return 0;
}