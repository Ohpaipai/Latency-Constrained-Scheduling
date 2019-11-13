#include"Graph.h"
int main(int argc,char *argv[]) {
	Graph G;
	//std::string name = "test.blif";
	//G.makeCircuitDiagram(name);
	
	//G.Output();
//	std::cout<<argc<<"\n";
if(argc==3){
	//std::string s=argv[1];
		std::string name=argv[1];
		std::stringstream ss;
		ss<<argv[2];
		int timeristrict;
		ss>>timeristrict;
		G.makeCircuitDiagram(name);
//		std::cout<<"hello\n";
		if(G.ALAP(timeristrict)){
			std::cout << "Latency-constrained Scheduling\n";
		//G.schdeul();
        G.schdeulForChildrenNum(0);
		G.COut();
		//G.Check();
		}else
			std::cout<<"No feasible Scheduling\nEND"<<std::endl;

}
/*else if(argc==5)
{
	std::string s1=argv[1];
 	if(s1=="exception")
        {
		 std::stringstream sss;
		 sss<<argv[2];
		 int up;
		 sss>>up;
                std::string name=argv[3];
                std::stringstream ss;
                ss<<argv[4];
                int timeristrict;
                ss>>timeristrict;
                G.makeCircuitDiagram(name);
                if(G.ALAP(timeristrict)){
                G.schdeulForChildrenNum(up);
                G.COut();
				G.Check();
                }
                else
                        std::cout<<"No feasible Scheduling"<<std::endl;
        }
        else{
                std::cout<<"wrong input?"<<std::endl;
        }

}
else{
	 std::cout<<"wrong input@"<<std::endl;
}
*/
//	std::cout << "--------------------------------------------------------------------------\n";
/*	if (G.ALAP(50)) {
		//G.setException();
		G.schdeul();
		G.COut();
	}
	else
	{
		std::cout << "Not answer\n";
	}*/
	//G.Output();
	


	//system("pause");
	return 0;

}
