#include <stdio.h>

#include <time.h> 
#include <glib.h>
#include <stdlib.h> // for sort

typedef struct pcap_hdr_s {
  guint32 magic_number;   /* magic number */
  guint16 version_major;  /* major version number */
  guint16 version_minor;  /* minor version number */
  gint32  thiszone;       /* GMT to local correction */
  guint32 sigfigs;        /* accuracy of timestamps */
  guint32 snaplen;        /* max length of captured packets, in octets */
  guint32 network;        /* data link type */  
} pcap_hdr_t;


typedef struct pcaprec_hdr_s {
  guint32 ts_sec;
  guint32 ts_usec;
  guint32 incl_len;
  guint32 orig_len;
} pcaprec_hdr_t;

typedef struct tcp_pck_s {
  guint32 seq_no;
  guint32 ack_no;
  char data[1500];
} tcp_pck_t;

// quick sort
int pckt_comparator(const void *v1, const void *v2)
{
  const tcp_pck_t *p1 = (tcp_pck_t *)v1;
  const tcp_pck_t *p2 = (tcp_pck_t *)v2;

  if (p1->seq_no < p2->seq_no)
    return -1;
  else if (p1->seq_no > p2->seq_no)
    return 1;
  else
    return 0;
}

// reads 16 bits in little endian format
guint16 get_short (char * buff)
{
  return buff[1] << 8 | buff[0];
}

// reads 32 bits in little endian format
guint32 get_long (char * buff)
{
  return
    (unsigned char) *(buff) |
    (unsigned char) *(buff+1) << 8 |
    (unsigned char) *(buff+2) << 8 * 2 |
    (unsigned char) *(buff+3) << 8 * 3;
}

// reads 32 bits in big endian format
guint32 get_long_be (char * buff)
{
  return
    (unsigned char) *(buff) << 8 * 3|
    (unsigned char) *(buff+1) << 8 * 2 |
    (unsigned char) *(buff+2) << 8 * 1 |
    (unsigned char) *(buff+3);
}

// reads 32 bits and returns them as a signed value
// TODO: fix implementation of this
gint32 get_long_signed(char * buff) {
  return (gint32) get_long(buff);
}

void print_rec_hdr(struct pcaprec_hdr_s hdr) {

  printf("ts_sec: %X\n", hdr.ts_sec);
  printf("ts_usec: %lu\n", hdr.ts_usec);
  printf("incl_len: %lu\n", hdr.incl_len);
  printf("orig_len: %lu\n", hdr.orig_len);
}

