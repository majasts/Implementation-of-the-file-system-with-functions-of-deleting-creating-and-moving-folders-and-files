#ifndef TREE_H
#define TREE_H

#include "File.h"

#include <fstream>
#include <string>
using namespace std;

class Tree {
public:
	Tree();
	Tree(const string& filename);
	~Tree();
	void addFile(string& name, File* elem,const string& filename, int& pos);
	void addFolder(string& name, File* elem);
	void addToTree(File* f, File* parent);
	void sortChild(vector <File*> &f);
	void sortTree(vector<File*> &f);
	File* searchTree(vector<File*> &f, string& text);
	File* searchTreeExe(vector<File*> f); 
	File::TypeC convertC(const string& str);
	string convertCtoString(File::TypeC c);
	void getComannd(string& info, const string& filename);
	void printTree(vector <File*> f, const string& filename, bool empty, File* curr);
	vector<File*> setCurrentDirectory(File* curr);
	int getPositionOfElem(vector <File*>c, File* curr);

private:
	vector <File*> root_;
	vector <File*> folders_;

};
#endif
