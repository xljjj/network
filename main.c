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
    /*运输层开始封装*/
    struct UDP udp= TransportPack(data,length,Source_address,Destination_address,Source_port,Destination_port);
    /*网络层开始封装*/
    struct Ipv4 ip=packIP(udp,Source_address,Destination_address);
    //unsigned char * ip1=(unsigned char *)(&ip);
    /*数据链路层分装*/
    struct PPP  ppp=dataLinkPack(ip,ip.length,0);
    /*数据链路层解封装*/
    struct Ipv4  ip1= dataLinkUnpack(ppp);
    /*********网络层开始解压*************/
    struct UDP udp1= unpackIP(ip1);
    /**********运输层开始解压************/
    unsigned char* ans= TransportUnpack(udp1,ip.Source_address,ip.Destination_address);
    printf("应用层数据 %s\n",ans);
    return 0;
}
