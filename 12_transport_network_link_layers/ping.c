#include <stdio.h>

#include <arpa/inet.h>
#include <netdb.h> // for NI_MAXHOST
#include <netinet/ip_icmp.h>

// ping packet size
#define PING_PKT_S 64

// default port number
#define PORT_NO 0

// timeout delay for receiving packets, in seconds
#define RECV_TIMEOUT 1

typedef struct ping_pkt_s {
  struct icmphdr hdr;
  char msg [PING_PKT_S - sizeof(struct icmphdr)];
} ping_pkt_t;

unsigned short checksum (void * b, int len)
{
  unsigned short *buff = b;
  unsigned int sum = 0;
  unsigned short result;

  for (sum = 0; len > 1; len -= 2)
    sum += *buff++;

  if (len == 1)    
    sum += *(unsigned char *) buff;

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += sum >> 16;

  result = ~sum;

  return result;  
}

int main ()
{
  int sockfd;
  char *ip_addr, *reverse_hostname;
  struct sockaddr_in addr_con;
  int addrlen = sizeof(addr_con);
  char net_buf[NI_MAXHOST];

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sockfd < 0) {
    printf("Socket file descriptor not received!\n");
    return 1;
  }
  
  printf("hi, I am ping.\n");
}
