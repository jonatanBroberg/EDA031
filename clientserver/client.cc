#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "connection.h"
#include "connectionclosedexception.h"

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
  cout << "Welcome!" << endl;
  while(true) {  
    int com = userInput(cout, cin);
    switch(com) {
    case 1: 
      //listNewsgroups(cout);
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
  int input;
  if((in >> input) < 0 || input > 8)
    input = -1;
  return input;
  
}
