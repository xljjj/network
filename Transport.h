//
// Created by Xu Lingjue on 2022/11/30.
//

#ifndef NETWORKFINAL_TRANSPORT_H
#define NETWORKFINAL_TRANSPORT_H

#include <stdio.h>
#include <string.h>

//采用UDP协议
struct UDP{
    //源端口
    unsigned short Source_port;
    //目的端口
    unsigned short Destination_port;
    //UDP数据报长度
    unsigned short length;
    //检验和
    unsigned short Verified_sum;
    //数据
    unsigned char* data;
};
/************************************辅助函数***************************************/
//ip地址十进制点分转32位
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

//ip地址32位转十进制点分
char* ip32ToAddress(unsigned int ip){
    //char ans[16];  使用此方式声明的数组函数返回时会被释放
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

//把unsigned char*看作16位字数组计算得到相加的和
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


/**UDP检验和的计算方法是：
按每16位求和得出一个32位的数；
如果这个32位的数，高16位不为0，则高16位加低16位再得到一个32位的数；
重复第2步直到高16位为0，将低16位取反，得到校验和。**/
unsigned short getUPD(struct UDP p,const char* Source_address,const char* Destination_address){
    unsigned int sumup=0;
    unsigned int src= ipAddressTo32(Source_address);
    unsigned int dst= ipAddressTo32(Destination_address);

    //第一个十六位
    unsigned short b1=src>>16;
    //第二个十六位
    unsigned short b2=src;
    //第三个十六位
    unsigned short b3=dst>>16;
    //第四个十六位
    unsigned short b4=dst;
    //第五个十六位
    unsigned short b5=0x0011;
    //第六个十六位
    unsigned short b6=p.length;
    //第七个十六位
    unsigned short b7=p.Source_port;
    //第八个十六位
    unsigned short b8=p.Destination_port;
    //第九个十六位
    unsigned short b9=p.length;
    //第十个十六位
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
/************************************运输层对外接口***************************************/
void printUDP(struct UDP udp){
    printf("%04x-%04x-%04x-%04x",udp.Source_port,udp.Destination_port,udp.length,udp.Verified_sum);
    for (int i=0;i<udp.length-8;i++){
        printf("-%02x",udp.data[i]);
    }
}

struct UDP TransportPack(unsigned char* data,unsigned int len,const char* Source_address,
                         const char* Destination_address,unsigned short Source_port,unsigned short Destination_port){
    printf("----------运输层开始封装----------\n");

    struct UDP udp;
    udp.Source_port=Source_port;
    udp.Destination_port=Destination_port;
    udp.length=len+8;
    udp.Verified_sum=0;
    udp.data=data;
    udp.Verified_sum=getUPD(udp,Source_address,Destination_address);

    printf("----------运输层封装结束----------\n");
    printf("运输层封装结果\n");
    printUDP(udp);
    printf("\n");
    return udp;
}

unsigned char* TransportUnpack(struct UDP udp,const char* Source_address,const char* Destination_address){
    printf("----------运输层开始解封装----------\n");

    //检查检验和 出错则丢弃报文
    if (getUPD(udp,Source_address,Destination_address)!=0){
        printf("error!UDP报文出错，自动丢弃\n");
    }
    else{
        printf("UDP报文检验正确\n");
    }
    printf("源端口 %d\n",udp.Source_port);
    printf("目的端口 %d\n",udp.Destination_port);

    printf("----------运输层解封装结束----------\n");
    printf("运输层封装结果\n");
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
