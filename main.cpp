#include"Graph.h"
int main() {
	Graph G;
	std::string name = "test.blif";
	G.makeCircuitDiagram(name);
	
//	G.Output();

	//std::cout << "--------------------------------------------------------------------------\n";
	if (G.ALAP(4)) {
		G.schdeul();
		G.COut();
	}
	else
	{
		std::cout << "Not answer\n";
	}
//	G.Output();
	


	system("pause");
	return 0;

}