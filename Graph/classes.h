#include <iostream>
#include <vector>
#include <set>
#include <tuple>
using namespace std;

class Graph
{
private:
	vector <vector <int> > connectivityMatrix;
	vector <set <int> > connectivityList;
	vector <set <pair <int, int> > > connectivityListWeight;
	set <pair <int, int> > edgeList;
	set <tuple <int, int, int> > edgeListWeight;
	char indicatorRepresentation;
	int Number;
	int indicatorWeight;
	int indicatorOrientation;
	int NumberEdge;
public:
	Graph();
	~Graph();
	void readGraph(string fileName);
	void addEdge(int from, int to, int weight);
	void removeEdge(int from, int to);
	int changeEdge(int from, int to, int newWeight);
	void transformToAdjList();
	void transformToAdjMatrix();
	void transformToListOfEdges();
	void writeGraph(string fileName);
	Graph getSpaingTreePrima();
	Graph getSpaingTreeKruscal();
	Graph getSpaingTreeBoruvka();
};
