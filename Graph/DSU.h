#include <iostream>
#include <vector>
using namespace std;

class DSU{
private:
	int count;
public:
	DSU(int count);
	~DSU();
	void Unite(int x, int y);
	int Find(int x);
	int Size();

	vector <int> p;
	vector <int> rank;
};
