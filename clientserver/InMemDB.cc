#include "InMemDB.h"
#include <algorithm>
using namespace std;

InMemDB::InMemDB(){
	
}

bool InMemDB::createNewsGroup(const string& title){
	auto it = find_if(newsGroups.begin(), newsGroups.end(), [&title](pair<int, NewsGroup> p){return p.second.title == title;});
	if(it != newsGroups.end()){
		return false;
	}
	InMemDB::NewsGroup ng(title);
	++id;
	newsGroups.insert(make_pair(id, ng));
	return true;
}
vector<string> InMemDB::listNewsGroups() const{
	vector<string> tmp(2, " test ");
	return tmp;
}
bool InMemDB::deleteNewsGroup(const string& title){
	return false;
}
vector<string> InMemDB::listArticles(const string& newsGroup) const{
	vector<string> tmp(2, " test ");
	return tmp;
}
bool InMemDB::createArticle(const int id, const string title, const string author, const string text){
	return false;
}
bool InMemDB::deleteArticle(){
	return false;
}
string InMemDB::readArticle() const{
	return " ";
}
