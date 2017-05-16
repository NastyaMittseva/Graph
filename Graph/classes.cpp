#include "classes.h"
#include "DSU.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <tuple>
#include <map>

using namespace std;

Graph::Graph() {
	Number = 0;
}

Graph::~Graph() {
	
	if ((indicatorRepresentation = 'C') && (connectivityMatrix.size() != 0)) {
		for (int i = 0; i < Number; i++) {
			connectivityMatrix[i].clear();
		}
		connectivityMatrix.~vector();
	}
	if ((indicatorRepresentation = 'L') && (indicatorWeight == 0) && (connectivityList.size() != 0)) {
		for (int i = 0; i < Number + 1; i++) {
			connectivityList[i].clear();
		}
		connectivityList.~vector();
	}
	if ((indicatorRepresentation = 'L') && (indicatorWeight != 0) && (connectivityListWeight.size() != 0)) {
		for (int i = 0; i < Number + 1; i++) {
			connectivityListWeight[i].clear();
		}
		connectivityListWeight.~vector();
		
	}
	if ((indicatorRepresentation = 'E') && (indicatorWeight == 0) && (edgeList.size() != 0)) {
		edgeList.clear();
	}
	if ((indicatorRepresentation = 'E') && (indicatorWeight != 0) && (edgeListWeight.size() != 0)) {
		edgeListWeight.clear();
	}
}

void Graph::readGraph(string fileName) {
	ifstream fin(fileName);
	string str;
	while (!fin.eof()) {
		fin >> indicatorRepresentation;
		switch (indicatorRepresentation) {
		case 'C':									    //������� ���������
			fin >> Number >> indicatorWeight;
			connectivityMatrix.resize(Number);
			for (int i = 0; i<Number; i++) {
				connectivityMatrix[i].resize(Number);
			}
			for (int i = 0; i<Number; i++) {
				for (int j = 0; j<Number; j++) {
					fin >> connectivityMatrix[i][j];
				}
			}
			indicatorOrientation = 0;
			for (int i = 0; i<Number; i++) {
				for (int j = 0; j<Number; j++) {
					if ((connectivityMatrix[i][j] != connectivityMatrix[j][i]) || (connectivityMatrix[i][i] != 0)) {
						indicatorOrientation = 1;
						break;
					}
				}
			}
			break;
		case 'L':
			fin >> Number >> indicatorOrientation >> indicatorWeight;
			int bi, wi;
			if (indicatorWeight == 0) {					 //������ ��������� ��� ������������� �����
				connectivityList.resize(Number + 1);
				for (int i = 0; i <= Number; i++) {
					getline(fin, str);
					istringstream ist(str);
					while (ist >> bi) {
						connectivityList[i].insert(bi);
					}
				}
			}
			else {										 //������ ��������� ��� ����������� �����
				connectivityListWeight.resize(Number + 1);
				for (int i = 0; i <= Number; i++) {
					getline(fin, str);
					istringstream ist(str);
					while (ist >> bi >> wi) {
						connectivityListWeight[i].insert(make_pair(bi, wi));
					}
				}
			}
			break;
		case 'E':
			fin >> Number >> NumberEdge >> indicatorOrientation >> indicatorWeight;
			int ai;
			if (indicatorWeight == 0) {                  //������ ����� ��� ������������� �����
				for (int i = 0; i<NumberEdge; i++) {
					fin >> ai >> bi;
					edgeList.insert(make_pair(ai, bi));
				}
			}
			else {										//������ ����� ��� ����������� �����
				for (int i = 0; i<NumberEdge; i++) {
					fin >> ai >> bi >> wi;
					edgeListWeight.insert(make_tuple(ai, bi, wi));
				}
			}
			break;
		}
	}
}

