#include <Net.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace pi
{
	const char * Net::getAddr(const char * value)
	{
		if (value)
		{
			int fd;
			if ((fd = socket(PF_INET, SOCK_DGRAM, 0)) >= 0)
			{
				ifreq ifr;
				ifr.ifr_addr.sa_family = PF_INET; // IPv4
				strncpy(ifr.ifr_name, value, IF_NAMESIZE - 1); // eth0

				if (ioctl(fd, SIOCGIFADDR, &ifr) >= 0)
				{
					if (char* ip = inet_ntoa(((sockaddr_in*)&ifr.ifr_addr)->sin_addr))
					{
						close(fd);
						return ip;
					}
				}

				close(fd);
			}
		}
		return NULL;
	}
}