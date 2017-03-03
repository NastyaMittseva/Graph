#include "classes.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <tuple>

using namespace std;

void Graph::readGraph(string fileName){
	ifstream fin(fileName);	
	string str;
    while (!fin.eof()){
		fin >> indicatorRepresentation;
		switch (indicatorRepresentation){
			case 'C':									    //������� ���������
				fin >> Number >> indicatorWeight;
				connectivityMatrix.resize(Number);
				for (int i=0; i<Number; i++){
					connectivityMatrix[i].resize(Number); 
				}
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						fin >>connectivityMatrix[i][j];
					}
				}
				indicatorOrientation = 0;
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						if ((connectivityMatrix[i][j] != connectivityMatrix[j][i]) || (connectivityMatrix[i][i] != 0)){
							indicatorOrientation = 1;
							break;
						}
					}
				}
				break;
			case 'L':                                      
				fin >> Number >> indicatorOrientation >> indicatorWeight;
				int bi, wi;
				if (indicatorWeight == 0){					 //������ ��������� ��� ������������� �����
					connectivityList.resize(Number+1);
					for (int i=0; i<=Number; i++){
						getline(fin, str);
						istringstream ist (str);
						while (ist >> bi){                       
							connectivityList[i].insert(bi);
						}
					}
				}
				else{										 //������ ��������� ��� ����������� �����
					connectivityListWeight.resize(Number+1);
					for (int i=0; i<=Number; i++){
						getline (fin,str);
						istringstream ist (str);
						while (ist >> bi >> wi){
							connectivityListWeight[i].insert(make_pair(bi,wi));
						}
					}
				}
				break;
			case 'E':										
				fin >> Number >> NumberEdge >> indicatorOrientation >> indicatorWeight;
				int ai;
				if (indicatorWeight == 0){                  //������ ����� ��� ������������� �����
					for (int i=0; i<NumberEdge; i++){
						fin >> ai >> bi;
						edgeList.insert(make_pair(ai,bi));
					}
				}
				else{										//������ ����� ��� ����������� �����
					for (int i=0; i<NumberEdge; i++){
						fin >> ai >> bi >> wi;
						edgeListWeight.insert(make_tuple(ai,bi,wi));
					}
				}
				break;
			}
		}
}

void Graph::addEdge(int from,int to, int weight){
	switch (indicatorRepresentation){
		case 'C':                     //�������� ����� � ������� ���������
			if (indicatorWeight == 0){
				connectivityMatrix[from-1][to-1] = 1;
				NumberEdge++;
			}
			else{
				connectivityMatrix[from-1][to-1] = weight;
				NumberEdge++;
			}
			if (indicatorOrientation == 0){
				connectivityMatrix[to-1][from-1] = weight;
				NumberEdge++;
			}
			break;
		case 'L':
			if (indicatorOrientation == 0){  //�������� ����� � ������ ��������� ������������������ �����
				if (indicatorWeight == 0){
					connectivityList[from].insert(to);
					connectivityList[to].insert(from);
					NumberEdge+=2;
				}
				else{
					connectivityListWeight[from].insert(make_pair(to,weight));
					connectivityListWeight[to].insert(make_pair(from,weight));
					NumberEdge+=2;
				}
			}
			else{							//�������� ����� � ������ ��������� ���������������� �����
				if (indicatorWeight == 0) {
					connectivityList[from].insert(to);
					NumberEdge++;
				}
				else {
					connectivityListWeight[from].insert(make_pair(to,weight));	
					NumberEdge++;
				}
			}
			break;
		case 'E':
			if (indicatorOrientation == 0){   //�������� ����� � ������ ����� ������������������ �����
				if (indicatorWeight == 0){
					edgeList.insert(make_pair(from,to));
					edgeList.insert(make_pair(to,from));
					NumberEdge+=2;
				}
				else{
					edgeListWeight.insert(make_tuple(from,to,weight));
					edgeListWeight.insert(make_tuple(to,from,weight));
					NumberEdge+=2;
				}
			}
			else{								//�������� ����� � ������ ����� ���������������� �����
				if (indicatorWeight == 0) {
					edgeList.insert(make_pair(from,to));
					NumberEdge++;
				}
				else {
					edgeListWeight.insert(make_tuple(from,to,weight));
					NumberEdge++;
				}
			}
			break;
	}
}

