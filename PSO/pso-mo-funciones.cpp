#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;


double getRandom(double li, double ls){  return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));}

typedef vector<double> vd;

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

double w[]={1,1}; //wfx, wgx
double fx(particula I){ return 4.0*pow(I.x,2.0) + 4.0*pow(I.y,2.0); }
double gx(particula I){ return pow((I.x-5),2.0) + pow((I.y-5),2.0); }

double LI = -5;
double LS = 5;


//ordenamiento fot fx |(x,y) == (fx,gx)
bool decresiente(particula a, particula b){
	double fa = fx(a);
	double fb = fx(b);
	if( fa > fb){
		return true;
	}else{
		return false;
	}
}


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
		P[i].fitness = w[0]*fx(P[i]) + w[1]*gx(P[i]);
	}
}

double get_fitness(particula I){
	return w[0]*fx(I) + w[1]*gx(I);
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
	//double phi_1 = 2.0;
	//double phi_2 = 2.0;
	double rand_1 = getRandom(0,1);
	double rand_2 = getRandom(0,1);
	double w = getRandom(0,1);
	for (int i = 0; i < P.size(); ++i){
		//vx
		rand_1 = getRandom(0,1);
		rand_2 = getRandom(0,1);
		w = getRandom(0,1);
		P[i].vx = w*P[i].vx + rand_1*(mejor_l.x-P[i].x) + rand_2*(mejor_g.x-P[i].x);
		//vy
		rand_1 = getRandom(0,1);
		rand_2 = getRandom(0,1);
		w = getRandom(0,1);
		P[i].vy = w*P[i].vy + rand_1*(mejor_l.y-P[i].y) + rand_2*(mejor_g.y-P[i].y);
		
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

//=================================================
//retorna la primera frontera y el resto
vector<poblacion> get_frontier(poblacion P){
	poblacion p_frontera;//fontera
	poblacion P_resto; //no estan en la frontera
	sort(P.begin(), P.end(), decresiente);//ordenar soluciones en x o fx

	stack<particula> front;
	front.push(P[0]);
	for (int i = 1; i < P.size(); ++i){
	    while( !front.empty() ){
	    	double top_x = fx( front.top() );
	    	double top_y = gx( front.top() );
	    	double x = fx( P[i]);
	    	double y = gx( P[i]);
	    	if( top_x >= x  &&  top_y >= y){
	    		//cout<<top_x<<">="<<x<<"	"<<top_y<<">="<<y<<endl;
	    		//see("enter");	
	    		P_resto.push_back(front.top());    	
	    		front.pop();
	    	}
	    	else{
	    		break;
	    	}
	    }    	
		front.push(P[i]);
	}
	while( !front.empty() ){
		p_frontera.push_back(front.top());
		//cout<<fx(front.top(),front.top())<<" "<<gx(front.top(),front.top())<<endl;
		front.pop();
	}
	vector<poblacion> fronteras;
	fronteras.push_back( p_frontera );
	fronteras.push_back( P_resto );
	return fronteras;
}

vector<poblacion> get_fronteras(poblacion P){
	poblacion contenedor=P;
	vector<poblacion> fronteras;
	while(contenedor.size()>0){
		vector<poblacion> r = get_frontier(contenedor);
		fronteras.push_back(r[0]);
		contenedor = r[1];
	}
	return fronteras;
}

double distancia(particula I1, particula I2){
	double dx = abs(fx(I1) - fx(I2));//x
	double dy = abs(gx(I1) - gx(I1));//y
	return 2*dx + 2*dy;
}

poblacion get_poblacion_frontera(poblacion P, particula &m_local){
	double D = 20.0;
	int tam_poblacion = P.size();
	poblacion new_P;
	vector<poblacion> fronteras = get_fronteras(P);
	// Nueva Poblacion
	int cont =0;
	int fl=1;
	
	int index_f=0;
	while(fl){
		for (auto F : fronteras){
			index_f++;
			cout<<"frontera: "<<index_f<<endl;
			for(int i=0; i<F.size(); i++){//frontera ordenada cresiente
				if(i==0 || i==F.size()-1){
					cout<<i+1<<") "<<fx(F[i])<<" "<<gx(F[i])<<"	"<<"	|"<<F[i].fitness<<endl;
					new_P.push_back(F[i]);
					cont++;
				}else{
					if( distancia(F[i-1], F[i+1]) > D){//distancia
						cout<<i+1<<") "<<fx(F[i])<<" "<<gx(F[i])<<"	"<<"	|"<<F[i].fitness<<endl;
						new_P.push_back(F[i]);
						cont++;
					}
				}
				if(cont == tam_poblacion){fl=0; break;}
			}
			if(cont == tam_poblacion){fl=0; break;}
		}
	}
	int selected = rand()%(fronteras[0].size());
	m_local = fronteras[0][ selected ];
	return new_P;
}


//===============================================

poblacion unir_v(poblacion a, poblacion b){
	for (auto e:b){
		a.push_back(e);
	}
	return a;
}


void run(){
	poblacion P_frontera;
	int tan_pob=20;
	int num_it = 10;
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
		poblacion P = get_poblacion_frontera(P,mejor_l);//mejor local
		//P = unir_v(P_frontera,P);
		//vector<poblacion> fronteras = get_fronteras(P);
		//P_frontera = fronteras[0];
		if( fx(mejor_l) < fx(mejor_g) && gx(mejor_l) < gx(mejor_g) ){
			mejor_g = mejor_l;
		}

	}	
}


int main(){
	run();
	return 0;
}