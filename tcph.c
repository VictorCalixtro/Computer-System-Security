#include <string.h>
#include <unistd.h>
#include <stdio.h>

void foo(char* in);
char forproj[]="This string is created for practice \xFF\xE4! Never do this though!";

int main() {  // start communication
  char buf[512];
  int len;
  while (1) {
    len=read(0,buf,512);
    buf[len]='\0';
    foo(buf);
    if (strncmp(buf,"exit\n",5)==0) return 0;
    write(1,buf,len);
  }
  return 0;
}

void foo(char* in) {
  char buf[8];
  strcpy(buf, in);
}

