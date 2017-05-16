#include <iostream>
using namespace std;

class DSU{
private:
	int count;
	int * p = nullptr;
	int * rank = nullptr;
public:
	DSU(int count);
	~DSU();
	void Unite(int x, int y);
	int Find(int x);
	int Size();
};
