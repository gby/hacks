#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

/* Snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/*
 * This callback is called for every packet
 */
void
got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{

	static int count = 1;                   /* packet counter */
	
	printf("\nPacket number %d recevied on %d seconds and %d usec.\n", 
			count, header->ts.tv_sec, header->ts.tv_usec);
	
	/* packet is a pointer to the packet. */
	count++;
}


int main(int argc, char *argv[])
{
	char *dev = "any";			/* capture device name */
	char errbuf[PCAP_ERRBUF_SIZE];		/* error buffer */
	pcap_t *handle;				/* packet capture handle */
	char filter_exp[] = "ether proto 17 and ether[15] & 1 != 0";		/* filter expression [3] */
//	char filter_exp[] = "ip";		/* filter expression */
	struct bpf_program fp;			/* compiled filter program (expression) */
	int num_packets = 10;			/* number of packets to capture. use -1 to read forever */


	/* check for capture device name on command-line */
	if (argc == 2) {
		dev = argv[1];
	}
	else if (argc > 2) {
		fprintf(stderr, "error: unrecognized command-line options\n\n");
		exit(EXIT_FAILURE);
	}
	
	/* print capture info */
	printf("Device: %s\n", dev);
	printf("Number of packets: %d\n", num_packets);
	printf("Filter expression: %s\n", filter_exp);

	/* Open capture device */
	handle = pcap_open_live(dev, SNAP_LEN, 0, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		exit(EXIT_FAILURE);
	}

	/* Set the direction - we only want incoming packets */
	if (pcap_setdirection(handle, PCAP_D_IN) == -1) {
		fprintf(stderr, "Couldn't set direction %s\n", pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}
	
	/* Compile the filter expression */
	if (pcap_compile(handle, &fp, filter_exp, 1, 0) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n",
		    filter_exp, pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}
	
	/* Apply the compiled filter */
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n",
		    filter_exp, pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}

	/* now we can set our callback function */
	pcap_loop(handle, num_packets, got_packet, NULL);

	/* cleanup */
	pcap_freecode(&fp);
	pcap_close(handle);

	printf("\nCapture complete.\n");
	
	return(0);
}
