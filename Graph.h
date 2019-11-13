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
	int finallevel;
	int andResourceNum;//*
	int orResourceNum;//+
	int notResourceNum;//~
	int exception;
	void initial(std::string _name) {
		this->andResourceNum = 0;
		this->orResourceNum = 0;
		this->notResourceNum = 0;
		this->ASAPlevel = 0;
		this->ALAPlevel = -1;
		this->status = _NULL;
		this->name = _name;
		this->q = 0;
		this->finallevel = 0;
		this->exception = 0;
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
	void COut();
	void setException();
	void RecurisonForALAP(std::map<std::string, Node>&, std::string, int, bool&);
	void schdeul();
	void schdeulForChildrenNum(bool);
	//void RecurisonForPsforce(std::string _name,int _time);
	void RecurisonForReschdle(std::string _name, int _level);
	void Check(){
		bool rightanswer=true;
		for(std::map<std::string, Node>::iterator it = Circuit.begin(); it != Circuit.end(); it++)
		{
			if(it->second.status != _NULL){
				for(std::set<std::string>::iterator its = it->second.Consist.begin(); its != it->second.Consist.end(); its++)
				{
					std::map<std::string, Node>::iterator itFather=Circuit.find(*its);	
					if(it->second.finallevel <= itFather->second.finallevel){
						rightanswer=false;
						break;	
					}
				}
				for(std::set<std::string>::iterator its = it->second.BeConsist.begin(); its != it->second.BeConsist.end(); its++)
				{
					std::map<std::string, Node>::iterator itFather=Circuit.find(*its);	
					if(it->second.finallevel >= itFather->second.finallevel){
						rightanswer=false;

					}
				}
			}
		}


		if(rightanswer)
			std::cout<<"right\n";
		else
			std::cout<<"wrong\n";
	}	
private:
	std::map<std::string, Node>Circuit; //graph
	int Restrict_AND_Resource;
	std::map<std::string, Node>CircuitBackup; //graph
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
