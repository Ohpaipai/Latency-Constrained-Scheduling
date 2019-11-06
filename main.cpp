#include"Graph.h"
int main() {
	Graph G;
	std::string name = "test.blif";
	G.makeCircuitDiagram(name);
	G.ALAP(5);
	G.Output();
	


	system("pause");
	return 0;

}