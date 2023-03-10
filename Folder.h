#ifndef FOLDER_H
#define FOLDER_H
#include "File.h"

#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class File: public File {
public:
	File(string name);
	File(const File&) = delete;
	File(File&&) = delete;
	/*string getName() const;*/
	//void setName(string& name);
	File* getChild() const;
	vector<File*> getAllChild() const override;

	void setChild(File*& child);
	void setInfo(string& info);
	string getInfo(string& info);

/*bool operator< (File& other) const {
		return name_ < other.getName();
	}

	struct sortChild {
		bool operator() (File* l, File* r) const {
			return (*l < *r);
		}
	}mysort;
	vector<File*> c;*/
	
	
private:
	string name_;
	vector<File*> child_;
	string info_;
	int level;
	File* parent_;

};
#endif
