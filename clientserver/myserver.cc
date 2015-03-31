#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "MessageHandler.h"
#include "protocol.h"
#include "InMemDB.h"
#include "FileDB.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>

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

	InMemDB db;
	FileDB dDB;
	Protocol protocol;
			
	while(true) {
	  auto conn = server.waitForActivity();
	  if(conn != nullptr) {
	    try {
	    	MessageHandler mh(conn);
			int code = mh.readCode();
			
			switch(code){
				case protocol.COM_LIST_NG:
					{
						vector<pair<int, string>> NGs = db.listNewsGroups();
						mh.sendCode(protocol.ANS_LIST_NG);
						mh.sendIntPar(NGs.size());
						for_each(NGs.begin(), NGs.end(), [&mh] (pair<int, string> p) {mh.sendIntPar(p.first); mh.sendStringPar(p.second);});
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_CREATE_NG:
					{
						string name = mh.readStringPar();
						mh.sendCode(protocol.ANS_CREATE_NG);
						if(db.createNewsGroup(name)){
							mh.sendCode(protocol.ANS_ACK);							
						} else {
							mh.sendCode(protocol.ANS_NAK);
							mh.sendCode(protocol.ERR_NG_ALREADY_EXISTS);
						}
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_DELETE_NG:
					{
						int ngID = mh.readIntPar();
						mh.sendCode(protocol.ANS_DELETE_NG);
						if(db.deleteNewsGroup(ngID)){
							mh.sendCode(protocol.ANS_ACK);
						} else {
							mh.sendCode(protocol.ANS_NAK);
							mh.sendCode(protocol.ERR_NG_DOES_NOT_EXIST);
						}
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_LIST_ART:
					{
						int ngID = mh.readIntPar();
						mh.sendCode(protocol.ANS_LIST_ART);
						vector<pair<int,string>> arts = db.listArticles(ngID);
						if(arts.size() == 1 && arts[0].first == 0){
							mh.sendCode(protocol.ANS_NAK);
							mh.sendCode(protocol.ERR_NG_DOES_NOT_EXIST);
						} else {
							mh.sendCode(protocol.ANS_ACK);
							mh.sendIntPar(arts.size());
							for_each(arts.begin(), arts.end(), [&mh] (pair<int,string> p) {mh.sendIntPar(p.first); mh.sendStringPar(p.second);});
						}
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_CREATE_ART:
					{
						int ngID = mh.readIntPar();
						string title = mh.readStringPar();
						string author = mh.readStringPar();
						string text = mh.readStringPar();
						mh.sendCode(protocol.ANS_CREATE_ART);
						if(db.createArticle(ngID, title, author, text)){
							mh.sendCode(protocol.ANS_ACK);
						} else {
							mh.sendCode(protocol.ANS_NAK);
							mh.sendCode(protocol.ERR_NG_DOES_NOT_EXIST);
						}
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_DELETE_ART:
					{
						int ngID = mh.readIntPar();
						int artID = mh.readIntPar();
						int result = db.deleteArticle(ngID, artID);
						mh.sendCode(protocol.ANS_DELETE_ART);
						if(result == 1){
							mh.sendCode(protocol.ANS_ACK);
						} else {
							mh.sendCode(protocol.ANS_NAK);
							if(result == -1) {
								mh.sendCode(protocol.ERR_NG_DOES_NOT_EXIST);
							} else {
								mh.sendCode(protocol.ERR_ART_DOES_NOT_EXIST);	
							}
						}
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_GET_ART:
					{
						int ngID = mh.readIntPar();
						int artID = mh.readIntPar();
						mh.sendCode(protocol.ANS_GET_ART);
						vector<string> art = db.readArticle(ngID, artID);
						if(art.size() == 3){
							mh.sendCode(protocol.ANS_ACK);
							for_each(art.begin(), art.end(), [&mh] (string s) {mh.sendStringPar(s);});
						} else {
							mh.sendCode(protocol.ANS_NAK);
							if(art.size() == 0) {
								mh.sendCode(protocol.ERR_NG_DOES_NOT_EXIST);
							} else {
								mh.sendCode(protocol.ERR_ART_DOES_NOT_EXIST);	
							}
						}
						mh.sendCode(protocol.ANS_END);
					}
					break;
				case protocol.COM_END:
				
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
