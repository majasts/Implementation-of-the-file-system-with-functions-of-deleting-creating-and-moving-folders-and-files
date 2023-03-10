#include "FileSystem.h"

#include <sstream>
void FileSystem::loadHierarchy(const string& fs_filepath, const string& log_filepath)
{	
	Tree* t = new Tree(fs_filepath);
	setTree(t);
	setOutput(log_filepath);

}

void FileSystem::execute(const string& filepath)
{
	ifstream command(filepath);
	stringstream c;
	c << command.rdbuf(); command.close();
	string info = c.str();
	t_->getComannd(info, output_);
	t_->~Tree();
}

void FileSystem::setOutput(const string& output) {
	output_ = output;
}

string FileSystem::getOutput()
{
	return output_;
}

void FileSystem::setTree(Tree* t)
{
	t_ = t;
}

