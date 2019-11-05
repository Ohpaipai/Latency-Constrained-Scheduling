#pragma once
#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<vector>
#include<set>
#include<sstream>
#include<algorithm>
#include<queue>
#define Operationtime 1
//Opreation define
enum Operation {
	_NULL,
	_AND,
	_OR,
	_NOT,
	_OVER,
};


typedef struct Myclass {
	std::set<std::string>Consist; //parent
	std::set<std::string>BeConsist;//children
	std::string name;//name
	Operation status;//status(like add ,or ,not),node
	int level;
	int slack;
	int oldlevel;
	void initial(std::string _name) {
		this->level = 0;
		this->slack = 0;
		this->oldlevel = 0;
		this->status = _NULL;
		this->name = _name;
	}
	
}Node;

class Graph
{
public:
	Graph();
	~Graph();
	void makeCircuitDiagram(std::string);
	void Output();
	bool ALAP(int _time);
	void COut(bool,char);
	void Recurison(std::map<std::string, Node>&, std::string, int, bool&);
private:
	std::map<std::string, Node>Circuit;
	int Restrict_AND_Resource;
	int Restrict_OR_Resource;
	int Restrict_NOT_Resource;
	int MaxtimeInASAP=0;
	int outputMaxtime=0;
	std::ifstream ifile;
	std::string projectname;
	std::vector<std::string>inputN;
	std::vector<std::string>outputN;
};
