#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "MessageHandler.h"
#include "protocol.h"
#include "InMemDB.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}

	while(true) {
	  auto conn = server.waitForActivity();
	  if(conn != nullptr) {
	    try {
			MessageHandler mh(conn);
			int code = mh.readCode();
			Protocol protocol;
		    
		//	Database database;
			
			switch(code){
				case protocol.COM_LIST_NG:
					mh.sendCode(protocol.ANS_LIST_NG);
		//			database.listNewsGroups()
					mh.sendCode(protocol.ANS_END);
					break;
				case protocol.COM_CREATE_NG:
					mh.sendCode(protocol.ANS_CREATE_NG);
					//
					mh.sendCode(protocol.ANS_END);
					break;
				case protocol.COM_DELETE_NG:
					mh.sendCode(protocol.ANS_DELETE_NG);
				
					mh.sendCode(protocol.ANS_END);
					break;
				case protocol.COM_LIST_ART:
					mh.sendCode(protocol.ANS_LIST_ART);
				
					mh.sendCode(protocol.ANS_END);
					break;
				case protocol.COM_CREATE_ART:
					mh.sendCode(protocol.ANS_CREATE_ART);
				
					mh.sendCode(protocol.ANS_END);
					break;
				case protocol.COM_DELETE_ART:
						mh.sendCode(protocol.ANS_DELETE_ART);
				
					mh.sendCode(protocol.ANS_END);
					break;
				case protocol.COM_GET_ART:
					mh.sendCode(protocol.ANS_GET_ART);
				
					mh.sendCode(protocol.ANS_END);
					break;
				default:
					cerr << "There is no such commando, exiting..." << endl;
					exit(1);
			}
			
	    }catch(ConnectionClosedException&) {
	      server.deregisterConnection(conn);
	      cout << "Client closed connection" << endl;
			}
	  } else {
		conn = make_shared<Connection>();
		server.registerConnection(conn);
		cout << "New client connects" << endl;
	  }
	}
}