void Graph::removeEdge(int from,int to){
	switch (indicatorRepresentation){
		case 'C':							//������� ����� � ������� ���������
			connectivityMatrix[from-1][to-1] = 0;
			NumberEdge--;
			if (indicatorOrientation == 0){
				connectivityMatrix[to-1][from-1] = 0;	 
				NumberEdge--;
			}
			break;
		case 'L':
			if (indicatorOrientation == 0){   //������� ����� � ������ ��������� ������������������ �����
				if (indicatorWeight == 0){
					auto search = connectivityList[from].upper_bound(to);
					connectivityList[from].erase(search);
					search = connectivityList[to].upper_bound(from);
					connectivityList[to].erase(search);
					NumberEdge-=2;
				}
				else{						
					auto search = connectivityListWeight[from].upper_bound(make_pair(to,0));
					connectivityListWeight[from].erase(search);
					search = connectivityListWeight[to].upper_bound(make_pair(from,0));
					connectivityListWeight[to].erase(search);
					NumberEdge-=2;
				}
			}
			else{							//������� ����� � ������ ��������� ���������������� �����
				if (indicatorWeight == 0){
					auto search = connectivityList[from].upper_bound(to);
					connectivityList[from].erase(search);
					NumberEdge--;
				}
				else{
					auto search = connectivityListWeight[from].upper_bound(make_pair(to,0));
					connectivityListWeight[from].erase(search);
					NumberEdge--;
				}
			}
			break;
		case 'E':
			if (indicatorOrientation == 0){    //������� ����� � ������ ����� ������������������ �����
				if (indicatorWeight == 0){
					auto search = edgeList.upper_bound(make_pair(from,to));
					edgeList.erase(search);	
					search = edgeList.upper_bound(make_pair(to,from));
					edgeList.erase(search);
					NumberEdge-=2;
				}
				else{
					auto search = edgeListWeight.upper_bound(make_tuple(from,to,0));
					edgeListWeight.erase(search);	
					search = edgeListWeight.upper_bound(make_tuple(to,from,0));
					edgeListWeight.erase(search);	
					NumberEdge-=2;
				}
			}
			else{                              //������� ����� � ������ ����� ���������������� �����
				if (indicatorWeight == 0){
					auto search = edgeList.upper_bound(make_pair(from,to));
					edgeList.erase(search);	
					NumberEdge--;
				}
				else{
					auto search = edgeListWeight.upper_bound(make_tuple(from,to,0));
					edgeListWeight.erase(search);	
					NumberEdge--;
				}
			}
			break;

	}
}

int Graph::changeEdge(int from,int to,int newWeight){
	switch (indicatorRepresentation){
		case 'C':										//�������� ��� ����� � ������� ���������
			if (indicatorWeight == 1){
				int oldWeight = connectivityMatrix[from-1][to-1];
				connectivityMatrix[from-1][to-1] = newWeight;
				if (indicatorOrientation == 0) connectivityMatrix[to-1][from-1] = newWeight;
				return oldWeight;
			}
			break;
		case 'L':										//�������� ��� ����� � ������ ���������
			if (indicatorWeight == 1){
				auto search = connectivityListWeight[from].upper_bound(make_pair(to,0));
				int oldWeight = search->second;
				connectivityListWeight[from].insert(search, make_pair(to,newWeight));
				connectivityListWeight[from].erase(search);
				if (indicatorOrientation == 0){
					auto search = connectivityListWeight[to].upper_bound(make_pair(from,0));
					connectivityListWeight[to].insert(search, make_pair(from,newWeight));
					connectivityListWeight[to].erase(search);
				}
				return oldWeight;	
			}
			break;
		case 'E':										//�������� ��� ����� � ������ �����
			if (indicatorWeight == 1){
				auto search = edgeListWeight.upper_bound(make_tuple(from,to,0));
				int oldWeight = get<2>(*search);
				edgeListWeight.insert(search, make_tuple(from,to,newWeight));
				edgeListWeight.erase(search);	
				if (indicatorOrientation == 0){
					auto search = edgeListWeight.upper_bound(make_tuple(to,from,0));
					edgeListWeight.insert(search, make_tuple(to,from,newWeight));
					edgeListWeight.erase(search);
				}
				return oldWeight;
			}
			break;
	}
	
}