int main ()
{
  short first;
  short HEAD_SIZE = 24;
  pcap_hdr_t head;

  FILE * f = fopen("net.cap", "r");

  unsigned char stuff[HEAD_SIZE + 1];
  fgets(stuff, HEAD_SIZE+1, f);

  short p = 0;
  tcp_pck_t tcp_packets [99];

  // head = (pcap_hdr_t) *stuff;

  /* printf("-- header bytes start --\n"); */
  /* for (int i = 0; i < 24; i++) { */
  /*    printf("0x%02X\n", stuff[i]); */
  /*   /\* unsigned char c = getc(f); *\/ */
  /*   /\* stuff[0] = c; *\/ */
  /*   /\* printf("0x%02X\n", c); *\/ */
  /* } */
  /* printf("-- header bytes end --\n"); */

  printf("magic: 0x%08X\n", get_long(&stuff[0]));
  printf("vers (maj): 0x%04X\n", get_short(&stuff[4])); 
  printf("vers (min): 0x%04X\n", get_short(&stuff[6]));
  printf("zone: %lu\n", get_long(&stuff[8]));
  printf("sigfig: %lu\n", get_long(&stuff[12]));
  printf("len: %lu\n", get_long(&stuff[16]));
  printf("net: %lu\n", get_long(&stuff[20])); // 1 for ethernet

  // default values for TCP packets
  unsigned int start_seq = 0;
  unsigned int start_ack = 0;

  // start a cycle of reading header + packet until EOF
  // while (1) {
  for (int j = 0; j < get_long(&stuff[16]); j++) {
    short HEADSIZE = 16;
    unsigned char headbytes[HEADSIZE + 1]; // array needs and extra char at end for NUL
    // fgets(headbytes, HEADSIZE+1, f); // fgets reads up to N - 1 chars to make space for NUL at end

    // fgets seems to result in strange contents of headbytes so the individual bytes are
    // read individually
    char c;
    for (int i = 0; i < HEADSIZE; i++) {
      headbytes[i] = getc(f);
    }
    // checks if EOF was encountered after reading header
    if (feof(f))
      break;
    headbytes[HEADSIZE+1] = 0; // manually set NUL at end

    /*
    printf("0x%X\n", headbytes[HEADSIZE]);
    for (int i = 0; i < HEADSIZE; i++) {
      printf("head c = %02x\n", headbytes[i]);
      // printf("head c = %02x\n", getc(f));
      // if ((i+i) % 4 == 0 && i != 0) printf("--\n");
    }
    */
    

    time_t tim = get_long(headbytes);
    struct tm * loc_tm = localtime(&tim);
    // time_t now = time(&now);
    // struct tm * loc_tm = localtime(&now);
    // printf("%lu\n", time);

    struct pcaprec_hdr_s rec_hdr; // = malloc(sizeof(pcaprec_hdr_t));
    rec_hdr.ts_sec = get_long(headbytes);
    rec_hdr.ts_usec = get_long(headbytes + 4);
    rec_hdr.incl_len = get_long(headbytes + 8);
    rec_hdr.orig_len = get_long(headbytes + 12);

    //    printf("ts time: %s", ctime((&tim)));
    // print_rec_hdr(rec_hdr);
    int PAYLOADSIZE = rec_hdr.incl_len;

    short verbose = 0;
    if (verbose) {
    // generate summary of packet
    printf("> %4d-%02d-%02d %02d:%02d:%02d (%d) ", loc_tm->tm_year + 1900,
	   loc_tm->tm_mon + 1,
	   loc_tm->tm_mday,
	   loc_tm->tm_hour,
	   loc_tm->tm_min,
	   loc_tm->tm_sec, j);
    printf("%d of bytes payload (%d read)\n", rec_hdr.incl_len, rec_hdr.orig_len);
    }

    // ---- parse packet payload ----
    unsigned char data[PAYLOADSIZE + 1];
    // fgets(data, PAYLOADSIZE+1, f);
    // char c;
    for (int i = 0; i < PAYLOADSIZE; i++) {
      //c = getc(f);
      data[i] = getc(f);
      // printf("%d, %d, %02X\n", PAYLOADSIZE, i, data[i]);
    }
    data[PAYLOADSIZE + 1] = 0; // append NUL

    // checks if EOF was encountered after reading payload
    if (feof(f))
      break;

    // ---- parse ETH packet
    short ETH_HEAD_SIZE = 14;
 
    /**
     * data values 0 - 5 encode source MAC address
     * data values 6 - 11 encode destination MAC address
     * data values 12 - 13 encode type of packet in payload (EtherType)
     *
     * Ether type values:
     *
     * 0x0800 IPv4
     * 0x0806 ARP packet
     * 0x86DD IPv6
     * 0x8100 IEEE 802.1Q tag is present
     *
     **/

    // will parse only IP packets
    if (data[12] != 0x08 || data[13] != 00) {
      printf("-- non IPv4 packet, skipping\n");
      // printf("data[12] = %02X, data[13] = %02X\n", data[12], data[13]);
      continue;
    }

    for (int i = ETH_HEAD_SIZE; i < PAYLOADSIZE; i++) {
      // printf("packet payload (%i): %02X\n", i, data[i]);
    }

    /**
     * IP parsing
     * - extract source and dest addresses (interested in packets coming from server)
     * - ignore all but TCP packets (protocol fields)
     * - need to figure out when payload starts (can be obtained from IHL value)
     **/

    int src_ip = get_long(&data[ETH_HEAD_SIZE + 12]);
    int dst_ip = get_long(&data[ETH_HEAD_SIZE + 16]);
    int ihl = get_short(&data[ETH_HEAD_SIZE]) & 0x0F; // read first octet and mask the lower order bits
    int prot = data[ETH_HEAD_SIZE + 9];
    int IP_HEAD_SIZE = ihl * 4; // in bytes (ihl number measures header in 32 bit words)

    /**
     * Addressing (assumptions):
     * - client: 0x6500A8C0
     * - server: 0x9AFC1EC0
     **/

    if (src_ip != 0x9AFC1EC0)
      continue;

    // printf("\tip payload: src %X\tdst %X\tIHL %02X\tprot %02X\tpckt size %i\n", src_ip, dst_ip, ihl, prot, PAYLOADSIZE);

    // ---- parsing TCP packet
    int TCP_START = ETH_HEAD_SIZE + IP_HEAD_SIZE;

    unsigned int TCPHEAD_SIZE = ((data[TCP_START + 12] & 0xF0 ) >> 4) * 4;
    
    unsigned int seq_no = get_long_be(&data[TCP_START+4]);
    unsigned int ack_no = get_long_be(&data[TCP_START+8]);
    
    if (first && start_seq == 0) {
      start_seq = seq_no;
      start_ack = ack_no;
    }

    unsigned int seq_no_rel = seq_no - start_seq;
    unsigned int ack_no_rel = ack_no - start_ack;

    if (seq_no_rel == 0)
      continue;

    unsigned int HTTPSTART = TCPHEAD_SIZE + TCP_START;
    for (int i = HTTPSTART; i < PAYLOADSIZE; i++) {
      // printf("TCP: %02X\n", data[i]);
    }

    printf("\t\tTCP:\tsrc port %02X\tseq no:%lu\tack no: %lu\tstart seq: %02X\n", data[TCP_START+1], seq_no_rel, ack_no_rel, start_seq);

    // create a new struct and store it in data
    tcp_pck_t pck;
    pck.seq_no = seq_no_rel;
    pck.ack_no = ack_no_rel;

    for (int i = HTTPSTART; i < PAYLOADSIZE; i++) {
      // printf("storing val %X at %lu from %i\n", data[i], i - HTTPSTART, i);
      pck.data[i- HTTPSTART] = data[i];
    }

    //if (seq_no_rel > 100000)
    //continue;

    // check to strip the first TCP packet (with seq_no_rel of 0)
    if (seq_no_rel < 100000) {
      tcp_packets[p] = pck;
      p++;
    }
    // break;
    // if (j > 3) break;
    first = 1;
  }

  printf("---\n");
  qsort(tcp_packets, p, sizeof(tcp_pck_t), pckt_comparator);
  int skipped = 0;
  for (int i = 0; i < p; i++) {
    if (i > 0 && tcp_packets[i].seq_no == tcp_packets[i-1].seq_no) {
      tcp_packets[i].seq_no = -1;
      skipped++;
    }
  }
  qsort(tcp_packets, p, sizeof(tcp_pck_t), pckt_comparator);
  
  FILE * fout = fopen("out.jpg", "w");
  int start = 0;
  // sort packets by seq no
  for (int i = 0; i < p; i++) {
    if (tcp_packets[i].seq_no == -1)
      continue;
    
    printf("packet seq no: %10i\tack no: %i\n", tcp_packets[i].seq_no, tcp_packets[i].ack_no);
    // printf("data: %s\n", tcp_packets[i].data);
    // printf("%s", tcp_packets[i].data);
    int chars = 0;
    for (int j = 0; tcp_packets[i].data[j] != '\0'; j++) {
      chars++;
      // printf("%02X ", tcp_packets[i].data[j]);
      
      if (j > 1 && tcp_packets[i].data[j] == '\r' && tcp_packets[i].data[j-1] == '\n') {
	printf("--- foundit ---\n");
	start = 1;
	continue;
      }
      if (start)
	fwrite(&tcp_packets[i].data[j], sizeof(char), 1, fout);
    }
    // fwrite(&tcp_packets[i].data, sizeof(char), chars, fout);
  }
  printf("\n");

  // dump http data to file 
  printf("--- end ---\n");
}
