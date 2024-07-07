#include <stdio.h>
#include "DataLink.h"
#include "IpV4.h"
#include "Transport.h"

int main() {
    char *data="hello";
    unsigned int Source_port=8080;
    unsigned int Destination_port=8081;
    unsigned int length=strlen(data);
    const char* Source_address="192.168.0.0";
    const char* Destination_address="192.168.1.0";
    /*����㿪ʼ��װ*/
    struct UDP udp= TransportPack(data,length,Source_address,Destination_address,Source_port,Destination_port);
    /*����㿪ʼ��װ*/
    struct Ipv4 ip=packIP(udp,Source_address,Destination_address);
    //unsigned char * ip1=(unsigned char *)(&ip);
    /*������·���װ*/
    struct PPP  ppp=dataLinkPack(ip,ip.length,0);
    /*������·����װ*/
    struct Ipv4  ip1= dataLinkUnpack(ppp);
    /*********����㿪ʼ��ѹ*************/
    struct UDP udp1= unpackIP(ip1);
    /**********����㿪ʼ��ѹ************/
    unsigned char* ans= TransportUnpack(udp1,ip.Source_address,ip.Destination_address);
    printf("Ӧ�ò����� %s\n",ans);
    return 0;
}
