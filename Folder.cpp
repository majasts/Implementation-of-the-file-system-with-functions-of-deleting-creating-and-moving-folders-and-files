#include "Folder.h"

Folder::Folder(string name):name_(name){}

//string Folder::getName() const
//{
//	return name_;
//}

//void Folder::setName(string& name)
//{
//	name_ = name;
//}

File* Folder::getChild() const
{
	return child_.front();
}

vector<File*> Folder::getAllChild() const
{
	return child_;
}

void Folder::setChild(File*& child)
{
	child_.push_back(child);
}

void Folder::setInfo(string& info)
{ 
	info_= info;
}

string Folder::getInfo(string& info)
{
	return info_;
}
