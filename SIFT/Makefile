CXX	= g++
#OBJS	= main.o sift.o server.o database.o version.o
SOBJS	= server.o network.o sift.o database.o version.o
COBJS	= client.o network.o
CFLAGS	= `pkg-config --cflags --libs opencv` `mysql_config --include --libs`
VPATH	= src

#sift: $(OBJS)
#	$(CXX) -o $@ $(OBJS) $(CFLAGS)

server: $(SOBJS)
	$(CXX) -o $@ $(SOBJS) $(CFLAGS)

client: $(COBJS)
	$(CXX) -o $@ $(COBJS) $(CFLAGS)

.cc.o:
	$(CXX) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f server client *.o
