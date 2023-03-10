#include "Tree.h"
#include "Error.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Tree::Tree() {}


Tree::Tree(const string& filename)
{
	fstream input(filename, ios::in);
	char c;
	string text;
	File* curr = nullptr;
	File* node = nullptr;


	while (input.peek() != EOF) {
		try {
			c = input.get();
			if (text.size() > 1) {
				if (c == ' ') throw new EFile();  //Making new file
				else if (c == '\n') throw new ENewFolder(); //Making new folder
				else if (c == '\\') throw new ETryFolder(); //Finding folder that is already made and setting it to current directory
			}
			text.push_back(c);

		}
		catch (EFile*) {
			int pos = input.tellg(); //Saving position in input file because we have to keep reading from it to find information that is stored in new file

			addFile(text, curr, filename, pos);
			text.clear();
			curr = nullptr;
			input.seekg(pos);

		}
		catch (ENewFolder*) {
			addFolder(text, curr);
			text.clear();
			curr = nullptr;
		}
		catch (ETryFolder*) {
			try {
				if (curr == nullptr) {
					//If there isnt current directory we look for new one in root vector
					curr = searchTree(root_, text);
					if (curr == nullptr) {
						throw EInput("Ulaz je bio neispravan");
						return;
					}
				}
				else {
					//If there is current directory we look for new current directory in child vector of current
					vector<File*>f = curr->getAllChild();
					curr = searchTree(f, text);
					if (curr == nullptr) {
						throw EInput("Ulaz je bio neispravan ");
						return;
					}
				}
			}
			catch(EInput &e){
				cout << e.what() << endl;
				return;
			}
			text.clear();
			text.push_back(c);
		}
	}
	//After loading information abaout tree we sort it alphabetically
	sortTree(root_);
}


Tree::~Tree()
{
	vector<File*> ::iterator it;

	vector<File*> unvisited;
	//If element of root has child add it to new vector -unvisted-, if it doesnt have child delete it
	for (it = root_.begin(); it != root_.end(); ) {
		File* tmp = *it;
		if (tmp->getAllChild().empty()) {
			delete(*it);
			it=root_.erase(it);
		}
		else {
			unvisited.push_back(*it);
			++it;
		}
	}
	//Now iterating through -unvisited- we delete elements that dont have child; for elements that have
	//child we first save their child in vector unvisited and then delete them
	while (!unvisited.empty() ) {
		
		for (int i = 0; i < unvisited.size();i++) {
			if (unvisited[i]->getAllChild().empty()){
				delete(unvisited[i]);
				unvisited.erase(unvisited.begin() + i);
			}
			else {
				vector<File*> c = unvisited[i]->getAllChild();
				unvisited[i]->emptyChild();
				for (int j = 0; j < c.size();j++ ) {
					unvisited.push_back(c[j]);	
				}
			}
		}
	}
	unvisited.clear();
	root_.clear();
}

void Tree::addFile(string& name, File* elem, const string& filename, int& pos)
{

	char c;
	fstream input(filename, ios::in);
	string info;
	//Reading the content of file:
	input.seekg(pos);
	while (input.peek() != EOF) {
		input.get(c);
		info.push_back(c);
		if (c == '\n') {
			if (input.peek() == '\\' || input.peek() == EOF) break;
		}
	}
	pos = input.tellg();
	//Making new file add making it part of tree:
	File* f = new File(name, info);
	File::Type t = f->convert(f->getExtension(name));
	f->setType(f->convert(f->getExtension(name)));
	f->setInfo(info);

	addToTree(f, elem);
}

void Tree::addFolder(string& name, File* elem)
{
	File* f = new File(name);
	f->setType(File::Type::folder);
	addToTree(f, elem);
	folders_.push_back(f);
}

void Tree::addToTree(File* f, File* parent)
{
	//If pointer elem is nullptr loaded file is root file, 
	//if elem points to folder than loaded file is child of that folder 
	if (parent == nullptr) {
		root_.push_back(f);
		f->setParent(nullptr);
	}
	else {
		parent->setChild(f);
		f->setParent(parent);
	}
}

