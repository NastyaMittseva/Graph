#include "classes.h"
#include <fstream>
using namespace std;

int main()
{
	Graph graph1;
	graph1.readGraph("Test/elerov10.txt");
	vector<int> way = graph1.getEuleranTourFleri();
	ofstream fout("output.txt");
	for (int i = 0; i < way.size(); i++) {
		fout << way[i] << " ";
	}
	fout.close();
	system("pause");
}