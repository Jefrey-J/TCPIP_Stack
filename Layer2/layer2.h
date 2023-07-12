#ifndef __LAYER2__
#define __LAYER2__ 

#include "../net.h"
#include "../gluethread/glthread.h"
#include "utils.h"

#pragma pack (push, 1)

typedef struct ethernet_hdr_{
    mac_addr_t dst_mac; 
    mac_addr_t src_mac;
    short type; 
    char payload[248]; /* Max allowed (1500) */
    unsigned int FCS;
} ethernet_hdr_t;

#pragma pack(pop)

#define ETH_HDR_SIZE_EXCL_PAYLOAD (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t *)0)->payload))

#define ETH_FCS(eth_hdr_ptr, payload_size) ((eth_hdr_ptr)->FCS)

static inline ethernet_hdr_t *ALLOC_ETH_HDR_WITH_PAYLOAD(char *pkt, unsigned int pkt_size) {
    
    char *temp = calloc(1, pkt_size); 
    memcpy(temp, pkt, pkt_size); 

    ethernet_hdr_t *eth_hdr = (ethernet_hdr_t *)(pkt - ETH_HDR_SIZE_EXCL_PAYLOAD); 
    memset((char*)eth_hdr, 0, ETH_HDR_SIZE_EXCL_PAYLOAD); 
    memcpy(eth_hdr->payload, temp, pkt_size);

    free(temp);
    return eth_hdr;
}

static inline bool l2_frame_recv_qualify_on_interface(interface_t *interface, ethernet_hdr_t *ethernet_hdr) {
    bool h1 = IS_INTF_L3_MODE(interface);
    bool h2 = memcmp(IF_MAC(interface), ethernet_hdr->dst_mac, sizeof(mac_addr_t)) == 0; 
    bool h3 = IS_MAC_BROADCAST_ADDR(ethernet_hdr->dst_mac);

    return h1 && (!h2 && h3 || h2 &&  !h3);
}


#endif "__LAYER2__"