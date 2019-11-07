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
					if (itMap->second.ASAPlevel >=t)
					{
						t = itMap->second.ASAPlevel + 1;
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
			innode.ASAPlevel = t;
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
		std::cout << it->second.name << " , " << it->second.status << "   ASAPlevel :"<<it->second.ASAPlevel << "   ALAPlevel :"<<it->second.ALAPlevel<<std::endl<<" Consist : ";
		for (std::set<std::string>::iterator iter=it->second.Consist.begin(); iter != it->second.Consist.end();iter++)
		{
			std::cout << *iter << " ";
		}
		std::cout << std::endl<<" beConsist : ";
		for (std::set<std::string>::iterator iter = it->second.BeConsist.begin(); iter != it->second.BeConsist.end(); iter++)
		{
			std::cout << *iter << " ";
		}
		std::cout << std::endl;
	}
	for (int i = 1; i <=this->outputMaxtime; i++)
	{
		std::cout << "level = " << i << "\n and --> " << qand[i] << " ,or --> " << qor[i] << " ,not --> "<< qnot[i] << std::endl;
	}
}


bool Graph::ALAP(int _time)
{
	std::map<std::string, Node>ALAPCircuit=this->Circuit;
	for (std::map<std::string, Node>::iterator it=ALAPCircuit.begin();it!=ALAPCircuit.end();it++)
	{
		//it->second.slack = it->second.ASAPlevel;
		//it->second.ALAPlevel = it->second.ASAPlevel;
		it->second.ALAPlevel = -1;
		
	}
	bool _rvalue = true;
	for (int i = 0; i < this->outputN.size(); i++)
	{
		std::map<std::string, Node>::iterator it = ALAPCircuit.find(this->outputN[i]);
		it->second.ALAPlevel = _time;
		int time = _time;
		RecurisonForALAP(ALAPCircuit, it->first, time - 1, _rvalue);
		if (_rvalue == false)
			break;

	}
	if (_rvalue)
	{
		this->Circuit = ALAPCircuit;
	
		this->outputMaxtime = _time;

		//give Probability size
		this->qand.resize(_time+1);
		this->qor.resize(_time+1);
		this->qnot.resize(_time+1);

		for (std::map<std::string, Node>::iterator it=Circuit.begin();it!=Circuit.end();it++)
		{
			if (it->second.status != _NULL) { //not initial Node
				
			
					double motherNum = (it->second.ALAPlevel) - (it->second.ASAPlevel) + 1.0;
					for (int i = it->second.ASAPlevel; i <=it->second.ALAPlevel; i++)
					{
						switch (it->second.status)
						{
						case _AND:
							this->qand[i] += 1.0/motherNum;
							break;
						case _OR:
							this->qor[i] += 1.0 / motherNum;
							break;
						case _NOT:
							this->qnot[i] += 1.0 / motherNum;
							break;
						default:
							break;
						}
					}
				
			}
		}


	
		
	}
	return _rvalue;
}

void Graph::COut()
{


		std::vector<std::string>outor;
		std::vector<std::string>outnot;
		std::vector<std::string>outand;
		for (int i = 1; i <= this->outputMaxtime; i++)
		{
			outor.clear();
			outand.clear();
			outnot.clear();
			int a = 0; int b = 0; int c = 0;
			for (std::map<std::string, Node>::iterator it = Circuit.begin(); it != Circuit.end(); it++)
			{
				
				if (it->second.finallevel == i)
				{
					switch (it->second.status)
					{
					case _OR:
						a++;
						outor.push_back(it->first);
						break;
					case _AND:
						b++;
						outand.push_back(it->first);
						break;
					case _NOT:
						c++;
						outnot.push_back(it->first);
						break;
					default:
						break;
					}
				}
				
			}
			if (this->Restrict_AND_Resource < b) this->Restrict_AND_Resource = b;
			if (this->Restrict_OR_Resource < a) this->Restrict_OR_Resource = a;
			if (this->Restrict_NOT_Resource < c) this->Restrict_NOT_Resource = c;
			std::cout << i << " :";
			std::cout << " { ";
			for (int k = 0; k < outand.size(); k++)
			{
				if (k + 1 == outand.size())
					std::cout << outand[k];
				else
					std::cout << outand[k] << ",";
			}
			std::cout << " } ";
			std::cout << " { ";
			for (int k = 0; k < outor.size(); k++)
			{
				if (k + 1 == outor.size())
					std::cout << outor[k];
				else
					std::cout << outor[k] << ",";
			}
			std::cout << " } ";
			std::cout << " { ";
			for (int k = 0; k < outnot.size(); k++)
			{
				if (k + 1 == outnot.size())
					std::cout << outnot[k];
				else
					std::cout << outnot[k] << ",";
			}
			std::cout << " } ";
			std::cout << std::endl;
		}

		std::cout << "#AND: " << this->Restrict_AND_Resource << std::endl;
		std::cout << "#OR: " << this->Restrict_OR_Resource << std::endl;
		std::cout << "#NOT: " << this->Restrict_NOT_Resource << std::endl;
		std::cout << "END" << std::endl;
	
}

void Graph::RecurisonForALAP(std::map<std::string, Node>& _G, std::string _name, int t, bool& can)
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
			itT->second.ALAPlevel = -2;
		}
		else if (itT->second.ALAPlevel == -1)
		{
			itT->second.ALAPlevel = t;
		}
		else if (itT->second.ALAPlevel > t)
		{
			itT->second.ALAPlevel = t;
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
			
			RecurisonForALAP(_G, itT->first, t - 1, can);
		}
	}
}

