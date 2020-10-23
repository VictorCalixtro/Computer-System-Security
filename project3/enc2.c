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
    printf("command (for example) : ./enc2 file_to_encrypt key_in_hex\n");
    return 1;
  }
  
  // get the key and make sure it is good for des
  unsigned long long k=strtoull(argv[2], NULL, 16);
  DES_cblock* key=(DES_cblock*)(&k);
  DES_key_schedule ks;
  DES_set_odd_parity(key);
  if (DES_set_key_checked(key, &ks)<0) {
    printf("the key is not good\n");
    return 1;
  }

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
  strncpy(enc+strlen(file), ".enc2\0", 5);
  enc[strlen(file)+5]='\0';
  int fdc=open(enc, O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR);
  if (fdc<0) {
    printf("cannot save the encrypted file %s\n", enc);
    return 1;
  }

  // encryption
  DES_cblock in, out;
  int len;
  while ((len=read(fdp, (char*)&in, 8))>0) {
    if (len<8) {
      char* buf=(char*)&in;
      for (; len<8; len++) buf[len]='\0';
      printf("warning, the end of the file does not have 8 bytes\n");
    }
    DES_ecb_encrypt(&in, &out, &ks, DES_ENCRYPT);
    write(fdc, (char*)&out, len);
  }

  close(fdp);
  close(fdc);
  return 0;
}

