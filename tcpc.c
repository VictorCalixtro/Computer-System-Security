#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
  // create a client socket
  int clsck=socket(PF_INET,SOCK_STREAM,0);
  if (clsck<0) { printf("socket open fail.\n"); return(1); }

  // set the server's address
  struct sockaddr_in svaddr;
  svaddr.sin_family=AF_INET;
  inet_aton("127.0.0.1",&svaddr.sin_addr);
  svaddr.sin_port=htons(30000);

  // connect to the server
  if (connect(clsck,(struct sockaddr *)&svaddr,sizeof(struct sockaddr_in))<0) { printf("cannot connect server.\n"); return(1); }

  // send message to the server
  // "exit" to quit
  int status=1;  
  char buf[256];
  int len;
  while (status) {
    len=read(0,buf,256);
    write(clsck,buf,len);
    len=read(clsck,buf,256);
    write(1,buf,len);
    if (strncmp(buf,"exit\n",5)==0) status=0;
  }
  // close the client socket
  close(clsck);
  return 0;
}

