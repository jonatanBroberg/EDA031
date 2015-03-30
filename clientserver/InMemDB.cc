#include "InMemDB.h"
#include <algorithm>
using namespace std;

InMemDB::InMemDB(){
	newsGroupID = 0;
}

bool InMemDB::createNewsGroup(const string& title){
	auto it = find_if(newsGroups.begin(), newsGroups.end(), [&title](pair<int, NewsGroup> p){return p.second.title == title;});
	if(it != newsGroups.end()){
		return false;
	}
	InMemDB::NewsGroup ng(title);
	newsGroups.insert(make_pair(++newsGroupID, ng));
	return true;
}
vector<pair<int,string>> InMemDB::listNewsGroups() const{
	vector<pair<int, string>> groups;
	for(auto tmp : newsGroups){
		groups.push_back(make_pair(tmp.first, tmp.second.title));	
	}
	return groups;
}
bool InMemDB::deleteNewsGroup(const int& ngID){
	auto it = newsGroups.find(ngID);
	if(it == newsGroups.end()){
		return false;
	}
	newsGroups.erase(it);
	return true;
}
vector<pair<int, string>> InMemDB::listArticles(const int& ngID) const{
	vector<pair<int, string>> articleList;
	auto it = newsGroups.find(ngID);
	if(it == newsGroups.end()){
		return articleList;
	}
	for(auto article : it->second.articles){
		articleList.push_back(make_pair(article.articleID, article.title));
	}
	return articleList;
}
bool InMemDB::createArticle(const int& ngID, const string& title, const string& author, const string& text){
	auto it = newsGroups.find(ngID);
	if(it == newsGroups.end()){
		return false;
	}
	InMemDB::Article article(it->second.articles.size() + 1, title, author, text);
	it->second.articles.push_back(article);
	return true;
}
bool InMemDB::deleteArticle(){
	return false;
}
string InMemDB::readArticle() const{
	return " ";
}
