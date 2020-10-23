#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/des.h>

int main(int argc, char** argv) {

  if (argc!=3) {
    printf("command (for example) : ./enc1 file_to_encrypt key_in_hex\n");
    return 1;
  }
  
  // get the key
  unsigned long long key=strtoull(argv[2], NULL, 16);

  // open the file
  char* file=argv[1];
  int fdp=open(file, O_RDONLY);
  if (fdp<0) {
    printf("cannot open the file %s\n", file);
    return 1;
  }

  // open the file to save the encryption
  char enc[strlen(file)+10];
  strncpy(enc, file, strlen(file));
  strncpy(enc+strlen(file), ".enc1\0", 5);
  enc[strlen(file)+5]='\0';
  int fdc=open(enc, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
  if (fdc<0) {
    printf("cannot save the encrypted file %s\n", enc);
    return 1;
  }

  // encryption
  unsigned long long buf;
  unsigned long long k;
  int len;
  k=key;
  while ((len=read(fdp, (char*)&buf, 8))>0) {
    buf^=k;
    k*=key;
    write(fdc, (char*)&buf, len);
  }

  close(fdp);
  close(fdc);
  return 0;
}

