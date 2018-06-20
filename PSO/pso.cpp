#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;

double getRandom(double li, double ls){  return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));}

typedef struct{
	double x;
	double y;
	double vx;
	double vy;
	double fitness;
}particula;

typedef vector<particula> poblacion;

bool Mejor(particula a, particula b){
	if(a.fitness < b.fitness)//< minimizar | > maximizar
		return true;
	return false;
}


double fx(float x, float y){	return pow(x,2)+pow(y,2); }

double LI = -5;
double LS = 5;

poblacion get_poblacion(int tam){
	poblacion P;
	for (int i = 0; i < tam; ++i){
		particula pp;
		pp.x=getRandom(LI,LS); 		
		pp.y=getRandom(LI,LS); 		
		pp.vx=getRandom(LI,LS); 		
		pp.vy=getRandom(LI,LS);
		P.push_back(pp);
	}
	return P;
}

void calular_fitness(poblacion &P){
	for (int i = 0; i < P.size(); ++i){
		P[i].fitness = fx(P[i].x, P[i].y);
	}
}

void imprimir_poblacion(poblacion P){
	int i=0;
	for (particula pp : P){
		++i;
		cout<<i<<") x1 = "<<pp.x<<" "<<"	x2 = "<<pp.y<<"	v1 = "<<pp.vx<<"	v2 = "<<pp.vy<<endl;
	}
}

void imprimir_finess(poblacion &P){
	cout<<"____Fitness_____"<<endl;
	int i=0;
	for (particula pp : P){
		++i;
		cout<<i<<") "<<pp.fitness<<endl; 
	}
}

particula mejor(poblacion P){
	sort(P.begin(), P.end(), Mejor);
	return P[0];
}

void calcular_velocidades(poblacion &P, particula mejor_l, particula mejor_g){
	double phi_1 = 2.0;
	double phi_2 = 2.0;
	double rand_1 = getRandom(0,1);
	double rand_2 = getRandom(0,1);
	double w = getRandom(0,1);
	for (int i = 0; i < P.size(); ++i){
		//vx
		rand_1 = getRandom(0,1);
		rand_2 = getRandom(0,1);
		w = getRandom(0,1);
		P[i].vx = w*P[i].vx + phi_1*rand_1*(mejor_l.x-P[i].x) + phi_2*rand_2*(mejor_g.x-P[i].x);
		//vy
		rand_1 = getRandom(0,1);
		rand_2 = getRandom(0,1);
		w = getRandom(0,1);
		P[i].vy = w*P[i].vy + phi_1*rand_1*(mejor_l.y-P[i].y) + phi_2*rand_2*(mejor_g.y-P[i].y);
		
		//x
		P[i].x = P[i].y + P[i].vx;
		//y
		P[i].y = P[i].y + P[i].vy;
		//
		if( P[i].x > LS  ){ P[i].x = LS;}
		if( P[i].y > LS  ){ P[i].y = LS;}
		if( P[i].x < LI  ){ P[i].x = LI;}
		if( P[i].y < LI  ){ P[i].y = LI;}
	}
}

void run(){
	int tan_pob=4;
	int num_it = 3;
	poblacion P;
	P = get_poblacion(tan_pob);
	imprimir_poblacion(P);
	calular_fitness(P);
	imprimir_finess(P);
	particula mejor_g;
	particula mejor_l;
	mejor_g = mejor(P);
	mejor_l = mejor_g;
	for (int it = 0; it < num_it; ++it){
		cout<<"________________"<<it<<"________________"<<endl;
		cout<<"Mejor :"<<" x1="<<mejor_g.x<<"	x2="<<mejor_g.y<<"	fitness="<<mejor_g.fitness<<endl;
		cout<<"Cúmulo de Particulas Siguietes"<<endl;
		calcular_velocidades(P,mejor_l, mejor_g);
		imprimir_poblacion(P);
		calular_fitness(P);
		mejor_l = mejor(P);
		if(mejor_l.fitness < mejor_g.fitness){
			mejor_g = mejor_l;
		}
	}	
}


int main(){
	run();
	return 0;
}