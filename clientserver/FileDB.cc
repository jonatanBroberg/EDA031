#include "FileDB.h"
#include <algorithm>
#include <string>
#include <sys/stat.h>
 #include <sys/types.h>
 
using namespace std;

FileDB::FileDB(){
	root = "/home/testfolderForProjectInCPP";
	mkdir("/tmp/testfolderForProjectInCPP", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

/*FileDB::~FileDB(){
}
*/
bool FileDB::createNewsGroup(const string& title){
	return true;
}

vector<pair<int,string>> FileDB::listNewsGroups() const{
	vector<pair<int,string>> groups;
	return groups;
}

bool FileDB::deleteNewsGroup(const int& ngID){
	return true;
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
