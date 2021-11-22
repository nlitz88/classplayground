#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>



int main(int argc, char **argv) {

    // Define struct type in_addr instances for maintaining address information of the connections we want to make.
    // SO: socket address structure maintains address information that defines the connection of that socket.
    struct sockaddr_in v4, sa;
    // Note that this "socket address structure" contains more than just solely the address. It contains the port as well,
    // and a field referencing what address family the address belongs to (IPv4, IPv6, UNIX address for internal use, etc.).
    // struct sockaddr_in6 sa6;    // ipv6 address structure. Just to demonstrate the ability to work with ipv6 addresses.
    
    // Also Note: We define two socket address structures here: v4 and sa. v4 is the socket address structure for one socket,
    // and I believe sa will be the socket address for the server program we're connecting to.

    // IPv4 string containing human readable ip address.
    char myRouterIP[] = "192.168.0.1";
    // use inet_aton to convert the string address to an in_addr. The in_addr variable within the "address struct" is
    // named "sin_addr" == socket interface address.
    // inet_aton(myRouterIP, &(v4.sin_addr));
    inet_pton(AF_INET, myRouterIP, &(v4.sin_addr));    // NOTE: aton is actually deprecated, as it doesn't support ipv6. Therefore, better to use the more modern varients pton and ntop.
    inet_pton(AF_INET, "192.168.0.82", &(sa.sin_addr));
    
    // Another useful function we might want to use is inet_ntop(). This converts a UNIX in_addr structure into an ASCII string.
    char myServerIP[INET_ADDRSTRLEN];       // Can use defined constants to form common strings.
    inet_ntop(AF_INET, &(sa.sin_addr), myServerIP, INET_ADDRSTRLEN); 
    printf("Server IP address from the socket address structure sa: %s\n", myServerIP);

    // Now, what if we want to use DNS to lookup the IP address of a particular host name?
    char serverhostname[] = "mc1.nlitz.com";
    uint16_t port = 25565;
    char portString[16];
    sprintf(portString, "%d", port);
    // Going to employ the POSIX operation "getaddrinfo"
    // Need to first setup a hints structure to constrain the lookup process.
    struct addrinfo hints;
    struct addrinfo *results; // *resultPointer;   // Define pointers to resulting addrinfo structures returned,
    // as well as a pointer to point to the next addrinfo structure in the linked list of addrinfo structures returned.
    // Set up the hints structure for our client code.
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    // Variable to track success of call.
    int success;
    // Finally, make call to getaddrinfo(). getaddrinfo will allocate and initialize a linked list of addrinfo structures
    // one for each of the network addresses that matches the node and service arguments, limited by the restrictions
    // specified via the hints structure! Returns a pointer to the beginning of that linked list.
    // Note that the node == the hostname you are looking up and the service is the port string.
    success = getaddrinfo(serverhostname, portString, &hints, &results);
    if(success != 0) {
        fprintf(stderr, "%s: %s\n", serverhostname, gai_strerror(success));
        abort();
    }
    // If reaching this point, the lookup was successful. Just for fun, display the returned address.
    char serverIPResult[INET_ADDRSTRLEN];
    // NOTE: Here we perform a very interesting yet fundamental conversion. See, the POSIX getaddrinfo function
    //       returns the head of an addrinfo linked list. These addrinfo struct instances contain pointers to 
    //       sockaddr structures that contain generic address information. HOWEVER, we're interested in the
    //       INTERNET variants of these sockaddr structures such that we can intepret the address information according
    //       to IP. Therefore, we must cast the more generic sockaddr struct pointer to a socket internet address 
    //       pointer (sockaddr_in). Then, a socket internet address instance contains an internet address,
    //       I.e. an in_addr, called sin_addr, for socket internet address.
    inet_ntop(AF_INET, &((struct sockaddr_in *)(results->ai_addr))->sin_addr, serverIPResult, INET_ADDRSTRLEN);
    printf("The first IP address returned by the DNS lookup for %s was %s\n", serverhostname, serverIPResult);
    freeaddrinfo(results); // releases memory associated with addrinfo linked list allocated by getaddrinfo.
    // From here, the desired IP address could be recorded and used in the creation of a socket.
    // In this example, however, I'll be connecting to a machine in the local network, and won't be using this address.

    // Now, create a socket. Use socket() to create a new socket instance entry in the system open file table.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // The first parameter is the address family, the second is the type of socket, and the third is a fiel to
    // specify what protocol you want it to use for communication. 0 sticks with the default of the socket type.
    if(sockfd == -1) {
        printf("Error on socket creation \n");
    }
    // NOW, NOTE: actually creating the socket doesn't establish any sort of connection or anything.
    // The next step is to set up a socket internet address structure (sockaddr_in) for a particular server we want to connect to.
    // We defined one earlier, but just for the sake of continuity, we'll define one here.
    char serverIPString[] = "192.168.0.82";
    uint16_t serverPort = 8555;
    struct sockaddr_in serversockaddr;
    serversockaddr.sin_family = AF_INET;
    if(inet_pton(AF_INET, serverIPString, &(serversockaddr.sin_addr)) == 0) {
        return -1;
    }
    serversockaddr.sin_port = htons(serverPort);
    // Note that when setting the socket internet address port, must use the htons function, or
    // the HOST TO NETWORK SHORT function. What this function does is make sure that the number specified
    // is stored using BIG ENDIAN ordering. This is used standardly such that both hosts or routers interpretting
    // the packet frames can properly read the port, as they all agree on BIG ENDIAN, I guess.
    // Also, htons used here (as opposed to htonl) because the port number is a short integer (16 bits).

    // Finally, once the server's socket address structure is set up, we can attempt to initiate a connection
    // to the server at that address, referencing the connection using our socket.
    // What this will do (in the case of our stream socket) is perform a TCP connection with the computer we're
    // trying to connect to and, if successful, associate that socket internet address structure with the socket provided
    // such that the connection can be manipulated via the socket in the read/write/close system calls.
    if(connect(sockfd, (const struct sockaddr *)&serversockaddr, sizeof(serversockaddr)) == -1) {
        printf("Attempt to connect to %s failed.\n", serverIPString);
        return -1;
    }
    
    // If successful, can read and write data.
    
    // Finally, can close the socket.
    close(sockfd);
    sockfd = -1;









    return 0;
}