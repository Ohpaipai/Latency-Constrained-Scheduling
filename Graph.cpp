#include "Graph.h"
Graph::Graph()
{
	this->Restrict_AND_Resource = 0;
	this->Restrict_OR_Resource=0;
	this->Restrict_NOT_Resource=0;
	this->projectname="";
	this->inputN.clear();
	this->outputN.clear();
	this->Circuit.clear();
	this->MaxtimeInASAP=0;
	this->outputMaxtime=0;
}

Graph::~Graph()
{
	this->Restrict_AND_Resource = 0;
	this->Restrict_OR_Resource=0;
	this->Restrict_NOT_Resource=0;
	this->projectname="";
	this->inputN.clear();
	this->outputN.clear();
	this->Circuit.clear();
	this->MaxtimeInASAP=0;
	this->outputMaxtime=0;
}

void Graph::makeCircuitDiagram(std::string _filename)
{
	
	this->ifile.open(_filename, std::ios::in);
	std::string inputString;//input
	std::stringstream ss;
	while (1) {
		this->ifile >> inputString;
		if (inputString == ".inputs")
		{
			std::getline(this->ifile, inputString);
			ss.clear();
			ss.str(inputString);
			while (ss>>inputString)
			{
				if (inputString[0] == '#') break;
				if (inputString[0] == '\\') {
					inputString.clear();
					std::getline(this->ifile, inputString);
					ss.clear();
					ss.str(inputString);
				}
				else {
					//add new node
					Node newNode;
					newNode.initial(inputString);
					newNode.status = _NULL;
					this->Circuit.insert(std::pair<std::string, Node>(inputString, newNode));
					inputN.push_back(inputString);
				}

			}
		}
		else if (inputString == ".model")
		{
			this->ifile >> inputString;
			this->projectname = inputString;
		}
		else if (inputString == ".outputs")
		{
			std::getline(this->ifile, inputString);
			ss.clear();
			ss.str(inputString);
			while (ss >> inputString)
			{
				if (inputString[0] == '#') break;
				if (inputString[0] == '\\') {
					inputString.clear();
					std::getline(this->ifile, inputString);
					ss.clear();
					ss.str(inputString);
				}
				else {
					//add new node
					Node newNode;
					newNode.initial(inputString);
					newNode.status = _NULL;
					this->Circuit.insert(std::pair<std::string, Node>(inputString, newNode));
					outputN.push_back(inputString);
				}

			}
		}
		else if(inputString==".names")
		{
			std::vector<std::string>Leaf;
			std::getline(this->ifile, inputString);
			ss.clear();
			ss.str(inputString);
			//get Node
			while (ss >> inputString)
			{
				if (inputString[0] == '#') break;
				if (inputString[0] == '\\') {
					inputString.clear();
					std::getline(this->ifile, inputString);
					ss.clear();
					ss.str(inputString);
				}
				else {
					//add new node
					Leaf.push_back(inputString);
				}

			}
			//read
			//new input
			inputString.clear();
			std::getline(this->ifile, inputString);
			ss.clear();
			ss.str(inputString);
			bool isnext = false;
			int runtime = Leaf.size() - 1; 
			std::set<std::string>FunctionAboutNode;
			Operation op = _AND;
			//deal .names function
			while (ss >> inputString)
			{
				//deal .names #
				bool nottodo=false;
				if (inputString[0] == '#') {
					if(isnext==true||FunctionAboutNode.size()==0)
						nottodo=true;
					else
						break;
				}
				else if (inputString[0] == '\\') {
					inputString.clear();
					std::getline(this->ifile, inputString);
					ss.clear();
					ss.str(inputString);
				}
				else {
					for (int i = 0; i < inputString.size(); i++)
					{
						if (inputString[i] == '-') {
							op = _OR;
							isnext = true;
						}
						else if (inputString[i] == '1')
						{
							FunctionAboutNode.insert(Leaf[i]);
						}
						else if (inputString[i] == '0')
						{
							op = _NOT;
							FunctionAboutNode.insert(Leaf[i]);
						}
					}
				}
				if(!nottodo)
					ss >> inputString;

				//is or
				if (isnext)
				{
					//deal .names #
					if(!nottodo)
					{
						runtime -= 1;
						if (runtime <= 0) break;
						inputString.clear();
						std::getline(this->ifile, inputString);
						ss.clear();
						ss.str(inputString);
					}
					else{
						inputString.clear();
						std::getline(this->ifile, inputString);
						ss.clear();
						ss.str(inputString);
					}
				} //have # but is not read time just like ""
				else if(nottodo&&FunctionAboutNode.size()==0){
				
						inputString.clear();
						std::getline(this->ifile, inputString);
						ss.clear();
						ss.str(inputString);
				}
				
			}

			//made node

			Node innode;
			innode.initial(Leaf[Leaf.size() - 1]);
			int t=0;
			for (std::set<std::string>::iterator it=FunctionAboutNode.begin();it!=FunctionAboutNode.end();it++)
			{
				std::map<std::string, Node>::iterator itMap = Circuit.find(*it);
				if (itMap != Circuit.end())
				{
					innode.Consist.insert(itMap->first);
					if (itMap->second.level >=t)
					{
						t = itMap->second.level + 1;
					}
					itMap->second.BeConsist.insert(Leaf[Leaf.size()-1]);
				}
				else
				{
					Node n;
					n.initial(*it);
					this->Circuit[*it] =n;
					innode.Consist.insert(*it);
				}
				
			}
			innode.level = t;
			if(t>this->MaxtimeInASAP)
				this->MaxtimeInASAP =t;
			innode.status = op;
			this->Circuit[Leaf[Leaf.size() - 1]] = innode;
			
			


		}
		else if (inputString == ".end") break;
	
	}
	this->ifile.close();
}

