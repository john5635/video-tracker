GPP = g++ -g
CC = gcc -g

#include config.conf # file di configurazione da includere se si inserisce

CFLAGS= -I/usr/include/opencv #pkg-config --cflags opencv
LIBS=-lcxcore -lcv -lhighgui -lcvaux -lml #pkg-config --libs opencv

TARGET = main
RM = rm *.o


OBJ = extractBlob.o getBackground.o kalman.o main.o

#fase di linking
main: $(OBJ) 
	$(GPP) $(CFLAGS) $(OBJ) -o video-tracker  $(LIBS)

#fase di compilazione
main.o: 
	$(GPP) $(CFLAGS) -c main.cpp
	
extractBlob.o: 
	$(GPP) $(CFLAGS) -c extractBlob.cpp

getBackground.o: 
	$(GPP) $(CFLAGS) -c getBackground.cpp

kalman.o: 
	$(GPP) $(CFLAGS) -c kalman.cpp

main:        	$(TARGET)

all:            $(TARGET)

clean:
			$(RM)		     
clstart:
				rm main.o
love:
			  @echo "Not war!  (John Lennon)"
#install:
#			  
#			  @echo "copying the necessary library...  "
#			  @cp libiw.so.27 /lib
#			  @echo "I'm copying libiw.so27 in /lib"
#			  @cp -r /lib/iptables/ /usr/local/lib/
#			  @echo "I'm copying /lib/iptables/ in  /usr/local/lib/"
#			  @cp agent /sbin/
#			  @echo "I'm copying  agent in /sbin"
#			  @cp MeshAP1.5.dtd /sbin/
#			  @echo "I'm copying MeshAP1.5.dtd in /sbin"
#lib:
#			  @apt-get install iptables-dev libxml++2.6-dev gcc g++
#			  
#uninstall:
#			  @echo "Removing agents and its components...  "
#			  @rm /lib/libiw.so.27
#			  @echo "I'm removing /lib/libiw.so.27"
#			  @rm -r /usr/local/lib/iptables
#			  @echo "I'm removing /usr/local/lib/iptables"
#			  @rm  /sbin/agent
#			  @echo "I'm removing /sbin/agent"
#			  @rm  /sbin/MeshAP1.5.dtd
#			  @echo "I'm removing /sbin/MeshAP1.5.dtd"
#
