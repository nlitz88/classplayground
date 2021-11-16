#include <stdio.h>
#include <arpa/inet.h>



int main(int argc, char **argv) {

    // Define struct type in_addr instances for maintaining address information of the connections we want to make.
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

    return 0;
}