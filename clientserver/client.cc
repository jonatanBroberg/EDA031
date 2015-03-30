#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

using namespace std;

void errMsgExit(string, ostream&);
int userInput(ostream&, istream&);
/*
 * The client program. 
 * Arguments: host_name, port_number
 */

int main(int argc, char* argv[]) {
  /*
   * Close the program if the user don't provide the host name and port number
   */
  if(argc != 3) {
    errMsgExit("Usage: myclient host_name port_number", cerr);
  }

  int port = -1;
  /*
   * Try to convert the provided port number from string to int
   * close the program if the port number isn't a valid one
   */
  try {
    port = stoi(argv[2]);
  }catch(exception& e) {
    errMsgExit("Wrong port number\n", cerr);
    //cerr << "Wrong port number. " << e.what() << endl;
    //exit(1);
  }

  /*
   * Create a connection. Close the program if it fails
   */
  Connection conn(argv[1], port);
  if(!conn.isConnected()) {
    /*
     * Uncomment when server exists
     errMsgExit("Connection attempt failed", cerr);
    */
    
    //For testing
    cerr << "Connection attempt failed" << endl;
  }
  /*
   * Commands: 1. List News group 2. Create newsgroup
   * 3. Delete newsgroup 4. List articles in current newsgroup
   * 5. Create article 6. Delete article 7. Exit
   */
  MessageHandler mh(conn);
  Protocol protocol;
  vector<pair<string, int>> newsGroups;
  cout << "Welcome!" << endl;
  while(true) {  
    string com;
	cin >> com;
    if(com == "listNG") {
		mh.sendCode(protocol.COM_LIST_NG);
		mh.sendCode(protocol.COM_END);
		if(mh.readCode() != protocol.ANS_LIST_NG){
			cerr << "Wrong answer from server" << endl;
			exit(1);
		}
		try{
			newsGroups.clear();
			int n = mh.readIntPar();
			for(int i = 1; i != n + 1; ++i) {
				int id = mh.readIntPar();
				string title = mh.readStringPar();
				newsGroups.push_back(make_pair(title, id));
				cout << i << ". " << title << endl;
			}
			if(mh.readCode() != protocol.ANS_END){
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}
		}catch(ConnectionClosedException e){
			exit(1);
		}
		
	}else if(com == "listART"){

	}else if(com == "createNG"){
		string name;
		cin >> name;
		if(name.size() == 0) {
			cerr << "Wrong input format." << endl;
		} else {
		 mh.sendCode(protocol.COM_CREATE_NG);
		mh.sendStringPar(name);
		mh.sendCode(protocol.COM_END);
		
		if(mh.readCode() == protocol.ANS_CREATE_NG){
			int answer = mh.readCode();
			if(answer == protocol.ANS_ACK){
				cout << name << " was added." << endl;
			}else if(answer == protocol.ANS_NAK){
				int errorCode = mh.readCode();
				cout << name << " already exist. " << endl;
			}else{
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}
		}else{
			cerr << "Wrong answer from server" << endl;
			exit(1);
		}
		if(mh.readCode() != protocol.ANS_END){
			cerr << "Wrong answer from server" << endl;
			exit(1);
		}
	}else if(com == "deleteNG"){

	}else if(com == "createART"){

	}else if(com == "deleteART"){

	}else if(com == "readART"){

	}else if(com == "exit"){

	}else{
		cerr << "Undefined commando..." << endl;
	}

    case 1:
		mh.sendCode(protocol.COM_LIST_NG);
		mh.sendCode(protocol.COM_END);
      break;
    case 2:
		
      //createNewsgroup();
      break;
    case 3:
      //deleteNewsgroup();
      break;
    case 4:
      //listArticles();
      break;
    case 5:
      //createArticle();
      break;
    case 6:
      //deleteArticle();
      break;
    case 7:
      exit(1);
      break;
    default:
      cerr << "Unknown command" << endl;
      break;
    }
  }
  return 0;
    
}

void errMsgExit(string errMsg, ostream& errOut) {
  errOut << errMsg << endl;
  exit(1);
}

int userInput(ostream& out, istream& in) {
  
  out << "1. List newsgroups\n2. Create newsgroup\n3. Delete newsgroup\n"
      << "4. List articles\n5. Create article\n6. Delete article\n7. Exit\n"
      << endl;
  string com1;
	
  
  return input;
  
}