void Graph::addEdge(int from, int to, int weight) {
	switch (indicatorRepresentation) {
	case 'C':                     //�������� ����� � ������� ���������
		if (indicatorWeight == 0) {
			connectivityMatrix[from - 1][to - 1] = 1;
			NumberEdge++;
		}
		else {
			connectivityMatrix[from - 1][to - 1] = weight;
			NumberEdge++;
		}
		if (indicatorOrientation == 0) {
			connectivityMatrix[to - 1][from - 1] = weight;
			NumberEdge++;
		}
		break;
	case 'L':
		if (indicatorOrientation == 0) {  //�������� ����� � ������ ��������� ������������������ �����
			if (indicatorWeight == 0) {
				connectivityList[from].insert(to);
				connectivityList[to].insert(from);
				NumberEdge += 2;
			}
			else {
				connectivityListWeight[from].insert(make_pair(to, weight));
				connectivityListWeight[to].insert(make_pair(from, weight));
				NumberEdge += 2;
			}
		}
		else {							//�������� ����� � ������ ��������� ���������������� �����
			if (indicatorWeight == 0) {
				connectivityList[from].insert(to);
				NumberEdge++;
			}
			else {
				connectivityListWeight[from].insert(make_pair(to, weight));
				NumberEdge++;
			}
		}
		break;
	case 'E':
		if (indicatorOrientation == 0) {   //�������� ����� � ������ ����� ������������������ �����
			if (indicatorWeight == 0) {
				edgeList.insert(make_pair(from, to));
				edgeList.insert(make_pair(to, from));
				NumberEdge += 2;
			}
			else {
				edgeListWeight.insert(make_tuple(from, to, weight));
				edgeListWeight.insert(make_tuple(to, from, weight));
				NumberEdge += 2;
			}
		}
		else {								//�������� ����� � ������ ����� ���������������� �����
			if (indicatorWeight == 0) {
				edgeList.insert(make_pair(from, to));
				NumberEdge++;
			}
			else {
				edgeListWeight.insert(make_tuple(from, to, weight));
				NumberEdge++;
			}
		}
		break;
	}
}

void Graph::removeEdge(int from, int to) {
	switch (indicatorRepresentation) {
	case 'C':							//������� ����� � ������� ���������
		connectivityMatrix[from - 1][to - 1] = 0;
		NumberEdge--;
		if (indicatorOrientation == 0) {
			connectivityMatrix[to - 1][from - 1] = 0;
			NumberEdge--;
		}
		break;
	case 'L':
		if (indicatorOrientation == 0) {   //������� ����� � ������ ��������� ������������������ �����
			if (indicatorWeight == 0) {
				auto search = connectivityList[from].upper_bound(to);
				connectivityList[from].erase(search);
				search = connectivityList[to].upper_bound(from);
				connectivityList[to].erase(search);
				NumberEdge -= 2;
			}
			else {
				auto search = connectivityListWeight[from].upper_bound(make_pair(to, 0));
				connectivityListWeight[from].erase(search);
				search = connectivityListWeight[to].upper_bound(make_pair(from, 0));
				connectivityListWeight[to].erase(search);
				NumberEdge -= 2;
			}
		}
		else {							//������� ����� � ������ ��������� ���������������� �����
			if (indicatorWeight == 0) {
				auto search = connectivityList[from].upper_bound(to);
				connectivityList[from].erase(search);
				NumberEdge--;
			}
			else {
				auto search = connectivityListWeight[from].upper_bound(make_pair(to, 0));
				connectivityListWeight[from].erase(search);
				NumberEdge--;
			}
		}
		break;
	case 'E':
		if (indicatorOrientation == 0) {    //������� ����� � ������ ����� ������������������ �����
			if (indicatorWeight == 0) {
				auto search = edgeList.upper_bound(make_pair(from, to));
				edgeList.erase(search);
				search = edgeList.upper_bound(make_pair(to, from));
				edgeList.erase(search);
				NumberEdge -= 2;
			}
			else {
				auto search = edgeListWeight.upper_bound(make_tuple(from, to, 0));
				edgeListWeight.erase(search);
				search = edgeListWeight.upper_bound(make_tuple(to, from, 0));
				edgeListWeight.erase(search);
				NumberEdge -= 2;
			}
		}
		else {                              //������� ����� � ������ ����� ���������������� �����
			if (indicatorWeight == 0) {
				auto search = edgeList.upper_bound(make_pair(from, to));
				edgeList.erase(search);
				NumberEdge--;
			}
			else {
				auto search = edgeListWeight.upper_bound(make_tuple(from, to, 0));
				edgeListWeight.erase(search);
				NumberEdge--;
			}
		}
		break;

	}
}

