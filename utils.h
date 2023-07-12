#ifndef __UTILS__
#define __UTILS__

#define IS_MAC_BROADCAST_ADDR(mac) (bool) ((mac[0] == 0xFF) && (mac[1] == 0xFF) && (mac[2] == 0xFF)\
                                           (mac[3] == 0xFF) && (mac[4] == 0xFF) && (mac[5] == 0xFF))


void apply_mask(const char* prefix, char mask, char** str_prefix);
void layer2_fill_with_broadcast_mac(char **mac_array); 

#endif /*__UTILS__*/