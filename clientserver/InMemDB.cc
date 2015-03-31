#include "InMemDB.h"
#include <algorithm>
using namespace std;

InMemDB::InMemDB(){
	newsGroupID = 0;
}

/*InMemDB::~InMemDB(){
}
*/
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
	  articleList.push_back(make_pair(0,""));
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
int InMemDB::deleteArticle(int ngID, int artID){
	auto ng = newsGroups.find(ngID);
	if(ng == newsGroups.end()){
		return -1;//NewsGroup does not exist
	}
	auto article = find_if(ng->second.articles.begin(), ng->second.articles.end(), [&artID](Article art){return art.articleID == artID;});
	if(article == ng->second.articles.end()){
		return -2;//Article does not exist
	}
	ng->second.articles.erase(article);
	return 1;
}
vector<string> InMemDB::readArticle(int ngID, int artID) const{
	vector<string> artInfo;
	auto ng = newsGroups.find(ngID);
	if(ng == newsGroups.end()){
		return artInfo;//NewsGroup does not exist
	}
	auto article = find_if(ng->second.articles.begin(), ng->second.articles.end(), [&artID](Article art){return art.articleID == artID;});
	if(article == ng->second.articles.end()){
		artInfo.push_back(" ");
		return artInfo;//Article does not exist
	}
	artInfo.push_back(article->title);
	artInfo.push_back(article->author);
	artInfo.push_back(article->text);
	return artInfo;
}