int Graph::changeEdge(int from, int to, int newWeight) {
	switch (indicatorRepresentation) {
	case 'C':										//�������� ��� ����� � ������� ���������
		if (indicatorWeight == 1) {
			int oldWeight = connectivityMatrix[from - 1][to - 1];
			connectivityMatrix[from - 1][to - 1] = newWeight;
			if (indicatorOrientation == 0) connectivityMatrix[to - 1][from - 1] = newWeight;
			return oldWeight;
		}
		break;
	case 'L':										//�������� ��� ����� � ������ ���������
		if (indicatorWeight == 1) {
			auto search = connectivityListWeight[from].upper_bound(make_pair(to, 0));
			int oldWeight = search->second;
			connectivityListWeight[from].insert(search, make_pair(to, newWeight));
			connectivityListWeight[from].erase(search);
			if (indicatorOrientation == 0) {
				auto search = connectivityListWeight[to].upper_bound(make_pair(from, 0));
				connectivityListWeight[to].insert(search, make_pair(from, newWeight));
				connectivityListWeight[to].erase(search);
			}
			return oldWeight;
		}
		break;
	case 'E':										//�������� ��� ����� � ������ �����
		if (indicatorWeight == 1) {
			auto search = edgeListWeight.upper_bound(make_tuple(from, to, 0));
			int oldWeight = get<2>(*search);
			edgeListWeight.insert(search, make_tuple(from, to, newWeight));
			edgeListWeight.erase(search);
			if (indicatorOrientation == 0) {
				auto search = edgeListWeight.upper_bound(make_tuple(to, from, 0));
				edgeListWeight.insert(search, make_tuple(to, from, newWeight));
				edgeListWeight.erase(search);
			}
			return oldWeight;
		}
		break;
	}

}

void Graph::transformToAdjMatrix() {       //������������� � ������� ���������
	switch (indicatorRepresentation) {
	case 'C':
		break;
	case 'L':
		connectivityMatrix.clear();
		connectivityMatrix.resize(Number);
		for (int i = 0; i<Number; i++) {
			connectivityMatrix[i].resize(Number, 0);
		}
		indicatorRepresentation = 'C';
		if (indicatorWeight == 0) {       //������� ������ ��������� ������������� ����� � ������� ���������
			for (int ai = 0; ai <= Number; ai++) {
				for (set <int >::iterator bi = connectivityList[ai].begin(); bi != connectivityList[ai].end(); ++bi) {
					connectivityMatrix[ai - 1][*bi - 1] = 1;
				}
			}
		}
		else {                           //������� ������ ��������� ����������� ����� � ������� ���������
			for (int ai = 0; ai <= Number; ai++) {
				for (set <pair <int, int> >::iterator bi = connectivityListWeight[ai].begin(); bi != connectivityListWeight[ai].end(); ++bi) {
					connectivityMatrix[ai - 1][bi->first - 1] = bi->second;
				}
			}
		}
		break;
	case 'E':
		connectivityMatrix.clear();
		connectivityMatrix.resize(Number);
		for (int i = 0; i<Number; i++) {
			connectivityMatrix[i].resize(Number, 0);
		}
		indicatorRepresentation = 'C';
		if (indicatorWeight == 0) {      //������� ������ ����� ������������� ����� � ������� ���������
			for (set <pair <int, int> >::iterator bi = edgeList.begin(); bi != edgeList.end(); ++bi) {
				connectivityMatrix[bi->first - 1][bi->second - 1] = 1;
			}
		}
		else {                          //������� ������ ����� ����������� ����� � ������� ���������
			for (set <tuple <int, int, int> >::iterator bi = edgeListWeight.begin(); bi != edgeListWeight.end(); ++bi) {
				connectivityMatrix[get<0>(*bi) - 1][get<1>(*bi) - 1] = get<2>(*bi);
			}
		}
		break;
	}
}

