#include "graph.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static unsigned int hash_code(void *ptr, unsigned int size) {
    unsigned int value = 0, i = 0; 
    char *str = (char *)ptr; 
    while(i < size) {
        value += *str; 
        value *= 97; 
        str++;  
        i++;
    }
    return value;
}

void interface_assign_mac_address(interface_t *interface) {
    node_t *node = interface->att_node; 
    if(!node) 
        return; 
    unsigned int hash_code_val = 0; 
    hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE); 
    hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE); 
    memset(IF_MAC(interface), 0, sizeof(IF_MAC(interface))); 
    memcpy(IF_MAC(interface), (char *)&hash_code_val, sizeof(unsigned int));
}

bool node_set_loopback_address(node_t *node, char *ip_addr) {
    assert(ip_addr);
    node->node_nw_prop.is_lb_addr_config = true;
    strncpy(NODE_LO_ADDR(node), ip_addr, 16); 
    NODE_LO_ADDR(node)[15] = '\0';  

    return true; 
}

bool node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask) {
    interface_t *interface = get_node_if_by_name(node, local_if); 
    
    if(!interface) assert(0); 
    
    strncpy(IF_IP(interface), ip_addr, 16); 
    IF_IP(interface)[15] = '\0'; 
    interface->intf_nw_props.mask = mask;
    interface->intf_nw_props.is_ip_addr_config = true; 

    return true; 
}

bool node_unset_intf_ip_address(node_t *node, char *local_if) {
    return true;    
}

void dump_nw_graph(graph_t *graph) {

    node_t *node;
    glthread_t *curr;
    
    printf("Topology Name = %s\n", graph->topology_name);

    ITERATE_GLTHREAD_BEGIN(&graph->node_list, curr){
        node = graph_glue_to_node(curr);
        dump_node_nw_props(node);    
    } ITERATE_GLTHREAD_END(&graph->node_list, curr);
}

void dump_node_nw_props(node_t *node) {
    printf("Node name: %s\n", node->node_name); 
    printf("\tIs loopback address configured? %s\n", (node->node_nw_prop.is_lb_addr_config) ? "YES" : "NO"); 
    if(node->node_nw_prop.is_lb_addr_config) {
        printf("\tLoopback address: %s\n", NODE_LO_ADDR(node));
    }
    interface_t *intf; 
    for(int i = 0; i < MAX_INTF_PER_NODE; i++) {
        intf = node->intf[i];
        if(!intf) break;
        dump_intf_nw_prop(intf);
    }
}

void dump_intf_nw_prop(interface_t *interface) { 
    printf("\tInterface name: %s\n", interface->if_name); 
    printf("\tCost: %u\n", interface->link->cost); 
    printf("\tIP Address: %s\n", IF_IP(interface));
    printf("\tMask: %d\n", IF_MASK(interface));
    printf("\tMAC Address: %hhu:%hhu:%hhu:%hhu:%hhu:%hhu\n", 
            IF_MAC(interface)[0], IF_MAC(interface)[1],
            IF_MAC(interface)[2], IF_MAC(interface)[3],
            IF_MAC(interface)[4], IF_MAC(interface)[5]);
}


unsigned int convert_ip_from_to_str_int(char *ip_addr) {
    char ip_val[4]; 

    sscanf(ip_addr, "%hhu.%hhu.%hhu.%hhu", &ip_val[0], &ip_val[1], &ip_val[2], &ip_val[3]); 

    return ip_val[0] << 24 | ip_val[1] << 16 | ip_val[2] << 8 | ip_val[3]; 
}

void convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer) {
    output_buffer = malloc(16 * sizeof(char));
    sprintf(output_buffer, "%u.%u.%u.%u", (ip_addr >> 24) & 0xFF,
                                          (ip_addr >> 16) & 0xFF,
                                          (ip_addr >> 8) & 0xFF,
                                          (ip_addr) & 0xFF);
}

interface_t *node_get_matching_subnet_interface(node_t *node, char *ip_addr) {
    uint8_t mask;
    char *subnet;
    char *node_subnet;

    sscanf(ip_addr, "%*[^/]/%hhu", &mask);

    apply_mask(ip_addr, mask, &subnet);
    
    interface_t *intf = NULL;

    for (int i = 0; i < MAX_INTF_PER_NODE; i++) {
        intf = node->intf[i];
        if(!intf) 
            continue;
        apply_mask(IF_IP(intf), IF_MASK(intf), &node_subnet);
        if (strncmp(node_subnet, subnet, sizeof(node_subnet)/sizeof(node_subnet[0])) != 0) {
            continue;
        } 
        return intf;
    }
    return NULL;
}
