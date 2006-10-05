GPP = g++ -g
CC = gcc -g

#include manager.conf # file di configurazione da includere se si inserisce

CFLAGS= -I/usr/include/opencv #pkg-config --cflags opencv
LIBS=-lcxcore -lcv -lhighgui -lcvaux -lml #pkg-config --libs opencv

TARGET = main
RM = rm *.o


OBJ = extractBlog.o getBackground.o kalman.o main.o

#fase di linking
main: $(OBJ) 
	$(GPP) $(OBJ)  $(LIBS)

#fase di compilamento
agent.o: 
	$(GPP) $(CFLAGS) -c main.cpp
	
extractBlob.o: debug.o Manager.cpp
	$(GPP) $(CFLAGS) 	-c Manager.cpp
	
ContextOperation.o: ContextOperation.cpp 
	$(GPP) $(LIBXML) -c ContextOperation.cpp
	
StateOperation.o: StateOperation/StateOperation.cpp 
	$(GPP)	-c StateOperation/StateOperation.cpp
	
Parser.o: Parser/Parser.cpp 
	$(GPP)	-c Parser/Parser.cpp

VectorFactory.o:  debug.o VectorFactory.cpp
	$(GPP) $(LIBXML) $(CFLAGS) -c VectorFactory.cpp

ParserDaemon.o: Parser/ParserDaemon.cpp 
	$(GPP)	 -c Parser/ParserDaemon.cpp


PacketManager.o: PacketManager.cpp 
	$(GPP)	-c PacketManager.cpp

debug.o: debug.c 
	$(CC)	-c debug.c

FirewallOperation.o: StateOperation/FirewallOperation.cpp 
	$(GPP) $(LIBXML)	 -c StateOperation/FirewallOperation.cpp

isAliveOperation.o: StateOperation/isAliveOperation.cpp 
	$(GPP) $(LIBXML)	 -c StateOperation/isAliveOperation.cpp

ListeningOperation.o: StateOperation/ListeningOperation.cpp 
	$(GPP) $(LIBXML) -c StateOperation/ListeningOperation.cpp

RoutingOperation.o: StateOperation/RoutingOperation.cpp routeList.o
	$(GPP) $(LIBXML) -c StateOperation/RoutingOperation.cpp

routeList.o: StateOperation/RoutingOperation/routeList.cpp route-xmlwriter.o
	$(GPP)  -c StateOperation/RoutingOperation/routeList.cpp

route-xmlwriter.o: StateOperation/RoutingOperation/route-xmlwriter.cpp
	$(GPP) $(LIBXML) -c StateOperation/RoutingOperation/route-xmlwriter.cpp

IdsOperation.o: StateOperation/IdsOperation.cpp ids-xmlparser.o
	$(GPP) $(LIBXML) -c StateOperation/IdsOperation.cpp

ids-xmlparser.o: StateOperation/IdsOperation/ids-xmlparser.cpp
	$(GPP) $(LIBXML) -c StateOperation/IdsOperation/ids-xmlparser.cpp

deauthfloodParser.o: StateOperation/IdsOperation/deauthfloodParser.cpp ids-xmlwriter.o
	$(GPP) $(LIBXML) -c StateOperation/IdsOperation/deauthfloodParser.cpp

ids-xmlwriter.o: StateOperation/IdsOperation/ids-xmlwriter.cpp
	$(GPP) $(LIBXML) -c StateOperation/IdsOperation/ids-xmlwriter.cpp	

Cipher.o: Cipher/Cipher.cpp 
	$(GPP)	-c Cipher/Cipher.cpp

NullCipher.o: Cipher/NullCipher.cpp 
	$(GPP)	-c Cipher/NullCipher.cpp

UserInterface.o: UserInterface/UserInterface.cpp Manager.cpp
	$(GPP)	-c UserInterface/UserInterface.cpp

TextInterface.o: UserInterface/TextInterface.cpp 
	$(GPP)	-c UserInterface/TextInterface.cpp

