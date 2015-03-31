#include "FileDB.h"
#include <algorithm>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h> 
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <utility>
 
using namespace std;

FileDB::FileDB(){
	auto dir = opendir(root.c_str());
	newsGroupID = 0;
	if(dir == NULL){
		mkdir(root.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	} else {
		auto ent = readdir(dir);
		while(ent != NULL){
			try{
				string IDname = ent->d_name;
				cout << IDname << endl;
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
	}
	newsGroupID++;
}

/*FileDB::~FileDB(){
}
*/

bool FileDB::createNewsGroup(const string& title){
	
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
			string name = IDname.substr(pos + 1, string::npos);
			
			if(name == title){
				return false;
			}
			
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);
	string ng_path = (path + to_string(newsGroupID++) + " " + title);
	mkdir(ng_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);		
	return true;
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
	sort(groups.begin(), groups.end(), [] (pair<int, string> p1, pair<int, string> p2) {return p1.first < p2.first;});
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
	
	string path = root + "/";
	auto dir = opendir(path.c_str());
	if(dir == NULL){
		return articleList;
	}
	
	bool groupExists = false;
	auto entity = readdir(dir);
	while(entity != NULL){
		try{
			string IDname = entity->d_name;
			auto pos = IDname.find_first_of(" ");
			int ID = stoi(IDname.substr(0, pos)); 
			if(ID == ngID) {
				groupExists = true;
				string art_path = path + IDname + "/";
				auto ngDir = opendir(art_path.c_str());
				if(ngDir == NULL){
					return articleList;
				}
				auto art_entity = readdir(ngDir);
				while(art_entity != NULL){
					try{
						string artIDString = art_entity->d_name;
						auto art_pos = artIDString.find_first_of(" ");
						int artID = stoi(artIDString.substr(0, art_pos)); 
						string artFilename = artIDString.substr(art_pos + 1, string::npos);
						auto ext_pos = artFilename.find_last_of(".");
						string artName = artFilename.substr(0, ext_pos);
						articleList.push_back({artID, artName});
					} catch (invalid_argument e) {
						cerr << "Invalid folders in Folder Newsgroup" << endl;
					}
					art_entity = readdir(ngDir);
				}
				closedir(ngDir);						
				
			} 
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);
	if(!groupExists){
		articleList.push_back({0, ""});
	}	
	sort(articleList.begin(), articleList.end(), [] (pair<int, string> p1, pair<int, string> p2) {return p1.first < p2.first;});
	return articleList;
}

bool FileDB::createArticle(const int& ngID, const string& title, const string& author, const string& text){
	
	//Find largest last used ID	
	int max_artID = 0;
	
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
				//Find largest ID
				string art_path = path + IDname + "/";
				auto ngDir = opendir(art_path.c_str());
				if(ngDir == NULL){
					return false;
				}
				auto art_entity = readdir(ngDir);
				while(art_entity != NULL){
					string artIDString = art_entity->d_name;
					try{
						int artID = stoi(artIDString); 
						if(artID > max_artID){
							max_artID = artID;
						}
					} catch (invalid_argument e) {
						cerr << "Invalid folders in Folder Newsgroup" << endl;
					}
					art_entity = readdir(ngDir);
				}
				closedir(ngDir);		
				//Write to file
				ofstream writer(art_path + to_string(++max_artID) + " " + title + ".txt");
				writer << author << '\n';
				writer << text;
				writer.close();
				return true;
			}
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);
	return false;
}

int FileDB::deleteArticle(int ngID, int artID){

	string path = root + "/";
	auto dir = opendir(path.c_str());
	if(dir == NULL){
		return -1;
	}
	
	auto entity = readdir(dir);
	while(entity != NULL){
		try{
			string IDname = entity->d_name;
			auto pos = IDname.find_first_of(" ");
			int ID = stoi(IDname.substr(0, pos)); 
			if(ID == ngID) {
				string art_path = path + IDname + "/";
				auto ngDir = opendir(art_path.c_str());
				if(ngDir == NULL){
					return -1;
				}
				auto art_entity = readdir(ngDir);
				while(art_entity != NULL){
					try{
						string artIDString = art_entity->d_name;
						auto art_pos = artIDString.find_first_of(" ");
						int art_ID = stoi(artIDString.substr(0, art_pos)); 					
						if(artID == art_ID) {
							return remove((art_path + artIDString).c_str()) == 0 ? 1 : -2 ;
						}
					} catch (invalid_argument e) {
						cerr << "Invalid folders in Folder Newsgroup" << endl;
					}
					art_entity = readdir(ngDir);
				}
				closedir(ngDir);							
				return -2;
			} 
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);	
	return -1;		//Newsgroup does not exits
}

vector<string> FileDB::readArticle(int ngID, int artID) const{
	vector<string> artInfo;
	
	string path = root + "/";
	auto dir = opendir(path.c_str());
	if(dir == NULL){
		return artInfo;
	}
	
	auto entity = readdir(dir);
	while(entity != NULL){
		try{
			string IDname = entity->d_name;
			auto pos = IDname.find_first_of(" ");
			int ID = stoi(IDname.substr(0, pos)); 
			if(ID == ngID) {
				string art_path = path + IDname + "/";
				auto ngDir = opendir(art_path.c_str());
				if(ngDir == NULL){
					return artInfo;
				}
				auto art_entity = readdir(ngDir);
				while(art_entity != NULL){
					try{
						string artIDString = art_entity->d_name;
						auto art_pos = artIDString.find_first_of(" ");
						int art_ID = stoi(artIDString.substr(0, art_pos)); 		
						if(artID == art_ID) {
							string artFilename = artIDString.substr(art_pos + 1, string::npos);
							auto ext_pos = artFilename.find_last_of(".");
							string artName = artFilename.substr(0, ext_pos);
							artInfo.push_back(artName);
							ifstream reader((art_path + artIDString));
							string line;
							if(getline(reader, line)){
								artInfo.push_back(line);
								string text;
								if(getline(reader,line)){
									text += line;
									int count = 0;
									while(getline(reader, line)){
										text += "\n" + line;	
										count++;			
									}		
									if(count > 1){
										text += "\n";
									}
								}
								artInfo.push_back(text);								
							}
							return artInfo;
						}
					} catch (invalid_argument e) {
						cerr << "Invalid folders in Folder Newsgroup" << endl;
					}
					art_entity = readdir(ngDir);
				}
				closedir(ngDir);
				artInfo.push_back(" ");					
				return artInfo;
			} 
		} catch (invalid_argument e) {
			cerr << "Invalid folders in Folder Newsgroup" << endl;
		}
		entity = readdir(dir);
	}
	closedir(dir);	
	return artInfo;
}
