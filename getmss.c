#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <netdb.h>

const char * socktype_to_string(int type) {
  switch(type) {
    case SOCK_STREAM:
      return "SOCK_STREAM";
    case SOCK_DGRAM:
      return "SOCK_DGRAM";
    case SOCK_SEQPACKET:
      return "SOCK_SEQPACKET";
    case SOCK_RAW:
      return "SOCK_RAW";
    case SOCK_RDM:
      return "SOCK_RDM";
    case SOCK_PACKET:
      return "SOCK_PACKET";
    default:
      return "UNKNOWN";
  }
}

const char * protocol_to_string(int protocol) {
  switch(protocol) {
    case AF_UNIX:
      return "AF_UNIX";
    case AF_INET:
      return "AF_INET";
    case AF_INET6:
      return "AF_INET6";
    case AF_IPX:
      return "AF_IPX";
    case AF_NETLINK:
      return "AF_NETLINK";
    case AF_X25:
      return "AF_X25";
    case AF_AX25:
      return "AF_AX25";
    case AF_ATMPVC:
      return "AF_ATMPVC";
    case AF_APPLETALK:
      return "AF_APPLETALK";
    case AF_PACKET:
      return "AF_PACKET";
    default:
      return "UNKNOWN";
  }
}

int main(int argc, char ** argv) {
  char * str_host;
  char * str_port = "http";

  if (argc >= 2) {
    str_host = argv[1];
    if (argc >= 3) {
      str_port = argv[2];
    }
  } else {
    printf("Usage: getmss <site>\n\n  Example getmss google.com 80\n");
    return 1;
  } 

  struct addrinfo *lookup_result, *i;
  int res = getaddrinfo(argv[1], str_port, NULL, &lookup_result);

  if (res != 0) {
    perror("Failed to lookup host");
    return 1;
  }

  for (i = lookup_result; i != NULL; i = i->ai_next) {
    char buffer[40];
    const char * ip_as_string;
    uint16_t port = 0;

    switch (i->ai_family) {
      case AF_INET:
        ip_as_string = inet_ntop(i->ai_family, &((const struct sockaddr_in *)i->ai_addr)->sin_addr, buffer, sizeof(buffer));
        port = ntohs(((struct sockaddr_in *)i->ai_addr)->sin_port);
        break;
      case AF_INET6:
        ip_as_string = inet_ntop(i->ai_family, &((const struct sockaddr_in6 *)i->ai_addr)->sin6_addr, buffer, sizeof(buffer));
        port = ntohs(((struct sockaddr_in6 *)i->ai_addr)->sin6_port);
        break;
      default:
        continue;
        printf("Trying unknown protocol. Can't decode connection information\n");
    }

    printf("Trying protocol %s socktype %s address %s port %u\n", protocol_to_string(i->ai_family), socktype_to_string(i->ai_socktype), ip_as_string, port);

    int sfd;
    sfd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);

    if (sfd == -1) {
      perror("Failed to create socket.");
      continue;
    }

    if (connect(sfd, i->ai_addr, i->ai_addrlen) != -1) {
      unsigned int mss;
      socklen_t sl = sizeof(mss);

      // Determine MSS for this socket.
      mss = 0;
      res = getsockopt(sfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &sl);
      if (res != 0) {
        perror("Failed to get MSS");

        close(sfd);
        freeaddrinfo(lookup_result);
        continue;
      }
 
      printf("Found MSS to be %u\n", mss);
      close(sfd);
      continue;
    } else {
      perror("Failed to connect.");
    }

    close(sfd);
  }

  freeaddrinfo(lookup_result);
  return 0;
}
