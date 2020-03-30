#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>


int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("no address set.");
    exit(2);
  }
  char *localInterfaceAddress = argv[1];
  printf("localInterfaceAddress is %s\n", localInterfaceAddress);
  if (fork()) {
    struct in_addr localInterface;
    struct sockaddr_in groupSock;
    char databuf[1024] = {0};
    int datalen = sizeof(databuf);
    int sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
      perror("Opening datagram socket error");
      exit(1);
    } else {
      printf("Opening the datagram socket...OK.\n");
    }

    memset((char *)&groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
    groupSock.sin_port = htons(4321);
    localInterface.s_addr = inet_addr(localInterfaceAddress);
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface,
                   sizeof(localInterface)) < 0) {
      perror("Setting local interface error");
      exit(1);
    } else {
      printf("Setting the local interface...OK\n");
    }
    while (1) {
      memset(databuf, 0, datalen);
      scanf("%s", databuf);
      if (sendto(sd, databuf, datalen, 0, (struct sockaddr *)&groupSock,
                sizeof(groupSock)) < 0) {
        perror("Sending datagram message error");
      } else {
        printf("Sending datagram message...OK\n");
      }
    }
    return 0;
  } else {
    printf("child process\n");
  }
  return 0;
}
