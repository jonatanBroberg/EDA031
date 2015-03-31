#include "FileDB.h"
#include <algorithm>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h> 
#include <iostream>
#include <stdlib.h>
 
using namespace std;

FileDB::FileDB(){
	auto dir = opendir(root.c_str());
	newsGroupID = 1;
	if(dir == NULL){
		mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	} else {
		auto ent = readdir(dir);
		while(ent != NULL){
			try{
				string IDname = ent->d_name;
				auto pos = IDname.find_first_of(" ");
				int ngID = stoi(IDname.substr(0, pos));
				if(ngID > newsGroupID){
					newsGroupID = ngID;
				}
			} catch (invalid_argument e){
				cerr << "Invalid folders in Folder Newsgroup" << endl;
			}
			ent = readdir(dir);
		}
		closedir(dir);
		newsGroupID++;
	}
}

/*FileDB::~FileDB(){
}
*/

bool FileDB::createNewsGroup(const string& title){
	string id = to_string(newsGroupID++);
	string path = root + "/" + id + " " + title;
	auto dir = opendir(path.c_str());
	if(dir == NULL){
		mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);		
		return true;
	}
	closedir(dir);
	return false;
}

vector<pair<int,string>> FileDB::listNewsGroups() const{
	vector<pair<int,string>> groups;
	
	string path = root + "/";
	auto dir = opendir(path.c_str());
	if(dir == NULL){
		return groups;
	}
	
	auto entity = readdir(dir);
	while(entity != NULL){
		try{
			string IDname = entity->d_name;
			auto pos = IDname.find_first_of(" ");
			int ngID = stoi(IDname.substr(0, pos)); 
			string name = IDname.substr(pos + 1, string::npos);
			groups.push_back({ngID, name});
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);
	return groups;
}

bool FileDB::deleteNewsGroup(const int& ngID){
	string path = root + "/";
	auto dir = opendir(path.c_str());
	if(dir == NULL){
		return false;
	}
	
	auto entity = readdir(dir);
	while(entity != NULL){
		try{
			string IDname = entity->d_name;
			auto pos = IDname.find_first_of(" ");
			int ID = stoi(IDname.substr(0, pos)); 
			if(ID == ngID) {
				//Remove dir
				return remove((path + IDname).c_str()) == 0;
			}
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);
	return false;
}

vector<pair<int, string>> FileDB::listArticles(const int& ngID) const{
	vector<pair<int, string>> articleList;

	return articleList;
}

bool FileDB::createArticle(const int& ngID, const string& title, const string& author, const string& text){

	return true;
}

int FileDB::deleteArticle(int ngID, int artID){
	return 1;
}

vector<string> FileDB::readArticle(int ngID, int artID) const{
	vector<string> artInfo;
	return artInfo;
}
