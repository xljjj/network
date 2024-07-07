//
// Created by Xu Lingjue on 2022/11/30.
//

#ifndef NETWORKFINAL_TRANSPORT_H
#define NETWORKFINAL_TRANSPORT_H

#include <stdio.h>
#include <string.h>

//����UDPЭ��
struct UDP{
    //Դ�˿�
    unsigned short Source_port;
    //Ŀ�Ķ˿�
    unsigned short Destination_port;
    //UDP���ݱ�����
    unsigned short length;
    //�����
    unsigned short Verified_sum;
    //����
    unsigned char* data;
};
/************************************��������***************************************/
//ip��ַʮ���Ƶ��ת32λ
unsigned int ipAddressTo32(const char* str){
    unsigned char arr[4];
    int index=0;
    int cur=0;
    int n=strlen(str);
    for (int i=0;i<n;i++){
        if (str[i]=='.'){
            arr[index++]=(unsigned char)cur;
            cur=0;
        }
        else{
            cur=cur*10+(str[i]-'0');
        }
    }
    arr[index++]=cur;
    unsigned int ans=arr[3];
    ans|=(arr[2]<<8);
    ans|=(arr[1]<<16);
    ans|=(arr[0]<<24);
    return ans;
}

//ip��ַ32λתʮ���Ƶ��
char* ip32ToAddress(unsigned int ip){
    //char ans[16];  ʹ�ô˷�ʽ���������麯������ʱ�ᱻ�ͷ�
    char* ans=(char*) malloc(sizeof(char)*16);
    int index=0;
    unsigned char arr[4];
    arr[0]=ip>>24;
    arr[1]=ip>>16;
    arr[2]=ip>>8;
    arr[3]=ip;
    for (int i=0;i<4;i++){
        if (i>0){
            ans[index++]='.';
        }
        int x=arr[i];
        if (x<10){
            ans[index++]='0'+x;
        }
        else if (x<100){
            ans[index++]='0'+x/10;
            ans[index++]='0'+x%10;
        }
        else{
            ans[index++]='0'+x/100;
            ans[index++]='0'+(x%100)/10;
            ans[index++]='0'+x%10;
        }
    }
    ans[index]='\0';
    return ans;
}

//��unsigned char*����16λ���������õ���ӵĺ�
unsigned int get16Sum(const unsigned char* data,int length){
    unsigned int res=0;
    for(int i=0;i<length;i++){
        unsigned short re=data[i];
        if(i%2==0){
            res=res+(re<<8);
        }
        else {
            res = res + re;
        }
    }
    return res;
}


/**UDP����͵ļ��㷽���ǣ�
��ÿ16λ��͵ó�һ��32λ������
������32λ��������16λ��Ϊ0�����16λ�ӵ�16λ�ٵõ�һ��32λ������
�ظ���2��ֱ����16λΪ0������16λȡ�����õ�У��͡�**/
unsigned short getUPD(struct UDP p,const char* Source_address,const char* Destination_address){
    unsigned int sumup=0;
    unsigned int src= ipAddressTo32(Source_address);
    unsigned int dst= ipAddressTo32(Destination_address);

    //��һ��ʮ��λ
    unsigned short b1=src>>16;
    //�ڶ���ʮ��λ
    unsigned short b2=src;
    //������ʮ��λ
    unsigned short b3=dst>>16;
    //���ĸ�ʮ��λ
    unsigned short b4=dst;
    //�����ʮ��λ
    unsigned short b5=0x0011;
    //������ʮ��λ
    unsigned short b6=p.length;
    //���߸�ʮ��λ
    unsigned short b7=p.Source_port;
    //�ڰ˸�ʮ��λ
    unsigned short b8=p.Destination_port;
    //�ھŸ�ʮ��λ
    unsigned short b9=p.length;
    //��ʮ��ʮ��λ
    unsigned short b10=p.Verified_sum;
    sumup+=b1+b2+b3+b4+b5+b6+b7+b8+b9+b10;
    sumup+=get16Sum(p.data,p.length-8);
    while (sumup&0xffff0000){
        sumup=(sumup>>16)+(sumup&0xffff);
    }
    unsigned short res=sumup;
    res=~res;
    return res;
}
/************************************��������ӿ�***************************************/
void printUDP(struct UDP udp){
    printf("%04x-%04x-%04x-%04x",udp.Source_port,udp.Destination_port,udp.length,udp.Verified_sum);
    for (int i=0;i<udp.length-8;i++){
        printf("-%02x",udp.data[i]);
    }
}

struct UDP TransportPack(unsigned char* data,unsigned int len,const char* Source_address,
                         const char* Destination_address,unsigned short Source_port,unsigned short Destination_port){
    printf("----------����㿪ʼ��װ----------\n");

    struct UDP udp;
    udp.Source_port=Source_port;
    udp.Destination_port=Destination_port;
    udp.length=len+8;
    udp.Verified_sum=0;
    udp.data=data;
    udp.Verified_sum=getUPD(udp,Source_address,Destination_address);

    printf("----------������װ����----------\n");
    printf("������װ���\n");
    printUDP(udp);
    printf("\n");
    return udp;
}

unsigned char* TransportUnpack(struct UDP udp,const char* Source_address,const char* Destination_address){
    printf("----------����㿪ʼ���װ----------\n");

    //������� ������������
    if (getUPD(udp,Source_address,Destination_address)!=0){
        printf("error!UDP���ĳ����Զ�����\n");
    }
    else{
        printf("UDP���ļ�����ȷ\n");
    }
    printf("Դ�˿� %d\n",udp.Source_port);
    printf("Ŀ�Ķ˿� %d\n",udp.Destination_port);

    printf("----------�������װ����----------\n");
    printf("������װ���\n");
    for (int i=0;i<udp.length-8;i++){
        if (i>0){
            printf("-");
        }
        printf("%02x",udp.data[i]);
    }
    printf("\n");
    return udp.data;
}

#endif //NETWORKFINAL_TRANSPORT_H
