//
// Created by Xu Lingjue on 2022/11/30.
//

#ifndef NETWORKFINAL_IPV4_H
#define NETWORKFINAL_IPV4_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Transport.h"
/**************************网络层 Begin******************************************/

//使用Ipv4协议
struct  Ipv4{
    //版本:   4
    unsigned int version;
    //首部长度:4
    unsigned  int Head_length;
    //区分服务:8
    unsigned int Differentiate_services;
    //总长度: 16
    unsigned int length;
    //标识:16
    unsigned int logotype;
    //标志  1  1   1
    unsigned int other,DF,MF;
    //片偏移  13
    unsigned int Slice_offset;
    //生存时间  8
    unsigned int TTL;
    //协议    8
    unsigned int agreement;
    //首部检验和   16
    unsigned int First_inspection;
    //源地址    32
    char*  Source_address;
    //目的地址    32
    char*  Destination_address;
    //来自运输层的数据
    struct UDP Transport;
};
//检验检验和
unsigned int checkIP(struct Ipv4 p){
    //分割地址时使用
    unsigned int d1=0xFFFF0000;
    unsigned int d2=0x0000FFFF;
    unsigned int Source_address=ipAddressTo32(p.Source_address);
    unsigned int Destination_address=ipAddressTo32(p.Destination_address);
    //第一个16位
    unsigned int b1=(p.version<<12)+(p.Head_length<<8)+p.Differentiate_services;
    //第二个16位
    unsigned int b2=p.length;
    //第三个16位
    unsigned int b3=p.logotype;
    //第四个16位
    unsigned int b4=(p.other<<15)+(p.DF<<14)+(p.MF<<13)+p.Slice_offset;
    //第五个16位
    unsigned int b5=(p.TTL<<8)+(p.agreement);
    //第六个16位
    unsigned int b6=p.First_inspection;
    //第七个16位
    unsigned int b7=(Source_address&d1)>>16;
    //第八个16位
    unsigned int b8=(Source_address&d2);
    //第九个16位
    unsigned int b9=(Destination_address&d1)>>16;
    //第十个16位
    unsigned int b10=(Destination_address&d2);
    //计算
    unsigned int res=b1+b2+b3+b4+b5+b6+b7+b8+b9+b10;
    unsigned int res1=(res&d1)>>16;
    unsigned int res2=(res&d2);
    unsigned int res3=res1+res2;
    res3=res3^0xFFFF;
    return res3;
}
//计算检验和
unsigned int getIP(struct Ipv4 p){
    //分割地址时使用
    unsigned int d1=0xFFFF0000;
    unsigned int d2=0x0000FFFF;
    unsigned int Source_address=ipAddressTo32(p.Source_address);
    unsigned int Destination_address=ipAddressTo32(p.Destination_address);
    //第一个16位
    unsigned int b1=(p.version<<12)+(p.Head_length<<8)+p.Differentiate_services;
    //第二个16位
    unsigned int b2=p.length;
    //第三个16位
    unsigned int b3=p.logotype;
    //第四个16位
    unsigned int b4=(p.other<<15)+(p.DF<<14)+(p.MF<<13)+p.Slice_offset;
    //第五个16位
    unsigned int b5=(p.TTL<<8)+(p.agreement);
    //第六个16位
    unsigned int b6=0;
    //第七个16位
    unsigned int b7=(Source_address&d1)>>16;
    //第八个16位
    unsigned int b8=(Source_address&d2);
    //第九个16位
    unsigned int b9=(Destination_address&d1)>>16;
    //第十个16位
    unsigned int b10=(Destination_address&d2);
    //计算
    unsigned int res=b1+b2+b3+b4+b5+b6+b7+b8+b9+b10;
    unsigned int res1=(res&d1)>>16;
    unsigned int res2=(res&d2);
    unsigned int res3=res1+res2;
    res3=res3^0xFFFF;
    return res3;
}

void printIP(struct Ipv4 p ){
    unsigned  d1=0xFFFF0000;
    unsigned int d2=0x0000FFFF;
    //第一个16位
    unsigned int b1=(p.version<<12)+(p.Head_length<<8)+p.Differentiate_services;
    //第二个16位
    unsigned int b2=p.length;
    //第三个16位
    unsigned int b3=p.logotype;
    //第四个16位
    unsigned int b4=(p.other<<15)+(p.DF<<14)+(p.MF<<13)+p.Slice_offset;
    //第五个16位
    unsigned int b5=(p.TTL<<8)+(p.agreement);
    //第六个16位
    unsigned int b6=p.First_inspection;
    //第七个16位
    unsigned int Source_address= ipAddressTo32(p.Source_address);
    unsigned int b7=(Source_address&d1)>>16;
    //第八个16位
    unsigned int b8=(Source_address&d2);
    //第九个16位
    unsigned int Destination_address= ipAddressTo32(p.Destination_address);
    unsigned int b9=(Destination_address&d1)>>16;
    //第十个16位
    unsigned int b10=(Destination_address&d2);
    //数据部分
    printf("%04x-%04x-%04x-%04x-%04x-%04x-%04x-%04x-%04x-%04x-",b1,b2,b3,b4,b5,b6,b7,b8,b9,b10);
    printUDP(p.Transport);
}

struct Ipv4 packIP(struct UDP Transport,
                   const char* Source_address,const char* Destination_address){
    //len :ata长度
    //Source_address:源地址
    //Destination_address:目的地址
    printf("----------网络层开始打包------------\n");
    struct Ipv4 p;
    p.version=0x0004;
    p.Head_length=0x0005;
    p.Differentiate_services=0;
    p.length=Transport.length+20;
    p.logotype=0;
    p.other=0;
    p.DF=1;
    p.MF=0;
    p.Slice_offset=0;
    p.TTL=0;
    p.agreement=17;
    p.Source_address=Source_address;
    p.Destination_address=Destination_address;
    p.Transport=Transport;
    p.First_inspection=getIP(p);
    printf("-----------------网络层结束打包---------------\n");
    printf("网络层封装结果\n");
    printIP(p);
    printf("\n");
    return p;
};
struct UDP unpackIP(struct Ipv4 p){
    printf("------------------网络层开始解压:--------------\n");
    unsigned int res=checkIP(p);
    if(res==0){
        printf("-------------------------网络层首部检验和正确-------------------------\n");
        printf("-------------------------解压完成-----------------------\n");
    }
    else{
        printf("解压错误");
    }
    printf("网络层解封装结果\n");
    printUDP(p.Transport);
    printf("\n");
    return p.Transport;
}
/*****************************C*******网络层 End***************************************/

#endif //NETWORKFINAL_IPV4_H