void Graph::transformToAdjList() {          //������������� � ������ ���������
	switch (indicatorRepresentation) {
	case 'C':
		indicatorRepresentation = 'L';
		if (indicatorWeight == 0) {      //������� ������� ��������� ��� ������������� ����� � ������ ���������
			connectivityList.resize(Number + 1);
			for (int i = 0; i<Number; i++) {
				for (int j = 0; j<Number; j++) {
					if (connectivityMatrix[i][j] != 0) connectivityList[i + 1].insert(j + 1);
				}
			}
		}
		else {                           //������� ������� ��������� ��� ����������� ����� � ������ ���������
			connectivityListWeight.resize(Number + 1);
			for (int i = 0; i<Number; i++) {
				for (int j = 0; j<Number; j++) {
					if (connectivityMatrix[i][j] != 0) connectivityListWeight[i + 1].insert(make_pair(j + 1, connectivityMatrix[i][j]));
				}
			}
		}
		break;
	case 'L':
		break;
	case 'E':
		indicatorRepresentation = 'L';
		if (indicatorWeight == 0) {      //������� ������ ����� ��� ������������� ����� � ������ ���������
			connectivityList.resize(Number + 1);
			for (set <pair <int, int> >::iterator bi = edgeList.begin(); bi != edgeList.end(); ++bi) {
				connectivityList[bi->first].insert(bi->second);
			}
		}
		else {                          //������� ������ ����� ��� ����������� ����� � ������ ���������
			connectivityListWeight.resize(Number + 1);
			for (set <tuple <int, int, int> >::iterator bi = edgeListWeight.begin(); bi != edgeListWeight.end(); ++bi) {
				connectivityListWeight[get<0>(*bi)].insert(make_pair(get<1>(*bi), get<2>(*bi)));
			}
		}
		break;
	}
}

void Graph::transformToListOfEdges() {        //������������� � ������ �����
	switch (indicatorRepresentation) {
	case 'C':
		indicatorRepresentation = 'E', NumberEdge = 0;
		if (indicatorWeight == 0) {       //������� ������� ��������� ������������� ����� � ������ �����
			edgeList.clear();
			for (int i = 0; i<Number; i++) {
				for (int j = 0; j<Number; j++) {
					if (connectivityMatrix[i][j] == 1) {
						edgeList.insert(make_pair(i + 1, j + 1));
						NumberEdge++;
					}
				}
			}
		}
		else {							 //������� ������� ��������� ����������� ����� � ������ �����
			edgeListWeight.clear();
			for (int i = 0; i<Number; i++) {
				for (int j = 0; j<Number; j++) {
					if (connectivityMatrix[i][j] != 0) {
						edgeListWeight.insert(make_tuple(i + 1, j + 1, connectivityMatrix[i][j]));
						NumberEdge++;
					}
				}
			}

		}
		break;
	case 'L':
		indicatorRepresentation = 'E', NumberEdge = 0;
		if (indicatorWeight == 0) {           //������� ������ ��������� ������������� ����� � ������ �����        
			edgeList.clear();
			for (int ai = 0; ai <= Number; ai++) {
				for (set <int >::iterator bi = connectivityList[ai].begin(); bi != connectivityList[ai].end(); ++bi) {
					edgeList.insert(make_pair(ai, *bi));
					NumberEdge++;
				}
			}
		}
		else {                               //������� ������ ��������� ����������� ����� � ������ ����� 
			edgeListWeight.clear();
			for (int ai = 0; ai <= Number; ai++) {
				for (set <pair <int, int> >::iterator bi = connectivityListWeight[ai].begin(); bi != connectivityListWeight[ai].end(); ++bi) {
					edgeListWeight.insert(make_tuple(ai, bi->first, bi->second));
					NumberEdge++;
				}
			}
		}
		break;
	case 'E':
		break;
	}
}

