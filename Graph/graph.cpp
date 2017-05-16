#include "classes.h"
using namespace std;

int main()
{
	Graph graph1;
	graph1.readGraph("graph3.txt");
	graph1.getSpaingTreeBoruvka();
	graph1.writeGraph("output.txt");
	//system("pause");

}