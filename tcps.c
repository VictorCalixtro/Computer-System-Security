#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>

/* ** A TCP server **
When the server listens a connection request, it invokes another process to handle the connection.
*/

int main(int argc, char** argv) {

  // create a server TCP socket
  int svsck;
  svsck=socket(PF_INET,SOCK_STREAM,0);
  if (svsck<0) { printf("socket open fail.\n"); return(1); }
  
  // set the server's address
  struct sockaddr_in svaddr;
  svaddr.sin_family=AF_INET;
  svaddr.sin_addr.s_addr=INADDR_ANY;
  svaddr.sin_port=htons(30000);
  
  // make address reusable
  int optval=1;
  if (setsockopt(svsck,SOL_SOCKET,SO_REUSEADDR,(const void *)(&optval), sizeof(int))<0) { printf("address is in use.\n"); return(1); }
  
  // bind address to the server socket
  if (bind(svsck, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_in))) { printf("cannot bind address.\n"); return(1); }
  
  // make the socket listen, no more than 2 pending clients
  if (listen(svsck,2)<0) { printf("socket cannot listen.\n"); return(1); }
  
  // start accept clients
  struct sockaddr_in claddr; // client's address
  int claddrlen=sizeof(struct sockaddr_in);
  int clsck;
  int pid;
  while (1) {
    // receive a connection request
    clsck=accept(svsck,(struct sockaddr *)&claddr,(socklen_t*)&claddrlen);
    if (clsck<0) { printf("connection fail.\n"); }
    // invoke another process to handle the connection
    pid=fork();
    if (pid==0) { // child process
      close(svsck);
      close(0);
      close(1);
      close(2);
      dup2(clsck,0);
      dup2(clsck,1);
      dup2(clsck,2);
      close(clsck);
      execl("/root/echoserver/tcph","tcph",NULL);
    } else if (pid>0) { // parent process
      close(clsck);
      printf("A connection from %s is opened!\n",inet_ntoa(claddr.sin_addr));
      wait(NULL);
      printf("A connection from %s is closed!\n",inet_ntoa(claddr.sin_addr));
    } else { // error
      close(clsck);
      printf("A connection from %s cannot be served!\n",inet_ntoa(claddr.sin_addr));
    }
  }
  return 0;
}

