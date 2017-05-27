#include "classes.h"
using namespace std;

int main()
{
	Graph graph1;
	graph1.readGraph("Test/input-3.txt");
	graph1.getSpaingTreePrima();
	graph1.transformToListOfEdges();
	graph1.writeGraph("output.txt");
	//system("pause");
}