void Graph::writeGraph(string fileName) {
	ofstream fout(fileName);
	switch (indicatorRepresentation) {
	case 'C':														//����� ������� ���������
		fout << indicatorRepresentation << ' ' << Number << ' ' << indicatorWeight << endl;
		for (int i = 0; i<Number; i++) {
			for (int j = 0; j<Number; j++) {
				fout << connectivityMatrix[i][j] << ' ';
			}
			fout << endl;
		}
		fout.close();
		break;

	case 'L':
		fout << indicatorRepresentation << ' ' << Number << endl
			<< indicatorOrientation << ' ' << indicatorWeight << endl;
		if (indicatorWeight == 0) {                            //����� ������ ��������� ��� ������������� �����
			for (vector <set <int> >::iterator i = connectivityList.begin() + 1; i != connectivityList.end(); ++i) {
				for (set <int >::const_iterator j = i->begin(); j != i->end(); ++j) {
					fout << *j << ' ';
				}
				fout << endl;
			}
		}
		else {														//����� ������ ��������� ��� ����������� �����
			for (vector <set <pair <int, int> > >::iterator i = connectivityListWeight.begin() + 1; i != connectivityListWeight.end(); ++i) {
				for (set <pair <int, int> >::const_iterator j = i->begin(); j != i->end(); ++j) {
					fout << j->first << ' ' << j->second << ' ';
				}
				fout << endl;
			}
		}
		fout.close();
		break;

	case 'E':
		fout << indicatorRepresentation << ' ' << Number << ' ' << NumberEdge << endl
			<< indicatorOrientation << ' ' << indicatorWeight << endl;
		if (indicatorWeight == 0) {                                  //����� ������ ����� ��� ������������� �����
			for (set <pair <int, int> >::iterator i = edgeList.begin(); i != edgeList.end(); ++i) {
				fout << i->first << ' ' << i->second << endl;
			}
		}
		else {														//����� ������ ����� ��� ����������� �����
			for (set <tuple <int, int, int> >::iterator i = edgeListWeight.begin(); i != edgeListWeight.end(); ++i) {
				fout << get<0>(*i) << ' ' << get<1>(*i) << ' ' << get<2>(*i) << endl;
			}
		}
		fout.close();
		break;
	}
}

Graph Graph::getSpaingTreePrima() {
	if ((indicatorWeight != 0) || (indicatorOrientation == 0)) {
		if ((indicatorRepresentation == 'C') || (indicatorRepresentation == 'E')) transformToAdjList();
		
		vector <set < pair<int, int>>>  initialList;
		initialList.resize(Number + 1);
		initialList = connectivityListWeight;
		connectivityListWeight.clear();
		connectivityListWeight.resize(Number + 1);
		const int INF = 1000000000; // �������� "�������������"
		vector<int> min_e(Number+1, INF), sel_e(Number+1, -1);
		min_e[1] = 0;
		vector<bool> used(Number + 1, false);
		set < pair<int, int> > q;
		
		q.insert(make_pair(0, 0));
		for (int i = 1; i <= Number; i++) {
			if (q.empty()) {
				exit(0);
			}
			int v1 = q.begin()->first;
			int v = q.begin()->second;
			q.erase(q.begin());
			used[v] = true;
			//����� ����� � ���������� ����������
			if (sel_e[v] != -1) {
				connectivityListWeight[sel_e[v]].insert(make_pair(v, v1));
				connectivityListWeight[v].insert(make_pair(sel_e[v], v1));
			}
			if (v == 0) v++;
			//����� ������, ������� �������� ����� � ���������� ���������� � ���������� ��������
			for (set <pair <int, int> >::iterator j = initialList[v].begin(); j != initialList[v].end(); ++j) {
				int to = j->first,
					cost = j->second;
				if (cost < min_e[to] && !used[to]) {
					q.erase(make_pair(min_e[to], to));
					min_e[to] = cost;
					sel_e[to] = v;
					q.insert(make_pair(min_e[to], to));
				}
			}
		}
		initialList.clear();
		return *this;
	}
}

