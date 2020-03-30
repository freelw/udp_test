#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

struct sockaddr_in localSock;
struct ip_mreq group;
int sd;
int datalen;
char databuf[1024];

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("no address set.");
    exit(2);
  }
  char *localInterfaceAddress = argv[1];
  printf("localInterfaceAddress is %s\n", localInterfaceAddress);
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    perror("Opening datagram socket error");
    exit(1);
  }
  else
    printf("Opening datagram socket....OK.\n");
  {
    int reuse = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                   sizeof(reuse)) < 0)
    {
      perror("Setting SO_REUSEADDR error");
      close(sd);
      exit(1);
    }
    else
      printf("Setting SO_REUSEADDR...OK.\n");
  }
  memset((char *)&localSock, 0, sizeof(localSock));
  localSock.sin_family = AF_INET;
  localSock.sin_port = htons(4321);
  localSock.sin_addr.s_addr = INADDR_ANY;
  if (bind(sd, (struct sockaddr *)&localSock, sizeof(localSock)))
  {
    perror("Binding datagram socket error");
    close(sd);
    exit(1);
  }
  else
    printf("Binding datagram socket...OK.\n");
  group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
  group.imr_interface.s_addr = inet_addr(localInterfaceAddress);
  if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group,
                 sizeof(group)) < 0)
  {
    perror("Adding multicast group error");
    close(sd);
    exit(1);
  }
  else
    printf("Adding multicast group...OK.\n");

  datalen = sizeof(databuf);
  if (read(sd, databuf, datalen) < 0)
  {
    perror("Reading datagram message error");
    close(sd);
    exit(1);
  }
  else
  {
    printf("Reading datagram message...OK.\n");
    printf("The message from multicast server is: \"%s\"\n", databuf);
  }
  return 0;
}
