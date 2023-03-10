#ifndef FILE_H
#define FILE_H


#include <fstream>
#include <string>
#include <vector>

using namespace std;

class File {
public: 
	File();
	File(string name);
	File(string& name, string& info);
	//Tipovi fajlova
	enum Type{
		txt=0,
		exe=1,
		folder=10
	};

	//Tipovi komandi
	enum TypeC {
		LS, CD, NEW, DEL, EXE
	};
	
	Type convert(const string& str); 
	void setType(Type type);
	Type getType();
	string getName();
	void setName(string name);
	void setInfo(string info);
	string getInfo();
	string getExtension(string name);
	vector<File*> getAllChild();
	bool isExe();
	void setParent(File* parent);
	void setChild(File*& child);
	File* getParent();
	void emptyChild();
	

private:
	string info_;
	string name_;
	Type type_;
	File* parent_;
	vector<File*> child_;

};

#endif
