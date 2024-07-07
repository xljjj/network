//
// Created by Xu Lingjue on 2022/11/30.
//

#ifndef NETWORKFINAL_IPV4_H
#define NETWORKFINAL_IPV4_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Transport.h"
/**************************����� Begin******************************************/

//ʹ��Ipv4Э��
struct  Ipv4{
    //�汾:   4
    unsigned int version;
    //�ײ�����:4
    unsigned  int Head_length;
    //���ַ���:8
    unsigned int Differentiate_services;
    //�ܳ���: 16
    unsigned int length;
    //��ʶ:16
    unsigned int logotype;
    //��־  1  1   1
    unsigned int other,DF,MF;
    //Ƭƫ��  13
    unsigned int Slice_offset;
    //����ʱ��  8
    unsigned int TTL;
    //Э��    8
    unsigned int agreement;
    //�ײ������   16
    unsigned int First_inspection;
    //Դ��ַ    32
    char*  Source_address;
    //Ŀ�ĵ�ַ    32
    char*  Destination_address;
    //��������������
    struct UDP Transport;
};
//��������
unsigned int checkIP(struct Ipv4 p){
    //�ָ��ַʱʹ��
    unsigned int d1=0xFFFF0000;
    unsigned int d2=0x0000FFFF;
    unsigned int Source_address=ipAddressTo32(p.Source_address);
    unsigned int Destination_address=ipAddressTo32(p.Destination_address);
    //��һ��16λ
    unsigned int b1=(p.version<<12)+(p.Head_length<<8)+p.Differentiate_services;
    //�ڶ���16λ
    unsigned int b2=p.length;
    //������16λ
    unsigned int b3=p.logotype;
    //���ĸ�16λ
    unsigned int b4=(p.other<<15)+(p.DF<<14)+(p.MF<<13)+p.Slice_offset;
    //�����16λ
    unsigned int b5=(p.TTL<<8)+(p.agreement);
    //������16λ
    unsigned int b6=p.First_inspection;
    //���߸�16λ
    unsigned int b7=(Source_address&d1)>>16;
    //�ڰ˸�16λ
    unsigned int b8=(Source_address&d2);
    //�ھŸ�16λ
    unsigned int b9=(Destination_address&d1)>>16;
    //��ʮ��16λ
    unsigned int b10=(Destination_address&d2);
    //����
    unsigned int res=b1+b2+b3+b4+b5+b6+b7+b8+b9+b10;
    unsigned int res1=(res&d1)>>16;
    unsigned int res2=(res&d2);
    unsigned int res3=res1+res2;
    res3=res3^0xFFFF;
    return res3;
}
//��������
unsigned int getIP(struct Ipv4 p){
    //�ָ��ַʱʹ��
    unsigned int d1=0xFFFF0000;
    unsigned int d2=0x0000FFFF;
    unsigned int Source_address=ipAddressTo32(p.Source_address);
    unsigned int Destination_address=ipAddressTo32(p.Destination_address);
    //��һ��16λ
    unsigned int b1=(p.version<<12)+(p.Head_length<<8)+p.Differentiate_services;
    //�ڶ���16λ
    unsigned int b2=p.length;
    //������16λ
    unsigned int b3=p.logotype;
    //���ĸ�16λ
    unsigned int b4=(p.other<<15)+(p.DF<<14)+(p.MF<<13)+p.Slice_offset;
    //�����16λ
    unsigned int b5=(p.TTL<<8)+(p.agreement);
    //������16λ
    unsigned int b6=0;
    //���߸�16λ
    unsigned int b7=(Source_address&d1)>>16;
    //�ڰ˸�16λ
    unsigned int b8=(Source_address&d2);
    //�ھŸ�16λ
    unsigned int b9=(Destination_address&d1)>>16;
    //��ʮ��16λ
    unsigned int b10=(Destination_address&d2);
    //����
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
    //��һ��16λ
    unsigned int b1=(p.version<<12)+(p.Head_length<<8)+p.Differentiate_services;
    //�ڶ���16λ
    unsigned int b2=p.length;
    //������16λ
    unsigned int b3=p.logotype;
    //���ĸ�16λ
    unsigned int b4=(p.other<<15)+(p.DF<<14)+(p.MF<<13)+p.Slice_offset;
    //�����16λ
    unsigned int b5=(p.TTL<<8)+(p.agreement);
    //������16λ
    unsigned int b6=p.First_inspection;
    //���߸�16λ
    unsigned int Source_address= ipAddressTo32(p.Source_address);
    unsigned int b7=(Source_address&d1)>>16;
    //�ڰ˸�16λ
    unsigned int b8=(Source_address&d2);
    //�ھŸ�16λ
    unsigned int Destination_address= ipAddressTo32(p.Destination_address);
    unsigned int b9=(Destination_address&d1)>>16;
    //��ʮ��16λ
    unsigned int b10=(Destination_address&d2);
    //���ݲ���
    printf("%04x-%04x-%04x-%04x-%04x-%04x-%04x-%04x-%04x-%04x-",b1,b2,b3,b4,b5,b6,b7,b8,b9,b10);
    printUDP(p.Transport);
}

struct Ipv4 packIP(struct UDP Transport,
                   const char* Source_address,const char* Destination_address){
    //len :ata����
    //Source_address:Դ��ַ
    //Destination_address:Ŀ�ĵ�ַ
    printf("----------����㿪ʼ���------------\n");
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
    printf("-----------------�����������---------------\n");
    printf("������װ���\n");
    printIP(p);
    printf("\n");
    return p;
};
struct UDP unpackIP(struct Ipv4 p){
    printf("------------------����㿪ʼ��ѹ:--------------\n");
    unsigned int res=checkIP(p);
    if(res==0){
        printf("-------------------------������ײ��������ȷ-------------------------\n");
        printf("-------------------------��ѹ���-----------------------\n");
    }
    else{
        printf("��ѹ����");
    }
    printf("�������װ���\n");
    printUDP(p.Transport);
    printf("\n");
    return p.Transport;
}
/*****************************C*******����� End***************************************/

#endif //NETWORKFINAL_IPV4_H
