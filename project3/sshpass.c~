#include <libssh2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

  char* username;
  char* password;
  in_addr_t servaddr; // server address
  uint16_t servport; // server port
  int servsck; // server socket
  struct sockaddr_in serv; // server address structure
  LIBSSH2_SESSION *session; // ssh session handler
  char *userauthlist;
  
  if (argc<3) {
    printf("Command format: sshpass username password\n");
    return -1;
  }
  
  username=argv[1];
  password=argv[2];

  // make a socket to the server
  servaddr=inet_addr("127.0.0.1");
  servport=htons(22);
  servsck=socket(AF_INET, SOCK_STREAM, 0);
  serv.sin_family=AF_INET;
  serv.sin_port=servport;
  serv.sin_addr.s_addr=servaddr;
  if (connect(servsck, (struct sockaddr*)(&serv), sizeof(struct sockaddr_in)) != 0) { 
    printf("Failed to connect to the ssh server!\n");
    return -1;
  }

  // create a ssh session over the socket
  session=libssh2_session_init();
  if (libssh2_session_startup(session, servsck)) {
    printf("Failed to establish a SSH session!\n");
    return -1;
  }
  
  // authenticate the user in the ssh session
  userauthlist=libssh2_userauth_list(session, username, strlen(username));
  if (strstr(userauthlist, "password") == NULL) {
    printf("The ssh server does not support password!\n");
    return -1;
  }
  if (libssh2_userauth_password(session, username, password)<0) {
    printf("Failed to authenticate the user!\n");
    return -1;
  }
  
  printf("Good!\n");

  libssh2_session_disconnect(session, "Get the password!");
  libssh2_session_free(session);

  return 0;
}
