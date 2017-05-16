#include "DSU.h"
#include <iostream>
using namespace std;

DSU::DSU(int count) {
//помещение каждой вершины в свое дерево
	this->count = count;
	p = new int[count];
	rank = new int[count];
	for (int i = 1; i <= count; i++) {
		p[i] = i;
		rank[i] = 0;
	}
}

DSU::~DSU() {
	if (p != nullptr) {
		p = nullptr;
		delete[] p;
	}
	if (rank != nullptr)
		rank = nullptr;
		delete[] rank;
}

void DSU::Unite(int x, int y) {
// объединение двух множеств заданных некоторыми элементами
	if ((x = Find(x)) == (y = Find(y)))
		return;

	if (rank[x] <  rank[y])
		p[x] = y;
	else
		p[y] = x;

	if (rank[x] == rank[y])
		++rank[x];
}

int DSU::Find(int x) {
//возвращает в каком множестве находится указанный элемент
	return (x == p[x] ? x : p[x] = Find(p[x]));
}

int DSU::Size() {
//возвращает количество множеств
	return (count);
}