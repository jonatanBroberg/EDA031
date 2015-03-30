#ifndef INMEMDB_H
#define INMEMDB_H

#include <map>
#include <vector>
#include <string>
#include "Database.h"
#include <utility>

class InMemDB : public Database {
	public:
		InMemDB();
	//	virtual ~InMemDB();
		virtual bool createNewsGroup(const std::string& title);
		virtual std::vector<std::pair<int,std::string>> listNewsGroups() const;
		virtual bool deleteNewsGroup(const int& title);
		virtual std::vector<std::pair<int,std::string>> listArticles(const int& newsGroup) const;
		virtual bool createArticle(const int& id, const std::string& title, const std::string& author, const std::string& text);
		virtual int deleteArticle(int ngID, int artID);
		virtual std::vector<std::string> readArticle(int ngID, int artID) const;
	private:
		struct Article{
			Article(int id, std::string t, std::string a, std::string te) : articleID(id), title(t), author(a), text(te){} 
			int articleID;
			std::string title;
			std::string author;
			std::string text;
		};
		struct NewsGroup{
			NewsGroup(std::string t) : title(t){}
			std::string title;
			std::vector<Article> articles;
		};
		std::map<int, NewsGroup> newsGroups; //the int is the id which is unique for each group
		int newsGroupID;
};






#endif