void Tree::sortChild(vector<File*>& f)
{
	string name1, name2;
	File* tmp;
	vector<File*> ::iterator it;
	vector<File*>::iterator itt;
	//Iterating through vector of child and sorting it alphabetically by name
	for (it = f.begin(); it != f.end(); ++it) {
		for (itt = it; itt != f.end(); ++itt) {
			name1 = (*it)->getName();
			name2 = (*itt)->getName();
			if (name1.compare(name2) > 0) {
				tmp = *it;
				*it = *itt;
				*itt = tmp;
			}
		}
	}
}

File* Tree::searchTree(vector<File*>& f, string& text)
{
	vector<File*> ::iterator it;
	for (it = f.begin(); it != f.end(); ++it) {
		if ((*it)->getName() == text) return *it;
		/*else {
			vector<File*> c;
			c = (*it)->getAllChild();
			if (!c.empty()) {
				searchTree(c, text);
			}

		}*/
	}
	//If not found:
	return nullptr;
}

File* Tree::searchTreeExe(vector<File*> f)
{
	vector<File*> ::iterator it;
	vector<File*> found;
	for (it = f.begin(); it != f.end(); ++it) {
		if ((*it)->isExe()) {

			if (find(found.begin(), found.end(), *it) == found.end()) {
				return *it;
				found.push_back(*it);
			}

		}
		else {
			vector<File*> c;
			c = (*it)->getAllChild();
			if (!c.empty()) {
				searchTreeExe(c);
			}

		}
	}
}

File::TypeC Tree::convertC(const string& str)
{
	if (str == "LS") return File::TypeC::LS;
	else if (str == "CD") return File::TypeC::CD;
	else if (str == "NEW") return File::TypeC::NEW;
	else if (str == "DEL") return File::TypeC::DEL;
	else if (str == "EXE") return File::TypeC::EXE;
}

string Tree::convertCtoString(File::TypeC c)
{
	if (c == File::TypeC::LS) return "LS";
	else if (c == File::TypeC::CD) return "CD";
	else if (c == File::TypeC::NEW) return "NEW";
	else if (c == File::TypeC::DEL) return "DEL";
	else if (c == File::TypeC::EXE) return "EXE";

}

void Tree::getComannd(string& info, const string& filename)
{
	fstream output(filename, ios::out);
	int i = 0;
	int len = info.size();
	string command;
	File* curr = nullptr;
	string lastcommand;
	try {
		while (i <= len) {

			if (info[i] == '\\' || info[i] == ' ' || i == len) {

				switch (convertC(command)) {


				case File::TypeC::LS: {
					lastcommand = convertCtoString(File::TypeC::LS) + "\n";
					sortTree(root_);
					vector<File*> c = setCurrentDirectory(curr); //Setting current directory (CD)
					output.flush();
					printTree(c, filename, true, curr);
					break;
				}


				case File::TypeC::CD: {
					lastcommand = convertCtoString(File::TypeC::CD) + "\n";
					i++; //Skipping space between CD and name of folder
					string text;
					while (info[i] != '\n') {
						text.push_back(info[i]);
						i++;
					}
					if (text == "..") {
						if (curr == nullptr) throw ECommand(lastcommand.c_str());
						curr = curr->getParent();
					}
					else {
						vector<File*> c = setCurrentDirectory(curr);
						text.insert(0, "\\");
						curr = searchTree(c, text);
						if (curr == nullptr) throw ECommand(lastcommand.c_str());
					}
					break;
				}


				case File::TypeC::NEW: {
					lastcommand = convertCtoString(File::TypeC::NEW) + "\n";
					i++; //Skipping the space
					string text;
					//Reading name of file and content of file
					while (info[i] != '\n' && (info[i] != '\\' || info[i + 1] != 'n')) {

						text.push_back(info[i]);
						i++;
					}
					if (info[i] == '\\' || info[i + 1] == 'n') {
						i++;
					}
					text.insert(0, "\\");
					if (text.find('.') != string::npos) {
						//If new element is file:
						int pos = text.find(' ');
						string info = text.substr(pos);
						string name = text.substr(0, pos);
						File* f = new File(name, info);
						f->setType(f->convert(f->getExtension(name)));
						if (f->getType() != File::Type::txt && info.size() != 0) throw ECommand(lastcommand.c_str());
						addToTree(f, curr);
						text.clear();
					}
					else {
						//If new element is folder:
						File* f = new File(text);
						addToTree(f, curr);
						text.clear();
					}
					break;
				}


				case File::TypeC::DEL: {
					lastcommand = "Error: "+convertCtoString(File::TypeC::DEL) + "\n";
					string text;
					i++;
					while (info[i] != '\n') {
						text.push_back(info[i]);
						i++;
					}
					lastcommand += text;
					text.insert(0, "\\");
					vector<File*> c(setCurrentDirectory(curr));

					File* del = searchTree(c, text);
					if (del == nullptr) throw ECommand(lastcommand.c_str());

					int pos = getPositionOfElem(c, del);
					//Resizing vector of children of parent:
					if (curr == nullptr) {
						if (root_.size() > 1) root_.erase(root_.begin() + pos);
						else root_.clear();

					}
					else {
						if (curr->getAllChild().size() > 1) curr->getAllChild().erase(curr->getAllChild().begin() + pos);
						else curr->emptyChild();

					}
					del->setParent(nullptr);
					del->emptyChild();
					delete(del);
					break;
				}


				case File::TypeC::EXE: {
					lastcommand = convertCtoString(File::TypeC::EXE) + "\n";
					string text;
					i++;
					while (info[i] != '\n') {
						text.push_back(info[i]);
						i++;
					}
					text.insert(0, "\\");
					vector<File*> c = setCurrentDirectory(curr);
					File* exe = searchTree(c, text);
					if (exe == nullptr) throw ECommand(lastcommand.c_str());
					string info = exe->getInfo();
					getComannd(info, filename);
					break;

				}
				default: {
					throw ECommand(lastcommand.c_str());
				}
				}
				command.erase(command.begin(), command.end());
				i++;
			}
			if (i <= len) command.push_back(info[i]);
			i++;
		}
	}
	catch (exception& e) {
		output << e.what();
	}
}