void Graph::transformToAdjMatrix(){       //������������� � ������� ���������
	switch (indicatorRepresentation){
		case 'C':	
			break;
		case 'L':
			connectivityMatrix.clear();
			connectivityMatrix.resize(Number);
			for (int i=0; i<Number; i++){
				connectivityMatrix[i].resize(Number,0); 
			}
			indicatorRepresentation = 'C';
			if (indicatorWeight == 0){       //������� ������ ��������� ������������� ����� � ������� ���������
				for (int ai = 0; ai <= Number; ai++){
					for (set <int >::iterator bi = connectivityList[ai].begin(); bi != connectivityList[ai].end(); ++bi){
						connectivityMatrix[ai-1][*bi-1] = 1;
					}
				}	
			}
			else{                           //������� ������ ��������� ����������� ����� � ������� ���������
				for (int ai = 0; ai <= Number; ai++){
					for (set <pair <int,int> >::iterator bi = connectivityListWeight[ai].begin(); bi != connectivityListWeight[ai].end(); ++bi){
						connectivityMatrix[ai-1][bi->first-1] = bi->second;
					}
				}	
			}
			break;
		case 'E':
			connectivityMatrix.clear();
			connectivityMatrix.resize(Number);
			for (int i=0; i<Number; i++){
				connectivityMatrix[i].resize(Number,0); 
			}
			indicatorRepresentation = 'C';
			if (indicatorWeight == 0){      //������� ������ ����� ������������� ����� � ������� ���������
				for (set <pair <int, int> >::iterator bi = edgeList.begin(); bi != edgeList.end(); ++bi){
					connectivityMatrix[bi->first-1][bi->second-1] = 1;
				}		
			}
			else {                          //������� ������ ����� ����������� ����� � ������� ���������
				for (set <tuple <int, int, int> >::iterator bi = edgeListWeight.begin(); bi != edgeListWeight.end(); ++bi){
					connectivityMatrix[ get<0>(*bi)-1][get<1>(*bi)-1] = get<2>(*bi);
				}
			}
			break;
	}
}

void Graph::transformToAdjList(){          //������������� � ������ ���������
	switch (indicatorRepresentation){
		case 'C':
			indicatorRepresentation = 'L';
			if (indicatorWeight == 0){      //������� ������� ��������� ��� ������������� ����� � ������ ���������
				connectivityList.resize(Number+1);
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						if (connectivityMatrix[i][j] != 0) connectivityList[i+1].insert(j+1);
					}
				}
			}
			else{                           //������� ������� ��������� ��� ����������� ����� � ������ ���������
				connectivityListWeight.resize(Number+1);
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						if (connectivityMatrix[i][j] != 0) connectivityListWeight[i+1].insert(make_pair(j+1,connectivityMatrix[i][j]));
					}
				}
			}
			break;
		case 'L':
			break;
		case 'E':
			indicatorRepresentation = 'L';
			if (indicatorWeight == 0){      //������� ������ ����� ��� ������������� ����� � ������ ���������
				connectivityList.resize(Number+1);
				for (set <pair <int, int> >::iterator bi = edgeList.begin(); bi != edgeList.end(); ++bi){
					connectivityList[bi->first].insert(bi->second);
				}		
			}
			else {                          //������� ������ ����� ��� ����������� ����� � ������ ���������
				connectivityListWeight.resize(Number+1);
				for (set <tuple <int, int, int> >::iterator bi = edgeListWeight.begin(); bi != edgeListWeight.end(); ++bi){
					connectivityListWeight[get<0>(*bi)].insert(make_pair(get<1>(*bi), get<2>(*bi)));
				}
			}
			break;
	}
}

