#pragma once
#include<exception>
using namespace std;

class ETryFolder :public exception {
public:
	ETryFolder() : exception() {}
	ETryFolder(const char* msg) : exception(msg) {}
};

class ENewFolder :public exception {

};

class EFile : public exception {

};
class ECommand :public exception { 
public:
	ECommand():exception(){}
	ECommand(const char* msg):exception(msg){}
};

class EInput :public exception {
public:
	EInput(const char* msg):exception(msg){}
};
