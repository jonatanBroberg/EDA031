#ifndef INMEMDB_H
#define INMEMDB_H

#include <map>
#include <vector>
#include <string>
#include "Database.h"

class InMemDB : public Database{
	public:
		InMemDB();
		virtual bool createNewsGroup(const std::string& title);
		virtual std::vector<std::pair<int,std::string>> listNewsGroups() const;
		virtual bool deleteNewsGroup(const int& title);
		virtual std::vector<std::pair<int,std::string>> listArticles(const int& newsGroup) const;
		virtual bool createArticle(const int id, const std::string title, const std::string author, const std::string text);
		virtual bool deleteArticle();
		virtual std::string readArticle() const;
	private:
		struct Article{
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