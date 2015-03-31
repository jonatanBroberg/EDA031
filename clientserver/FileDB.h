#ifndef FILEDB_H
#define FILEDB_H

#include <map>
#include <vector>
#include <string>
#include "Database.h"
#include <utility>

class FileDB : public Database {
	public:
		FileDB();
	//	virtual ~FileDB();
		virtual bool createNewsGroup(const std::string& title);
		virtual std::vector<std::pair<int,std::string>> listNewsGroups() const;
		virtual bool deleteNewsGroup(const int& ngID);
		virtual std::vector<std::pair<int,std::string>> listArticles(const int& ngID) const;
		virtual bool createArticle(const int& id, const std::string& title, const std::string& author, const std::string& text);
		virtual int deleteArticle(int ngID, int artID);
		virtual std::vector<std::string> readArticle(int ngID, int artID) const;
	private:
		std::char* root;
		
};






#endif
