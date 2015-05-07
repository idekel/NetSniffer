#include "rawsocketimpl.h"

#include <netinet/if_ether.h>

RawSocketImpl::RawSocketImpl()
{
    initSocket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
}
