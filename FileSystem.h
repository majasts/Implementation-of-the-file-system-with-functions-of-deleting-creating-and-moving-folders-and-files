#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include"Tree.h"

#include <string>
using namespace std;

class FileSystem {
public:
	void setOutput(const string& filename);
	string getOutput();
	void setTree(Tree* t);
	void loadHierarchy(const string& fs_filepath, const string& log_filepath);
	void execute(const string& filepath);
protected:
private:
	Tree* t_;
	string output_;
};


#endif

