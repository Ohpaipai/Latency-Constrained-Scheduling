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
	int ASAPlevel; //ASAP
	int ALAPlevel; //ALAP
	int q;
	void initial(std::string _name) {
		this->ASAPlevel = 0;
		this->ALAPlevel = -1;
		this->status = _NULL;
		this->name = _name;
		this->q = 0;
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
	void schdeul();
private:
	std::map<std::string, Node>Circuit; //graph
	int Restrict_AND_Resource;
	int Restrict_OR_Resource;
	int Restrict_NOT_Resource;
	int MaxtimeInASAP=0; //ASAP max
	int outputMaxtime=0; //ALAP max
	std::ifstream ifile;
	std::string projectname;
	std::vector<std::string>inputN;
	std::vector<std::string>outputN;
	std::vector<double>qand; //and Probability 
	std::vector<double>qor; //or Probability
	std::vector<double>qnot; //not Probability
};
