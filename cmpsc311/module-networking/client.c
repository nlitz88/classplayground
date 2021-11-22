#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>



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
    char portString[] = "25565";
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
    








    return 0;
}