#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <list>
#include <iterator>
#include <fstream>
#include <cstdio>
#include <sstream>

using namespace std;


static int Dim=2;
typedef struct {
	double val[2];
	double fit;
	int copies;
	int rank;
	bool muta;
	double prob;
}celula;

typedef vector<celula> Grupo;


bool compare(celula a, celula B){
	if(a.fit< B.fit) return true; return false;
}

bool isthesame (celula i, celula j) {
	if( i.val[0]==j.val[0] && i.val[1] == j.val[1])
  		return true;
  	return false;
}

double genRandom(double li, double ls){
	return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));
}

void printcelula(string a, celula v){
	cout<<a<<"<"<<v.val[0]<<","<<v.val[1]<<"> - "<<v.fit<<"\n";
}

void printClone(string a, celula v){
	cout<<a<<"<"<<v.val[0]<<","<<v.val[1]<<"> - Fit: "<<v.fit<<" cop: "<<v.copies<<" rank: "<<v.rank<<" Muta: "<<v.muta<<" Prob: "<<v.prob<<"\n";
}

void printGrupo(string a, Grupo & A){
	cout<<a;
	for (int i = 0; i < A.size(); i++){
		printcelula("",A[i]);
	}
}

void printGrupoClones(string a, Grupo & A){
	cout<<a;
	for (int i = 0; i < A.size(); i++){
		printClone("",A[i]);
	}
}

void evaluatecelula(celula &a){
	a.fit=a.val[0]*a.val[0]+a.val[1]*a.val[1];
}

void evaluateGrupo(Grupo &B){
	for (int i = 0; i < B.size(); i++){
		evaluatecelula(B[i]);
	}
}

void StartPopulation(Grupo &B, int paramN, double li, double ls){
	cout<<"Imprimiendo Poblacion Inicial\n"<<endl;
	B.resize(paramN);
	for(int i=0; i<paramN;i++){
		B[i].val[0]= genRandom(li,ls);B[i].val[1]=genRandom(li,ls);
		evaluatecelula(B[i]);
		printcelula("",B[i]);
	}
	sort(B.begin(), B.end(),compare);
}

void Clone(Grupo &B, Grupo &C,double paramBeta , int paramN){	
	int nc=0;
	for(int i=1;i<=B.size();i++){
		//cout<<"entra"<<endl;
		nc=round(paramBeta*paramN)/i;
		//cout<<"clones"<<nc<<endl;
		for(int k=0;k<nc;k++){
			B[i-1].copies=nc;
			B[i-1].rank=i;
			C.push_back(B[i-1]);
		}
	}		
}

int probSeleccion(Grupo & B){
	celula max=*(max_element(B.begin(), B.end(),compare));
	cout<<"\n max fitnes: "<<max.fit<<endl;
	double ruleta=0; 
	for(int i=0;i<B.size();i++){
		B[i].prob=B[i].fit/max.fit;
	}
}

void mutation(Grupo & G,double li, double ls){
	for (int i = 0; i < G.size(); i++){
		double r= genRandom(0,1);
		if(r<G[i].prob){
			int p=round(r);
			G[i].val[p]=genRandom(li,ls);
			G[i].muta=true;
		}	
	}
}

void reemplazo(Grupo & G, Grupo & C, int times,double li, double ls){
	for(int i = 0; i < times; ++i){
		G[i]=C[i];
	}
	for(int i = times; i < G.size(); ++i){
		G[i].val[0]= genRandom(li,ls);
		G[i].val[1]=genRandom(li,ls);
	}
}

int main(){
	srand (time(NULL));
	double li=-5;
	double ls=5;
	int paramN=8; 
	int paramD=4;
	double paramBeta=1;
	int iteraciones=1;
	Grupo G;
	Grupo C;
	StartPopulation(G, paramN,li,ls);
	printGrupo("\nPoblacion Ordenada\n\n",G);
	int times=0;
	while(times<iteraciones){
		cout<<"----------- ITERACION "<<times<<"-----------"<<endl;
		C.clear();
		Clone(G,C,paramBeta,paramN);
		probSeleccion(C);
		printGrupoClones("\nClones\n\n", C);
		mutation(C,li,ls);
		evaluateGrupo(C);
		printGrupoClones("\nClones Maduros\n\n", C);
		sort(C.begin(),C.end(),compare);
			auto last = std::unique(C.begin(), C.end(),isthesame);
	    	C.erase(last, C.end());
		printGrupoClones("\nClones Maduros - Unicos \n\n", C);
		int t=paramN-paramD;
		reemplazo(G,C,t,li,ls);
		printGrupo("\n Poblacion Generada\n\n",G);
		times++;
	}	
}