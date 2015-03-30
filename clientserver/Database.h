#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database{
	
	public:
		virtual ~Database();
		virtual bool createNewsGroup(const std::string& title) = 0;
		virtual std::vector<std::pair<int,std::string>> listNewsGroups() const = 0;
		virtual bool deleteNewsGroup(const std::string& title) = 0;
		virtual std::vector<std::string> listArticles(const std::string& newsGroup) const = 0;
		virtual bool createArticle(const int id, const std::string title, const std::string author, const std::string text) = 0;
		virtual bool deleteArticle() = 0;
		virtual std::string readArticle() const = 0;
};


#endif