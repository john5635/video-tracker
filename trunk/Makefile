GPP = g++ -g
CC = gcc -g

#WIN32=Y    #Parametro che se messo a vero, imposta delle parti di codice per marto in Win32 ora come ora solo in getFrames
#include config.conf # file di configurazione da includere se si inserisce

CFLAGS=  `pkg-config --cflags opencv`        #-I/usr/include/opencv
LIBS= `pkg-config --libs opencv`             #-lcxcore -lcv -lhighgui -lcvaux -lml

wxCFLAGS=`wx-config --cflags`
wxLIBS=`wx-config --libs`

TARGET = video-tracker
RM = rm *.o video-tracker getFrames


ifdef WIN32
wxCFLAGS += -DWIN32
endif


OBJ = extractBlob.o getBackground.o kalman.o main.o

#fase di linking
video-tracker: $(OBJ) 
	$(GPP) $(CFLAGS) $(OBJ) -o $@  $(LIBS)

#fase di compiling
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

getFrames:
	$(GPP) $(CFLAGS) $(wxCFLAGS)  -s getFrames.cpp $(LIBS) $(wxLIBS) -o $@
				     

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
