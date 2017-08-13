#include <stdio.h>
/*
 *  *  方法1：通过将int强制转换成char单字节，因为强制转换成char能得到int的
 *   *          低地址部分，通过判断这个低地址部分来判断机器字节序
 *    *
 *     */
bool is_big_endian_1(){
    int a = 0x1234;
    char b;
    b = *(char*)&a;
    if (0x12 == b)
        return true;
    return false;
}
/*
 *  *  方法2：利用联合体union的存放顺序是所有成员都从低地址开始存放，然后获取
 *   *          低地址部分判断机器字节序(关于union的妙用将会在后面的文字中介绍)
 *    *
 *     */
bool is_big_endian_2(){
    union NUM{
        int a;
        char b;
    }num;
    num.a = 0x1234;
    if (0x12 == num.b)
        return true;
    return false;
}
int main(int argc, char **argv){

    printf("start test is_big_endian_1:");
    if ( is_big_endian_1() )
        printf("your computer is Big-Endian...\n");
    else
        printf("your computer is Little-Endian...\n");
    printf("start test is_big_endian_2:");
    if ( is_big_endian_2() )
        printf("your computer is Big-Endian...\n");
    else
        printf("your computer is Little-Endian...\n");
    return 0;
}
