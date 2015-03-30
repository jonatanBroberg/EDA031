#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <utility>

class Database{
	
	public:
	//	virtual ~Database();
		virtual bool createNewsGroup(const std::string& title) = 0;
		virtual std::vector<std::pair<int,std::string>> listNewsGroups() const = 0;
		virtual bool deleteNewsGroup(const int& title) = 0;
		virtual std::vector<std::pair<int,std::string>> listArticles(const int& newsGroup) const = 0;
		virtual bool createArticle(const int& id, const std::string& title, const std::string& author, const std::string& text) = 0;
		virtual int deleteArticle(int ngID, int artID) = 0;
		virtual std::vector<std::string> readArticle(int ngID, int artID) const = 0;
};


#endif
