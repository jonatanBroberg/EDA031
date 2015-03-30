#ifndef INMEMDB_H
#define INMEMDB_H

#include <map>
#include <vector>
#include <string>

class InMemDB{
	public:
		InMemDB();
		bool createNewsGroup(const std::string& title);
		std::vector<std::string> listNewsGroups() const;
		bool deleteNewsGroup(const std::string& title);
		std::vector<std::string> listArticles(const std::string& newsGroup) const;
		bool createArticle(const int id, const std::string title, const std::string author, const std::string text);
		bool deleteArticle();
		std::string readArticle() const;
	private:
		struct Article{
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
		int id;
};






#endif