void Graph::Output()
{
	for (std::map<std::string, Node>::iterator it=Circuit.begin();it!=Circuit.end();it++)
	{
		std::cout << it->second.name << " , " << it->second.status << "   level :"<<it->second.level <<std::endl<<" Consist : ";
		for (std::set<std::string>::iterator iter=it->second.Consist.begin(); iter != it->second.Consist.end();iter++)
		{
			std::cout << *iter << " ";
		}
		std::cout << std::endl<<" beConsist : ";
		for (std::set<std::string>::iterator iter = it->second.BeConsist.begin(); iter != it->second.BeConsist.end(); iter++)
		{
			std::cout << *iter << " ";
		}
		std::cout << std::endl<<"Resourse : AND -> ";
		std::cout << std::endl;
	}
}


bool Graph::ALAP(int _time)
{
	std::map<std::string, Node>ALAPCircuit=this->Circuit;
	for (std::map<std::string, Node>::iterator it=ALAPCircuit.begin();it!=ALAPCircuit.end();it++)
	{
		//it->second.slack = it->second.level;
		it->second.oldlevel = it->second.level;
		it->second.level = -1;
		
	}
	bool _rvalue = true;
	for (int i = 0; i < this->outputN.size(); i++)
	{
		std::map<std::string, Node>::iterator it = ALAPCircuit.find(this->outputN[i]);
		it->second.level = _time;
		int time = _time;
		Recurison(ALAPCircuit, it->first, time - 1, _rvalue);
		if (_rvalue == false)
			break;

	}
	if (_rvalue)
	{
		this->Circuit = ALAPCircuit;
		int a = 0; int b = 0; int c = 0;
		for (std::map<std::string, Node>::iterator it=Circuit.begin();it!=Circuit.end();it++)
		{
			it->second.slack = std::abs(it->second.slack - it->second.level);
			switch (it->second.status)
			{
			case _AND:
				a++;
				break;
			case _OR:
				b++;
				break;
			case _NOT:
				c++;
				break;
			default:
				break;
			}
		}
		if(a!=0)
			this->Restrict_AND_Resource = 1; 
		if(b!=0)
			this->Restrict_OR_Resource = 1; 
		if(c!=0)
			this->Restrict_NOT_Resource = 1;
		this->outputMaxtime = _time;
	}
	return _rvalue;
}

void Graph::COut(bool can , char input)
{
	
	
}

void Graph::Recurison(std::map<std::string, Node>& _G, std::string _name, int t, bool& can)
{
	if (can == false) {
		return;
	}
	std::map<std::string, Node>::iterator it = _G.find(_name);
	for (std::set<std::string>::iterator its=it->second.Consist.begin();its!=it->second.Consist.end();its++)
	{
		std::map<std::string, Node>::iterator itT= _G.find(*its);
		if (itT->second.status == _NULL)
		{
			itT->second.level = 0;
		}
		else if (itT->second.level == -1)
		{
			itT->second.level = t;
		}
		else if (itT->second.level > t)
		{
			itT->second.level = t;
		}
		if (t - 1 < 0)
		{
			if (itT->second.status != _NULL)
			{
				can = false;
				return;
			}
		}
		else
		{
			
			Recurison(_G, itT->first, t - 1, can);
		}
	}
}
