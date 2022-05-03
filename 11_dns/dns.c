#include <stdio.h>

#include <sys/socket.h>

#include <stdint.h>

typedef struct dns_head_s {
  uint16_t id;
  uint16_t flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} dns_head_t;

typedef struct dns_qtn_s {
  char * name;
  uint16_t len;
  uint16_t class;  
} dns_qtn_t;

typedef struct dns_msg_s {
  char * name;
  uint16_t type;
  uint16_t class;
  uint16_t ttl;
  uint16_t rdlength;
  char * rdata;
} dns_msg_t;

void encode_dns_head(dns_head_t * head, char * buff, short len) {
  // assume that buff is already initialised with sufficient length

  
}

void encode_dns_qtn(dns_qtn_t * qtn, char * buff, short len);
void encode_dns_msg(dns_msg_t * msg, char * buff, short len);

int main()
{
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr * bind_addr;
  int b = bind(sockfd, bind_addr, sizeof(bind_addr));

  // information to send

  char * domain = "bradfieldcs.com";

  short DNS_HEAD_SIZE = 12; // in bytes
  // construct header
  char * header[DNS_HEAD_SIZE + 1];
  short arcount = 0;
  short nscount = 0;
  short ancount = 0;
  short qdcount = 1;
  short flags = 0;
  header[DNS_HEAD_SIZE] = 0;

  // construct question

  // construct answer
}
