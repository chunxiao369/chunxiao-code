

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    struct hostent *ph;
    //ph=gethostbyname(str_ip); /*一般域名转化ｉｐ用这个函数（如1218.com.cn），真正的ｉｐ通过gethostbyaddr,zmh*/
    // u_long res;
    char *sourseip;
    struct sockaddr_in addr;

    sourseip = (char *)malloc(16);
    memset(sourseip,0,16);
    strcpy(sourseip,"10.1.100.205");

    inet_aton(sourseip,&addr.sin_addr);
//    h=gethostbyaddr((char*)&addr.sin_addr,4,AF_INET);
    //ph = gethostbyaddr("10.1.0.1", 4, AF_INET); 
//        if ((res = inet_addr (sourseip)) != INADDR_NONE )
    if ((ph = gethostbyaddr ((char*)&addr.sin_addr,4,AF_INET)) == NULL)
    {
        perror("gethostbyaddr\n");
        return 1;
    }
        
    //if(ph)
    if (ph==NULL)        //没有匹配的计算机名
        printf("name is none!");
    else                 //找到
        printf("name  :\t%s\n", ph->h_name);
    return 0;
}

