#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <sstream>

#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "MessageHandler.h"

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

	shared_ptr<Connection> conn = make_shared<Connection>(argv[1], port);
	if(!(conn.get())->isConnected()) {
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
	//	conn = make_shared<Connection>();

	MessageHandler mh(conn);
	Protocol protocol;
	vector<pair<string, int>> newsGroups;
	pair<string, int> selectedNG = make_pair("",0);
	cout << "Welcome!" << endl;
	cout << "Available commands: \nlistNewsgroups \ncreateNewsgroup [name] \ndeleteNewsgroup [name] \nselect [newsgroup name] \nlistArticles \ncreateArticle [title author text] \ndeleteArticle [article ID] \nreadArticle[article ID] \nexit" << endl;
	while(true) {  
		string com;
		cin >> com;
	if(com == "listNewsgroups") {
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
				cout << id << ". " << title << endl;
			}
			if(mh.readCode() != protocol.ANS_END){
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}
		}catch(ConnectionClosedException e){
			exit(1);
		}

	}else if(com == "select"){
		string name, commandline;
		getline(cin, commandline);
		cout << "Name: ";
		getline(cin, name);
		auto it = find_if(newsGroups.begin(), newsGroups.end(), [&name](pair<string, int> p){return p.first == name;});
		if(it == newsGroups.end()){
			cout << name << " doesn't exist." << endl;
		}else{
			selectedNG = *it;
			cout << "Current newsgroup is: " << name << endl;
		}
	}else if(com == "listArticles"){
		if(selectedNG.second == 0){
			cout << "Please select a newsgroup." << endl;
		}else{
			mh.sendCode(protocol.COM_LIST_ART);
			mh.sendIntPar(selectedNG.second);
			mh.sendCode(protocol.COM_END);
			if(mh.readCode() != protocol.ANS_LIST_ART){
				cerr << "Wrong answer from server" << endl;
			}else{
				if(mh.readCode() == protocol.ANS_ACK){
					int n = mh.readIntPar();
					for(int i = 0; i != n; ++i){
						int id = mh.readIntPar();
						string name = mh.readStringPar();
						cout << id << ". " << name << endl;
					}
				}else{
					mh.readCode();
					cerr << selectedNG.first << " doesn't exist." << endl;
				}
				if(mh.readCode() != protocol.ANS_END){
					cerr << "Wrong answer from server" << endl;
					exit(1);
				}
			}
		}
	}else if(com == "createNewsgroup"){
		string name, commandline;
		getline(cin, commandline);
		cout << "Name: ";
		getline(cin, name);
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
		}
	}else if(com == "deleteNewsgroup"){
		string name, commandline;
		getline(cin, commandline);
		cout << "Name: ";
		getline(cin, name);
		if(name.size() == 0) {
			cerr << "Wrong input format" << endl;
		}else {
			auto it = find_if(newsGroups.begin(), newsGroups.end() , [&] (pair<string, int> p) {
				return p.first == name;});
			if(it == newsGroups.end()) {
				cerr << "No such newgroup!" << endl;
			}else {
				mh.sendCode(protocol.COM_DELETE_NG);
				mh.sendIntPar(it->second);
				mh.sendCode(protocol.COM_END);
				newsGroups.erase(it);
				if(mh.readCode() == protocol.ANS_DELETE_NG) {
					int answer = mh.readCode();
					if(answer == protocol.ANS_ACK) {
						cout << name << " was deleted" << endl;
					}else if(answer == protocol.ANS_NAK) {
						mh.readCode();
						cout << name << " doesn't exist" << endl;
					}else {
						cerr << "Wrong answer from server" << endl;
						exit(1);
					}

				}else {
					cerr << "Wrong answer from server" << endl;
					exit(1);
				}
				if(mh.readCode() != protocol.ANS_END) {
					cerr << "Wrong answer from server" << endl;
					exit(1);
				}
			}
		}
	}else if(com == "createArticle"){
		if(selectedNG.second == 0){
			cout << "Please select a newsgroup." << endl;
		}else{
			string title, author, text;
			/*	string line;
	getline(cin, line);
	cout << line << endl;
	stringstream ss(line);
	ss >> title;
	ss >> author;
	auto space1 = line.find(" ");
	string sub = line.substr(space1 + 1, string::npos);
	auto space2 = sub.find(" ");
	string sub2 = sub.substr(space2 + 1, string::npos);
	auto space3 = sub2.find(" ");
	text = sub2.substr(space3 + 1, string::npos);
			 */	

			cout << "Title: ";
			string commandline;
			getline(cin, commandline);
			getline(cin, title);
			cout << "Author: ";
			getline(cin, author);
			cout << "Text: ";
			getline(cin, text);

			mh.sendCode(protocol.COM_CREATE_ART);
			mh.sendIntPar(selectedNG.second);
			mh.sendStringPar(title);
			mh.sendStringPar(author);
			mh.sendStringPar(text);
			mh.sendCode(protocol.COM_END);
			if(mh.readCode() != protocol.ANS_CREATE_ART) {
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}
			if(mh.readCode() != protocol.ANS_ACK) {
				mh.readCode();
				cout << "No such newsgroup" << endl;
			}
			if(mh.readCode() != protocol.ANS_END) {
				cerr << "Server sucks" << endl;
				exit(1);
			}
		}
	}else if(com == "deleteArticle"){
		string artString;
		int art;
		cout << "ArticleID: ";
		cin >> artString;
		try{
			art = stoi(artString);
			mh.sendCode(protocol.COM_DELETE_ART);
			mh.sendIntPar(selectedNG.second);
			mh.sendIntPar(art);
			mh.sendCode(protocol.COM_END);
			if(mh.readCode() != protocol.ANS_DELETE_ART){
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}
			if(mh.readCode() != protocol.ANS_ACK) {
				int errCode = mh.readCode();
				if(errCode == protocol.ERR_NG_DOES_NOT_EXIST){
					cout << "Newsgroup doesn't exist" << endl;
				} else {
					cout << "Article doesn't exist" << endl;
				}
			}else {
				cout << "Article deleted" << endl;
			}
			if(mh.readCode() != protocol.ANS_END) {
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}
		} catch (invalid_argument e){
			cout << "articleID has to be a number" << endl;
		}
	}else if(com == "readArticle"){
		int art;
		string artString;
		cout << "AtricleID: ";
		cin >> artString;
		try{
			art = stoi(artString);
			mh.sendCode(protocol.COM_GET_ART);
			mh.sendIntPar(selectedNG.second);
			mh.sendIntPar(art);
			mh.sendCode(protocol.COM_END);
			if(mh.readCode() != protocol.ANS_GET_ART) {
				cerr << "Bad server" << endl;
				exit(1);
			}
			if(mh.readCode() != protocol.ANS_ACK) {
				int errCode = mh.readCode();
				if(errCode == protocol.ERR_NG_DOES_NOT_EXIST){
					cout << "Newsgroup doesn't exist" << endl;
				} else {
					cout << "Article doesn't exist" << endl;
				}
			}else {
				string title = mh.readStringPar();
				string author = mh.readStringPar();
				string text = mh.readStringPar();
				cout << "Title: " << title << "\n" << "Author: " << author << "\n\n" << text << endl;
			}
			if(mh.readCode() != protocol.ANS_END) {
				cerr << "Wrong answer from server" << endl;
				exit(1);
			}

		} catch (invalid_argument e){
			cout << "articleID has to be a number" << endl;
		}


	}else if(com == "exit"){
		cout << "Goodbye!" << endl;
		exit(0);

	}else{
		cerr << "Undefined commando..." << endl;
	}


	}
	return 0;

}

void errMsgExit(string errMsg, ostream& errOut) {
	errOut << errMsg << endl;
	exit(1);
}