void Graph::schdeul()
{
	for (std::map<std::string, Node>::iterator it = Circuit.begin(); it != Circuit.end(); it++)
	{
		std::pair<int, double> positionAndProbality;
		positionAndProbality.second = 9999999999;
		if ((it->second.ALAPlevel != it->second.ASAPlevel)&&it->second.status!=_NULL&&it->second.finallevel==0) //find best position
		{	
			for (int i = it->second.ASAPlevel; i <= it->second.ALAPlevel; i++)
			{
				//self force
				double num=0;
				double motherNum = (double)(it->second.ALAPlevel) - (double)(it->second.ASAPlevel) + 1.0;
				for (int j = it->second.ASAPlevel; j <= it->second.ALAPlevel; j++)
				{
					//self force
					if (i == j){
						switch (it->second.status)
						{
						case _AND:
							num += qand[j] * (1.0 - (double)1.0 / motherNum);
							break;
						case _OR:
							num += qor[j] * (1.0 - (double)1.0 / motherNum);
							break;
						case _NOT:
							num += qnot[j] * (1.0 - (double)1.0 / motherNum);
							break;
						default:
							break;
						}
					}
					else {
					
						switch (it->second.status)
						{
						case _AND:
							num += qand[j] * (0.0 - (double)1.0 / motherNum);
							break;
						case _OR:
							num += qor[j] * (0.0 - (double)1.0 / motherNum);
							break;
						case _NOT:
							num += qnot[j] * (0.0 - (double)1.0 / motherNum);
							break;
						default:
							break;
						}
					}
				} 
				//ps-force
				for (std::set<std::string>::iterator its = it->second.BeConsist.begin(); its != it->second.BeConsist.end(); its++)
				{
					std::map<std::string, Node>::iterator itps = Circuit.find(*its);
					if (itps->second.ASAPlevel <= i && itps->second.ASAPlevel >= i) { //in ASAP~ALAP reange ,be affect
						double motherNumSub = (double)(itps->second.ALAPlevel) - (double)(itps->second.ASAPlevel) + 1.0;
						//now is 0 possible
						switch (itps->second.status)
						{
						case _AND:
							num += qand[i] * (0.0 - (double)1.0 / motherNumSub);
							break;
						case _OR:
							num += qor[i] * (0.0 - (double)1.0 / motherNumSub);
							break;
						case _NOT:
							num += qnot[i] * (0.0 - (double)1.0 / motherNumSub);
							break;
						default:
							break;
						}
						//next is 1 possible
						for (int j = i+1; j <=itps->second.ALAPlevel; j++)
						{
							double keep;
							switch (itps->second.status)
							{
							case _AND:
								keep= qand[j] * (1.0 - (double)1.0 / motherNumSub);
								num += keep;
								break;
							case _OR:
								keep = qor[j] * (1.0 - (double)1.0 / motherNumSub);
								num += keep;
								break;
							case _NOT:
								keep = qnot[j] * (1.0 - (double)1.0 / motherNumSub);
								num += keep;
								break;
							default:
								break;
							}

							if (positionAndProbality.second > num) {
								positionAndProbality.first = i;
								positionAndProbality.second = num;
							}
							num -= keep;
						}
						
					}
				}
				//total force is num
				//find smallest
				
				
			}

			it->second.finallevel = positionAndProbality.first;
			it->second.ASAPlevel = positionAndProbality.first;
			it->second.ALAPlevel = positionAndProbality.first;
			//schedule
			for (std::set<std::string>::iterator its = it->second.BeConsist.begin(); its != it->second.BeConsist.end(); its++)
			{
				RecurisonForReschdle(*its, positionAndProbality.first);
			}
		}
		else {
			it->second.finallevel = it->second.ASAPlevel;
		}
		//reset
		for (int i = 0; i < this->outputMaxtime; i++)
		{
			this->qand[i] = 0;
			this->qnot[i] = 0;
			this->qor[i] = 0;
		}
		//update time-frames
		for (std::map<std::string, Node>::iterator it = Circuit.begin(); it != Circuit.end(); it++)
		{
			if (it->second.status != _NULL) { //not initial Node


				double motherNum = (it->second.ALAPlevel) - (it->second.ASAPlevel) + 1.0;
				for (int i = it->second.ASAPlevel; i <= it->second.ALAPlevel; i++)
				{
					switch (it->second.status)
					{
					case _AND:
						this->qand[i] += 1.0 / motherNum;
						break;
					case _OR:
						this->qor[i] += 1.0 / motherNum;
						break;
					case _NOT:
						this->qnot[i] += 1.0 / motherNum;
						break;
					default:
						break;
					}
				}

			}
		}
	}
}

void Graph::RecurisonForReschdle(std::string _name, int _level)
{
	std::map<std::string, Node>::iterator it = Circuit.find(_name);
	if (it->second.ASAPlevel <= _level)
	{
		it->second.ASAPlevel = _level + 1;
		if (it->second.ASAPlevel >= it->second.ALAPlevel)
		{
			it->second.ASAPlevel = it->second.ALAPlevel;
		}
		for (std::set<std::string>::iterator its = it->second.BeConsist.begin(); its != it->second.BeConsist.end(); its++)
		{
			RecurisonForReschdle(*its, it->second.ASAPlevel);
		}

	}
	else {
		return;
	}
}
	





