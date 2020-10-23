all: enc ssh

enc: enc2 dec2


enc2: enc2.c
	gcc -lssl -lcrypto -g -Wall -o enc2 enc2.c

dec2: dec2.c
	gcc -lssl -lcrypto -g -Wall -o dec2 dec2.c


clean:
	rm -rf  enc2 dec2 

