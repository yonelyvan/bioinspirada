#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;

double getRandom(double li, double ls){  return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));}

typedef struct{
	int x;
	int y;
	int vx;
	int vy;
}particula;

typedef vector<particula> poblacion;

poblacion get_poblacion(int tam){
	poblacion P;
	for (int i = 0; i < tam; ++i){
		particula pp;
		pp.x=getRandom(0,1); 		
		pp.y=getRandom(0,1); 		
		pp.vx=getRandom(0,1); 		
		pp.vy=getRandom(0,1);
		P.push_back(pp);
	}
	return P;
}


void run(){
	int tan_pob=4;
	poblacion P;
	P = get_poblacion(tan_pob);
	
}

int main(){
	run();
	return 0;
}