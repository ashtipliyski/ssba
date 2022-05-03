#include <stdio.h>

#include <sys/socket.h>

#include <stdint.h>


// use bitfields to minimise the size of the flag struct
typedef struct dns_hdr_flags_s {
  uint16_t qr : 1;
  uint16_t optcode : 4;
  uint16_t aa : 1;
  uint16_t tc : 1;
  uint16_t rd : 1;
  uint16_t ra : 1;
  uint16_t z : 4;
  uint16_t rcode : 4;
} dns_hdr_flags_t;

typedef struct dns_hdr_s {
  uint16_t id;
  dns_hdr_flags_t flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} dns_hdr_t;

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

// TODO: Implement a generic method to obtain individual bits from a byte array
uint16_t get_flag(char * buff, short boffset, short blen);

uint16_t read_short(char * buff) {
  return *(buff) << 8 | *(buff + 1);
}

// encode dns header for transmission (struct -> buffer)
void encode_dns_head(dns_hdr_t * head, char * buff, short len) {
  // assume that buff is already initialised with sufficient length

}

// encode dns question for transmission (struct -> buffer)
void encode_dns_qtn(dns_qtn_t * qtn, char * buff, short len);

// encode dns message for transmission (struct -> buffer)
void encode_dns_msg(dns_msg_t * msg, char * buff, short len);

// decode received dns header (buffer -> struct)
void decode_dns_head(char * buff, short len, dns_hdr_t * hdr)
{
  hdr->id = read_short(&buff[0]);

  // get flags
  hdr->flags.qr = (buff[2] >> 7) & 0x1;
  hdr->flags.optcode = (buff[2] >> 4) & 0xF;
  hdr->flags.aa = (buff[2] >> 2) & 0x1;
  hdr->flags.tc = (buff[2] >> 1) & 0x1;
  hdr->flags.rd = (buff[2]) & 0x1;
  hdr->flags.ra = (buff[3] >> 7) & 0x1;
  hdr->flags.z = (buff[3] >> 4) & 0x7;
  hdr->flags.rcode = (buff[3]) & 0xF;
  
  hdr->qdcount = read_short(&buff[4]);
  hdr->ancount = read_short(&buff[6]);
  hdr->nscount = read_short(&buff[8]);
  hdr->arcount = read_short(&buff[10]);
}

// decode received dns message (buffer -> struct)
void decode_dns_msg(char * buff, short len, dns_msg_t * msg);

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
