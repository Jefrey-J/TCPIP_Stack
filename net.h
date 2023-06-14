#ifndef __NET__
#define __NET__

#include <stdbool.h>
#include <memory.h>

typedef struct node_ node_t; 
typedef struct interface_ interface_t; 
typedef struct graph_ graph_t;

typedef struct ip_addr_ {
    char ip[16];
} ip_addr_t;

typedef struct mac_addr_ {
    char mac[6];
} mac_addr_t;

typedef struct node_nw_prop {
    /* L3 properties */
    bool is_lb_addr_config; 
    ip_addr_t lb_addr; // Loopback address of node
} node_nw_prop_t;

static inline void init_node_nw_prop(node_nw_prop_t *node_nw_prop) {
    //node_nw_prop->flags = 0; 
    node_nw_prop->is_lb_addr_config = false;
    memset(node_nw_prop->lb_addr.ip, 0, 16);
}

typedef struct intf_nw_props {
    /* L2 properties */
    mac_addr_t mac_addr; // MAC Address in the interface NIC 

    /* L3 properties */
    bool is_ip_addr_config; // The intf operates in L3 mode if ip address is configured on it 
    ip_addr_t ip_addr; // IP Address  
    char mask; 
} intf_nw_props_t;

static inline void init_intf_nw_prop(intf_nw_props_t *intf_nw_props) {
    memset(intf_nw_props->mac_addr.mac, 0, 8); 
    intf_nw_props->is_ip_addr_config = false; 
    memset(intf_nw_props->ip_addr.ip, 0, 16); 
    intf_nw_props->mask = 0;
}


/* GET Macros*/
#define IF_MAC(intf_ptr) ((intf_ptr)->intf_nw_props.mac_addr.mac)
#define IF_IP(intf_ptr) ((intf_ptr)->intf_nw_props.ip_addr.ip)
#define IF_MASK(intf_ptr) ((intf_ptr)->intf_nw_props.mask)
#define NODE_LO_ADDR(node_ptr) ((node_ptr)->node_nw_prop.lb_addr.ip)
#define IS_INTF_L3_MODE(intf_ptr) ((intf_ptr)->intf_nw_props.is_ip_addr_config == true)

/* APIs to set Network Node properties */
bool node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask); 
bool node_unset_intf_ip_address(node_t *node, char *local_if); 
void dump_nw_graph(graph_t *graph); 
void dump_node_nw_props(node_t *node);
void dump_intf_nw_prop(interface_t *interface);
bool node_set_loopback_address(node_t *node, char *ip_addr);
unsigned int convert_ip_from_to_str_int(char *ip_addr); 
void convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer); 
interface_t *node_get_matching_subnet_interface(node_t *node, char *ip_addr); 
void interface_assign_mac_address(interface_t *interface);
#endif /*__NET__*/