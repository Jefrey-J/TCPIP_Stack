#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "graph.h"
#include "utils.h"


#define MAX_OCTET 4

void apply_mask(const char* prefix, char mask, char** str_prefix) {
    uint8_t ip_val[4];
    sscanf(prefix, "%hhu.%hhu.%hhu.%hhu", &ip_val[0], &ip_val[1], &ip_val[2], &ip_val[3]);

    uint32_t mask_val = 0xFFFFFFFFu << (32 - mask);
    uint32_t network_val = (ip_val[0] << 24) | (ip_val[1] << 16) | (ip_val[2] << 8) | ip_val[3];
    network_val &= mask_val;

    *str_prefix = malloc(16 * sizeof(char)); // Tamanho suficiente para armazenar o endereço IP (XXX.XXX.XXX.XXX\0)

    sprintf(*str_prefix, "%u.%u.%u.%u",
            (network_val >> 24) & 0xFF,
            (network_val >> 16) & 0xFF,
            (network_val >> 8) & 0xFF,
            network_val & 0xFF);
}

void layer2_fill_with_brodcast_mac(char **mac_array) {
    *mac_array = malloc(6 * sizeof(char));
    sprintf(*mac_array, "%02X:%02X:%02X:%02X:%02X:%02X", 0xFF, 0XFF, 0XFF, 0xFF, 0XFF, 0XFF); 
}




