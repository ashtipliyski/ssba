#include <stdio.h>

#include <time.h> 
#include <glib.h>
#include <arpa/inet.h>

typedef struct pcap_hdr_s {
  guint32 magic_number;   /* magic number */
  guint16 version_major;  /* major version number */
  guint16 version_minor;  /* minor version number */
  gint32  thiszone;       /* GMT to local correction */
  guint32 sigfigs;        /* accuracy of timestamps */
  guint32 snaplen;        /* max length of captured packets, in octets */
  guint32 network;        /* data link type */  
} pcap_hdr_t;

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

// reads 32 bits and returns them as a signed value
// TODO: fix implementation of this
gint32 get_long_signed(char * buff) {
  return (gint32) get_long(buff);
}

int main ()
{
  short HEAD_SIZE = 24;
  pcap_hdr_t head;

  FILE * f = fopen("net.cap", "r");

  unsigned char stuff[HEAD_SIZE + 1];
  fgets(stuff, HEAD_SIZE+1, f);

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

  // start a cycle of reading header + packet until EOF
  // while (1) {
  for (int j = 0; j < 10; j++) {
    short HEADSIZE = 16;
    unsigned char headbytes[HEADSIZE + 1]; // array needs and extra char at end for NUL
    // fgets(headbytes, HEADSIZE+1, f); // fgets reads up to N - 1 chars to make space for NUL at end

    // fgets seems to result in strange contents of headbytes so the individual bytes are
    // read individually
    for (int i = 0; i < HEADSIZE; i++) {
      headbytes[i] = getc(f);
      if (headbytes[i] == EOF) break;
    }
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

    printf("ts time: %s", ctime((&tim)));
    printf("ts_sec: %X\n", get_long(headbytes));
    printf("ts_usec: %lu\n", get_long(headbytes + 4));
    printf("incl_len: %lu\n", get_long(headbytes + 8));
    printf("orig_len: %lu\n", get_long(headbytes + 12));
    int PAYLOADSIZE = get_long(headbytes + 8) - HEADSIZE;

    // generate summary of packet
    printf("> %4d-%02d-%02d %02d:%02d:%02d ", loc_tm->tm_year + 1900,
	   loc_tm->tm_mon + 1,
	   loc_tm->tm_mday,
	   loc_tm->tm_hour,
	   loc_tm->tm_min,
	   loc_tm->tm_sec);
    printf("%d of bytes payload (%d read)\n", get_long(headbytes + 8), get_long(headbytes + 12));

    // ---- parse packet payload ----
    unsigned char data[PAYLOADSIZE + 1];
    fgets(data, PAYLOADSIZE+1, f);
    // break;
  }
}
