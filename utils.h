#ifndef __UTILS__
#define __UTILS__

#define IS_MAC_BROADCAST_ADDR(mac) (bool) (strncmp(mac, "FF:FF:FF:FF:FF:FF", sizeof(mac)/sizeof(mac[0])) == 0)


void apply_mask(const char* prefix, char mask, char** str_prefix);
void layer2_fill_with_brodcast_mac(char **mac_array); 


#endif /*__UTILS__*/