Graph Graph::getSpaingTreeKruscal() {
	if ((indicatorWeight != 0) || (indicatorOrientation == 0)) {
		if ((indicatorRepresentation == 'C') || (indicatorRepresentation == 'L')) transformToListOfEdges();
		
		int cost = 0;
		set <tuple <int, int, int> > initialList;
		set <tuple <int, int, int>> sortedInitialList;
		vector<int> p;
		p.resize(Number);
		initialList = edgeListWeight;
		edgeListWeight.clear();
		
		//���������� ����� �� �����
		for (set <tuple <int, int, int> >::iterator i = initialList.begin(); i != initialList.end(); ++i) {
			sortedInitialList.insert(make_tuple(get<2>(*i), get<0>(*i), get<1>(*i)));
		}
		//�������� ������ ������� � ���� ������
		DSU components(Number);
		NumberEdge = 0;
		//������� �����, � ��� ������� ����� ������������, ����������� �� ��� ����� ������ ��������
		for (set <tuple <int, int, int> >::iterator i = sortedInitialList.begin(); i != sortedInitialList.end(); ++i) {
			int a = get<1>(*i), b = get<2>(*i), l = get<0>(*i);
			if (components.Find(a) != components.Find(b)) {
				cost += l;
				edgeListWeight.insert(make_tuple(a, b, l));
				//����������� ���� ��������
				components.Unite(a, b);
			}
		}
		NumberEdge = edgeListWeight.size();
		return *this;
	}
}

Graph Graph::getSpaingTreeBoruvka() {
	if ((indicatorWeight != 0) || (indicatorOrientation == 0)) {
		if ((indicatorRepresentation == 'C') || (indicatorRepresentation == 'L')) transformToListOfEdges();

		int cost;
		const int INF = 1000000000;
		map < int, tuple<int, int, int> > edgesWithComponents;
		set < tuple<int, int, int> > initialList;
		initialList = edgeListWeight;
		edgeListWeight.clear();
		NumberEdge = 0;
		DSU components(Number);

		//������������ �����, ���� �� ������ ��� �����
		while (edgeListWeight.size() < Number - 1) {
			//����� ����������� ����� ����� ������������ ���������
			for (set <tuple <int, int, int> >::iterator i = initialList.begin(); i != initialList.end(); ++i) {
				int a = get<0>(*i), b = get<1>(*i), l = get<2>(*i);
				if (components.Find(a) != components.Find(b)) {
					if (edgesWithComponents.find(components.Find(a)) != edgesWithComponents.end()) cost = get<2>(edgesWithComponents[components.Find(a)]);
					else cost = INF;
					if (l < cost) edgesWithComponents[components.Find(a)] = *i;

					if (edgesWithComponents.find(components.Find(b)) != edgesWithComponents.end()) cost = get<2>(edgesWithComponents[components.Find(b)]);
					else cost = INF;
					if (l < cost) edgesWithComponents[components.Find(b)] = *i;
				}
			}
			//��������� ��������� �����
			for (map <int, tuple <int, int, int> >::iterator i = edgesWithComponents.begin(); i != edgesWithComponents.end(); ++i) {
				int a = get<0>(i->second), b = get<1>(i->second), l = get<2>(i->second);
				edgeListWeight.insert(i->second);
				components.Unite(a, b);
			}
			edgesWithComponents.clear();
		}
		NumberEdge = edgeListWeight.size();
		return *this;
	}
}