void Tree::printTree(vector<File*> f, const string& filename, bool empty, File* curr)
{

	vector<File*> unprinted;
	string parents;
	fstream output(filename, ios::out | ios::app);
	File* tmp;
	vector<File*> ::iterator it;
	vector<File*> ::iterator itt;
	for (it = f.begin(); it != f.end(); ++it) {
		tmp = *it;
		//Saving names of parents of file that we want to print
		while (tmp->getParent() != curr) {
			tmp = tmp->getParent();
			parents.insert(0, tmp->getName());
		}
		//Checking is something printed:
		output.seekp(0, ios::end);
		int spot = output.tellg();
		if (spot != 0)
		{
			empty = false;
		}
		if (!empty) {
			output << endl;
		}
		output << parents;
		parents.erase();
		//After printing parents print wanted element:
		output << (*it)->getName();
		output.flush();
		empty = false;
		vector<File*> c;
		c = (*it)->getAllChild();
		if (!c.empty()) {
			printTree(c, filename, false, curr);
		}
		/*vector<File*> c;
		if (!(*it)->getAllChild().empty()) {
			c = (*it)->getAllChild();
			for (itt = c.begin(); itt != c.end(); ++itt) {
				unprinted.push_back(*itt);
			}
		}*/
	}
	/*if (!unprinted.empty()) {
		printTree(unprinted, filename, false, curr);
	}*/
}



vector<File*> Tree::setCurrentDirectory(File* curr)
{
	if (curr == nullptr) {
		return root_;
	}
	else {
		vector<File*> c;
		c = curr->getAllChild();
		return c;
	}
}

int Tree::getPositionOfElem(vector<File*> f, File* curr)
{
	vector<File*> ::iterator it;
	int pos = 0;
	for (it = f.begin(); it != f.end(); ++it) {
		if ((*it)->getName() == curr->getName()) return pos;
		pos++;
	}
}



void Tree::sortTree(vector<File*>& f)
{
	vector<File*> ::iterator it;
	sortChild(f);
	for (it = f.begin(); it != f.end(); ++it) {
		vector<File*> c;
		c = (*it)->getAllChild();
		if (!c.empty()) {
			sortChild(c);
			sortTree(c);
		}

	}

}








