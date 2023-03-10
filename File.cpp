#include "File.h"

File::File()
{}

File::File(string name) : name_(name){}

File::File(string& name, string& info):name_(name), info_(info)
{
}

File::Type File::convert(const string& str)
{
	if (str == ".txt") return File::Type::txt;
	else if (str == ".exe") return File::Type::exe;
}


void File::setType(Type type)
{
	type_ = type;
}

File::Type File::getType()
{
	return type_;
}

string File::getName()
{
	return name_;
}

void File::setName(string name)
{
	name_ = name;
}

void File::setInfo(string info)
{
	info_ = info;
}

string File::getInfo()
{
	return info_;
}

string File::getExtension(string name)
{
	int dotpos = name.find('.');
	int spacepos = name.size();
	int extensionlen = spacepos - dotpos;
	string extension = name.substr(dotpos, extensionlen);
	return extension;

}

vector<File*> File::getAllChild() 
{
	return child_;
}

bool File::isExe()
{
	return type_ == Type::exe;
}


File* File::getParent()
{
	return parent_;
}

void File::emptyChild()
{
	child_.clear();
}

void File::setParent(File* parent)
{
	parent_ = parent;
}

void File::setChild(File*& child)
{
	child_.push_back(child);
}