#include "DSU.h"
#include <iostream>
using namespace std;

DSU::DSU(int count) {
//��������� ������ ������� � ���� ������
	this->count = count;
	p.resize(count + 1);
	rank.resize(count + 1);
	for (int i = 1; i <= count; i++) {
		p[i] = i;
		rank[i] = 0;
	}
}

DSU::~DSU() {
	p.clear();
	rank.clear();
}

void DSU::Unite(int x, int y) {
// ����������� ���� �������� �������� ���������� ����������
	if ((x = Find(x)) == (y = Find(y)))
		return;

	if (rank[x] < rank[y]) {
		p[x] = y;
		++rank[y];
	}
	else {
		p[y] = x;
		++rank[x];
	}
}

int DSU::Find(int x) {
//���������� � ����� ��������� ��������� ��������� �������
	return (x == p[x] ? x : p[x] = Find(p[x]));
}

int DSU::Size() {
//���������� ���������� ��������
	return (count);
}