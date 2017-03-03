#include "classes.h"
using namespace std;

int main()
{
	Graph graph1;
	graph1.readGraph("graph1.txt");
	graph1.writeGraph("output1.txt");
	graph1.transformToAdjList();
	graph1.writeGraph("output2.txt");
	graph1.addEdge(2,3,7);
	graph1.addEdge(1,4,2);
	graph1.addEdge(3,5,3);
	graph1.changeEdge(1,4,5);
	graph1.writeGraph("output3.txt");
	graph1.removeEdge(2,5);
	graph1.removeEdge(4,5);
	graph1.writeGraph("output4.txt");
	graph1.transformToAdjMatrix();
	graph1.writeGraph("output5.txt");
	system ("pause");
}