void Graph::transformToListOfEdges(){        //������������� � ������ �����
	switch (indicatorRepresentation){
		case 'C':	
			indicatorRepresentation = 'E', NumberEdge = 0;
			if (indicatorWeight == 0){       //������� ������� ��������� ������������� ����� � ������ �����
				edgeList.clear();
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						if (connectivityMatrix[i][j] == 1) {
							edgeList.insert(make_pair(i+1,j+1));
							NumberEdge++;
						}
					}
				}
			}
			else{							 //������� ������� ��������� ����������� ����� � ������ �����
				edgeListWeight.clear();  
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						if (connectivityMatrix[i][j] != 0) {
							edgeListWeight.insert(make_tuple(i+1,j+1,connectivityMatrix[i][j]));
							NumberEdge++;
						}
					}
				}

			}
			break;
		case 'L':
			indicatorRepresentation = 'E', NumberEdge = 0;
			if (indicatorWeight == 0){           //������� ������ ��������� ������������� ����� � ������ �����        
				edgeList.clear();
				for (int ai = 0; ai <= Number; ai++){
					for (set <int >::iterator bi = connectivityList[ai].begin(); bi != connectivityList[ai].end(); ++bi){
						edgeList.insert(make_pair(ai,*bi));
						NumberEdge++;
					}
				}	
			}
			else {                               //������� ������ ��������� ����������� ����� � ������ ����� 
				edgeListWeight.clear();
				for (int ai = 0; ai <= Number; ai++){
					for (set <pair <int,int> >::iterator bi = connectivityListWeight[ai].begin(); bi != connectivityListWeight[ai].end(); ++bi){
						edgeListWeight.insert(make_tuple(ai,bi->first,bi->second));
						NumberEdge++;
					}
				}
			}
			break;
		case 'E':
			break;
	   }		
}

void Graph::writeGraph(string fileName){
	ofstream fout (fileName); 
	switch (indicatorRepresentation){
			case 'C':														//����� ������� ���������
				fout << indicatorRepresentation << ' ' << Number << ' ' << indicatorWeight << endl;
				for (int i=0; i<Number; i++){
					for (int j=0; j<Number; j++){
						fout << connectivityMatrix[i][j] << ' ';
					}
					fout << endl;
				}
				fout.close();
				break;

			case 'L':
				fout << indicatorRepresentation << ' ' << Number << endl
					 << indicatorOrientation << ' ' << indicatorWeight << endl;
				if (indicatorWeight == 0){                            //����� ������ ��������� ��� ������������� �����
					for (vector <set <int> >::iterator i = connectivityList.begin()+1; i != connectivityList.end(); ++i){
						for (set <int >::const_iterator j = i->begin(); j != i->end(); ++j){
							fout << *j << ' ';
						}
						fout << endl;
					}
				}
				else{														//����� ������ ��������� ��� ����������� �����
					for (vector <set <pair <int,int> > >::iterator i = connectivityListWeight.begin()+1; i != connectivityListWeight.end(); ++i){
						for (set <pair <int,int> >::const_iterator j = i->begin(); j != i->end(); ++j){
							fout << j->first << ' ' << j->second << ' ';
						}
						fout << endl;
					}
				}
				fout.close();
				break;

			case 'E':
				fout << indicatorRepresentation << ' ' << Number << ' ' << NumberEdge << endl
					 << indicatorOrientation << ' '<< indicatorWeight << endl;
				if (indicatorWeight == 0){                                  //����� ������ ����� ��� ������������� �����
					for (set <pair <int,int> >::iterator i = edgeList.begin(); i != edgeList.end(); ++i){
						fout << i->first << ' ' << i->second << endl;
					}
				}
				else{														//����� ������ ����� ��� ����������� �����
					for (set <tuple <int,int,int> >::iterator i = edgeListWeight.begin(); i != edgeListWeight.end(); ++i){
						fout << get<0>(*i) << ' ' << get<1>(*i) << ' ' << get<2>(*i) <<endl;
					}
				}
				fout.close();
				break;
	}
}