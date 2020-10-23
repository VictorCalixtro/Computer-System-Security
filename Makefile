DEBUG=-g3 -O0 -z execstack

all: tcpc tcps tcph attack
	@echo done
	
tcpc: tcpc.c
	gcc -Wall $(DEBUG) -o tcpc tcpc.c

tcps: tcps.c
	gcc -Wall $(DEBUG) -o tcps tcps.c

tcph: tcph.c
	gcc -Wall $(DEBUG) -o tcph tcph.c

attack: attack.c
	gcc -Wall $(DEBUG) -o attack attack.c

clean:
	rm -f tcpc tcps tcph attack bad.dat

install: all
	@su -c "./install.sh"
