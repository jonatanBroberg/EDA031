#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++


PROGS = client inmemserver fileserver

all: libclientserver.a $(PROGS)

client: client.o connection.o MessageHandler.o
<<<<<<< HEAD
myserver: myserver.o MessageHandler.o connection.o server.o InMemDB.o FileDB.o
#myserverOnDisk: myserver.o MessageHandler.o connection.o server.o InMemDB.o FileDB.o
=======
inmemserver: inmemserver.o MessageHandler.o connection.o server.o InMemDB.o
fileserver: fileserver.o MessageHandler.o connection.o server.o FileDB.o
>>>>>>> b59e4483cf86116db8651e971b885583e9074c44
# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.


libclientserver.a: connection.o server.o
	ar rv libclientserver.a  connection.o server.o
	ranlib libclientserver.a

# Phony targets
.PHONY: all clean install

:all $(PROGS)

# Install
install: all
	mkdir -p bin
	cp $(PROGS) ./bin

# Standard clean
clean:
	rm -f *.o libclientserver.a

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)