XmlHandler.o: XmlHandler.cpp
	$(GPP) $(LIBXML) $(CFLAGS) -c XmlHandler.cpp

fw-xmlwriter.o: StateOperation/FirewallOperation/fw-xmlwriter.cpp
	$(CC) $(LIBXML) -c StateOperation/FirewallOperation/fw-xmlwriter.cpp


fw-xmlparser.o: StateOperation/FirewallOperation/fw-xmlparser.cpp
	$(CC) $(CFLAGS_IPTABLES) $(LIBXML) -c StateOperation/FirewallOperation/fw-xmlparser.cpp

iptables.o: StateOperation/FirewallOperation/iptables.c fw-xmlwriter.o
	$(CC) $(CFLAGS_IPTABLES)  -c StateOperation/FirewallOperation/iptables.c

iptables-restore.o: StateOperation/FirewallOperation/iptables-restore.c fw-xmlparser.o iptables.o $(STATIC_LIBS)libiptc.a
	$(CC) $(CFLAGS_IPTABLES) $(LIBXML) -DIPT_LIB_DIR=\"$(IPT_LIBDIR)\" -c StateOperation/FirewallOperation/iptables-restore.c 


WirelessExtensionsOperation.o: StateOperation/WirelessExtensionsOperation.cpp iwconfig.o we-xmlparser.o
	$(GPP) $(CFLAGS_WE) $(MMD) $(LIBXML)  -c  StateOperation/WirelessExtensionsOperation.cpp

iwconfig.o: StateOperation/WirelessExtensionsOperation/iwconfig.c we-xmlwriter.o
	$(CC) $(CFLAGS_WE) $(MMD) -c StateOperation/WirelessExtensionsOperation/iwconfig.c 	
	
iwlib.so:  StateOperation/WirelessExtensionsOperation/iwlib.c
	$(CC) $(CFLAGS_WE) $(MMD) -c -o iwlib.so  StateOperation/WirelessExtensionsOperation/iwlib.c

libiw.so.27: iwlib.so
	$(CC) $(SHAREFLAGS) iwlib.so

we-xmlparser.o:  StateOperation/WirelessExtensionsOperation/we-xmlparser.cpp
	$(GPP) $(LIBXML) -c  StateOperation/WirelessExtensionsOperation/we-xmlparser.cpp
	
we-xmlwriter.o:  StateOperation/WirelessExtensionsOperation/we-xmlwriter.cpp
	$(GPP) $(LIBXML) -c  StateOperation/WirelessExtensionsOperation/we-xmlwriter.cpp


main:        	$(TARGET)

all:            $(TARGET)

clean:
			$(RM)		     
clstart:
				rm agent.o
love:
			  @echo "Not war!  (John Lennon)"
install:
			  
			  @echo "copying the necessary library...  "
			  @cp libiw.so.27 /lib
			  @echo "I'm copying libiw.so27 in /lib"
			  @cp -r /lib/iptables/ /usr/local/lib/
			  @echo "I'm copying /lib/iptables/ in  /usr/local/lib/"
			  @cp agent /sbin/
			  @echo "I'm copying  agent in /sbin"
			  @cp MeshAP1.5.dtd /sbin/
			  @echo "I'm copying MeshAP1.5.dtd in /sbin"
lib:
			  @apt-get install iptables-dev libxml++2.6-dev gcc g++
			  
uninstall:
			  @echo "Removing agents and its components...  "
			  @rm /lib/libiw.so.27
			  @echo "I'm removing /lib/libiw.so.27"
			  @rm -r /usr/local/lib/iptables
			  @echo "I'm removing /usr/local/lib/iptables"
			  @rm  /sbin/agent
			  @echo "I'm removing /sbin/agent"
			  @rm  /sbin/MeshAP1.5.dtd
			  @echo "I'm removing /sbin/MeshAP1.5.dtd"
