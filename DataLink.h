//
// Created by Xu Lingjue on 2022/11/30.
//

#ifndef NETWORKFINAL_DATALINK_H
#define NETWORKFINAL_DATALINK_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "IpV4.h"

//PPPЭ��  7E FF 03 Э�����ֽ� ��Ϣ���� FCS���ֽ� 7E
struct PPP{
    //�ײ�F A C
    unsigned char headF;
    unsigned char headA;
    unsigned char headC;
    //Э��
    unsigned short protocol;
    //���ݲ���
    unsigned char* data;
    //���ݲ�����һ������ʽ
    struct Ipv4   ip;
    //֡��������
    unsigned short FCS;
    //β��F
    unsigned char tailF;
    //��������ݳ��ȣ���Ϊ���ü��㣬���Դ��ڴ˴���ʵ�ʲ�����
    int extendLength;
};

/************************************��������***************************************/
//�ֽ����

int lengthAfterFill(const unsigned char* data,int length){
    //�������飬ȷ����������鳤��
    int extendLength=length;
    for (int i=0;i<length;i++){
        if (data[i]==0x7e||data[i]==0x7d){
            extendLength++;
        }
    }
    return extendLength;
}

unsigned char* byteFill(const unsigned char* data, int length,int extendLength){
    unsigned char* res= (unsigned char*)malloc(sizeof(unsigned char)*extendLength);
    int index=0;
    for (int i=0;i<length;i++){
        if (data[i]==0x7e){
            res[index++]=0x7d;
            res[index++]=0x5e;
        }
        else if (data[i]==0x7d){
            res[index++]=0x7d;
            res[index++]=0x5d;
        }
        else{
            res[index++]=data[i];
        }
    }
    return res;
}


//�ֽ���仹ԭ
int lengthAfterReduce(const unsigned char* data,int length){
    //�������飬ȷ����ԭ������鳤��
    int reduceLength=length;
    for (int i=0;i<length-1;i++){
        if (data[i]==0x7d&&(data[i+1]==0x5e||data[i+1]==0x5d)){
            reduceLength--;
        }
    }
    return reduceLength;
}



unsigned char* byteReduce(const unsigned char* data, int length,int reduceLength){
    unsigned char* res= (unsigned char*)malloc(sizeof(unsigned char)*reduceLength);
    int index=0;
    int i=0;
    while (i<length){
        if (i<length-1&&data[i]==0x7d){
            if (data[i+1]==0x5e){
                res[index++]=0x7e;
            }
            else{
                res[index++]=0x7d;
            }
            i+=2;
        }
        else{
            res[index++]=data[i++];
        }
    }
    return res;
}
/**
 * RFC1662��¼�ṩ�˼���FCS��C����ʵ��,�˴�ʹ�ò��
 * The FCS-16 generator polynomial: x**0 + x**5 + x**12 + x**16
 * ���´��뾭�������Ķ�
 * **/
/*
 * FCS lookup table as calculated by the table generator.
 */
static unsigned short fcstab[256] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

#define PPPINITFCS16    0xffff  /* Initial FCS value */
#define PPPGOODFCS16    0xf0b8  /* Good final FCS value */
/*
 * Calculate a new fcs given the current fcs and the new data.
 */
unsigned short pppfcs16(unsigned short fcs,const unsigned char* cp, int len)
{
    while (len--)
        fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];

    return (fcs);
}

/*
 * How to use the fcs
 */
//void tryfcs16(unsigned char* cp, int len)
//{
//    unsigned short trialfcs;
//
//    /* add on output */
//    trialfcs = pppfcs16( PPPINITFCS16, cp, len );
//    trialfcs ^= 0xffff;                 /* complement */
//    cp[len] = (trialfcs & 0x00ff);      /* least significant byte first */
//    cp[len+1] = ((trialfcs >> 8) & 0x00ff);
//    /* check on input */
//    trialfcs = pppfcs16( PPPINITFCS16, cp, len + 2 );
//    if ( trialfcs == PPPGOODFCS16 )
//        printf("Good FCS/n");
//}

/************************************������·�����ӿ�***************************************/
void printPPP(struct PPP ppp){
    printf("%02x-%02x-%02x-%04x-",ppp.headF,ppp.headA,ppp.headC,ppp.protocol);
    printIP(ppp.ip);
    printf("-%02x",ppp.tailF);
}

struct PPP dataLinkPack(struct Ipv4 ip, int length,unsigned short protocol){
    printf("----------������·�㿪ʼ��װ----------\n");
    const unsigned char* data=(unsigned char *)(&ip);
    struct PPP ppp;
    //�̶��ֶ�
    ppp.headF=0x7e;
    ppp.headA=0xff;
    ppp.headC=0x03;
    ppp.tailF=0x7e;
    ppp.ip=ip;
    //Э��
    if (protocol==0){
        //IP���ݱ�
        ppp.protocol=0x0021;
    }
    else if (protocol==1){
        //PPPЭ��LCP����
        ppp.protocol=0xc021;
    }
    else if (protocol==2){
        //������������
        ppp.protocol=0x8021;
    }
    else if (protocol==3){
        //��ȫ����֤PAP
        ppp.protocol=0xc023;
    }
    else if (protocol==4){
        //��ȫ��֤CHAP
        ppp.protocol=0xc223;
    }
    else{
        ppp.protocol=0;
    }
    //ѭ���������
    unsigned short fcs=pppfcs16(PPPINITFCS16,data,length);
    fcs^=0xffff;
    ppp.FCS=fcs;
    //�ֽ����
    int extendLength= lengthAfterFill(data,length);
    ppp.data= byteFill(data,length,extendLength);
    ppp.extendLength=extendLength;
    printf("----------������·���װ����----------\n");
    printf("������·���װ���\n");
    printPPP(ppp);
    printf("\n");
    return ppp;
}

struct Ipv4 dataLinkUnpack(struct PPP ppp){
    printf("----------������·�㿪ʼ���װ----------\n");
    //��ԭ���ݲ���
    int reduceLength= lengthAfterReduce(ppp.data,ppp.extendLength);
    unsigned char* ans= byteReduce(ppp.data,ppp.extendLength,reduceLength);
    //����ѭ���������
    //�½�һ���������FCS
    unsigned char* check=(unsigned char*)malloc(sizeof(unsigned char)*(reduceLength+2));
    memcpy(check,ans,sizeof(unsigned char)*reduceLength);
    check[reduceLength+1]=ppp.FCS>>8;
    check[reduceLength]=ppp.FCS;
    if (pppfcs16(PPPINITFCS16,check,reduceLength+2)!=PPPGOODFCS16){
        printf("error!PPP֡У������Զ�����\n");
    }
    else{
        printf("PPP֡������ȷ\n");
    }
    //�ж�Э��
    switch (ppp.protocol) {
        case 0x0021:
            printf("IP���ݱ�\n");
            break;
        case 0xc021:
            printf("PPPЭ��LCP����\n");
            break;
        case 0x8021:
            printf("������������\n");
            break;
        case 0xc023:
            printf("��ȫ����֤PAP\n");
            break;
        case 0xc223:
            printf("��ȫ��֤CHAP\n");
            break;
        default:
            printf("δ֪Э��\n");
    }

    printf("----------������·����װ����----------\n");
    printf("������·����װ���\n");
    printIP(ppp.ip);
    printf("\n");
    return ppp.ip;
}


#endif //NETWORKFINAL_DATALINK_H
