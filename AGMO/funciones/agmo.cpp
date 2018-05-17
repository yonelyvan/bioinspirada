#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;
double Pi= 3.14159;
double inf = 10000000.00001;

typedef struct{
	vector<double> cro;
	double fitness;
}individuo;

typedef vector<individuo> poblacion;
typedef vector<double> vd;
typedef vector<int> vi;
double getRandom(double li, double ls){  return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));}
double fx(individuo I){ return 4.0*pow(I.cro[0],2.0) + 4.0*pow(I.cro[1],2.0); }
double gx(individuo I){ return pow((I.cro[0]-5),2.0) + pow((I.cro[1]-5),2.0); }

double w[]={1,1};

bool Mejor(individuo a, individuo b){
	if(a.fitness < b.fitness)//< minimizar | > maximizar
		return true;
	return false;
}

//ordenamiento fot fx |(x,y) == (fx,gx)
bool decresiente(individuo a, individuo b){
	double fa = fx(a);
	double fb = fx(b);
	if( fa > fb){
		return true;
	}else{
		return false;
	}
}

// 0<= x <=5
// 0<= y <=3
double mix_x = 0;
double max_x = 5;
double min_y = 0;
double max_y = 3;
#define PROB_MUT 1 // 0.5-1% [0-100]Probabilidad de Mutación: 0.05

double get_fitness(individuo I){
	return w[0]*fx(I) + w[1]*gx(I);
}

bool es_valido(individuo &I){
	if(I.cro[0] < mix_x)
		I.cro[0] = mix_x;
	if(I.cro[1] < min_y)
		I.cro[1] = min_y;

	if(I.cro[0] > max_x)
		I.cro[0] = max_x;	
	if(I.cro[1] > max_y)
		I.cro[1] = max_y;
	I.fitness = w[0]*fx(I) + w[1]*gx(I);
	
	double x=I.cro[0];
	double y=I.cro[1];
	double ff1 = pow((x-5),2.0) + pow(y,2.0) - 25;
	double ff2 = -1*pow((x-8),2.0) - pow((y+3),2.0) +7.7;
	if( 0>=ff1 && 0>=ff2){
		return true;
	}
	return false;
}

poblacion get_poblacion_inicial(int tam_poblacion){
	poblacion P;
	for (int i = 0; i < tam_poblacion;){
		individuo I;
		double x= getRandom(mix_x, max_x);
		double y= getRandom(min_y, max_y);
		I.cro.push_back(x);
		I.cro.push_back(y);
		I.fitness = w[0]*fx(I) + w[1]*gx(I);
		if( es_valido(I)){
			P.push_back(I);
			++i;
		}
	}
	return P;
}


void imprimir_poblacion(poblacion P){
	for (int i = 0; i < P.size(); ++i){
		individuo I = P[i];
		cout<<i+1<<") "<<fx(I)<<" "<<gx(I)<<"	"<<"	|"<<I.fitness<<endl;
		//cout<<i+1<<") "<<I.cro[0]<<" "<<I.cro[1]<<"	"<<"	|"<<   <<I.fitness<<endl;
		//cout<<i+1<<") "<<I.cro[0]<<"	"<<I.cro[0]<<"|	"<<I.fitness<<endl;
	}
}

poblacion ruleta(poblacion &P){
	int total=0;
	for (int i = 0; i < P.size(); ++i){
		total+=P[i].fitness;
	}
	double cont=0;
	vd v_pro;//ruleta
	for (int i = 0; i < P.size(); ++i){
		cont += (P[i].fitness*100.0)/total;
		v_pro.push_back(cont); 
	}
	//seleccion
	poblacion seleccionados;

	for (int i = 0; i < P.size(); ++i){//P.size()
		int s= rand()%100;
		for (int j = 0; j < v_pro.size(); ++j){//verificando a q rango pertenece
			if( s <= v_pro[j] ){
				seleccionados.push_back(P[j]);
				break;
			}
		}
	}
	return seleccionados;
}

poblacion torneo(poblacion P){
	poblacion seleccionados;

	int tam_torneo = 3;
	for (int i = 0; i < P.size(); ++i){//seleccionar dos padres
		poblacion p_torneo;
		for (int i = 0; i < tam_torneo; ++i){
			p_torneo.push_back( P[rand()%P.size()] );
		}
		sort(p_torneo.begin(), p_torneo.end(), Mejor);	
		seleccionados.push_back(p_torneo[0]);
	}
	return seleccionados;
}

poblacion seleccion(poblacion &P){
	//return ruleta(P);
	return torneo(P);
}

poblacion cruzamiento_blx(poblacion &P){
	poblacion hijos;

	for (int i = 0; i < P.size();){
		int h = rand()%P.size();
		int k = rand()%P.size();
		double B= getRandom(-5.0,1.5); ///
		//cruzar
		individuo I;
		I.cro.resize(2);
		//p1 + B (P2 - P1)
		I.cro[0] = P[h].cro[0] + B*( P[k].cro[0]-P[h].cro[0] );
		I.cro[1] = P[h].cro[1] + B*( P[k].cro[1]-P[h].cro[1] );
		I.fitness = w[0]*fx(I) + w[1]*gx(I);
		if(es_valido(I)){
			hijos.push_back(I);
			++i;
		}
	}
	return hijos;
}

void mutar(poblacion &P){
	for (int i = 0; i < P.size();++i){
		int pro_mut = rand()%100;
		if(pro_mut <= PROB_MUT ){
			individuo I;
			cout<<"Mutacion I: "<< i+1 <<endl;
			do{
				I=P[i];
				int k = rand()%2;
				if(k==0){ //en x
					I.cro[0]=getRandom(mix_x, max_x);
				}
				if(k==1){ //en y
					I.cro[1]=getRandom(min_y, max_y);
				}
			}while (!es_valido(I));
			P[i].cro=I.cro;
			P[i].fitness = get_fitness(P[i]);
		}
	}
}

//retorna la primera frontera y el resto
vector<poblacion> get_frontier(poblacion P){
	poblacion p_frontera;//fontera
	poblacion P_resto; //no estan en la frontera
	sort(P.begin(), P.end(), decresiente);//ordenar soluciones en x o fx

	stack<individuo> front;
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


double distancia(individuo I1, individuo I2){
	double dx = abs(fx(I1) - fx(I2));//x
	double dy = abs(gx(I1) - gx(I1));//y
	return 2*dx + 2*dy;
}



void run(){
	int iteraciones = 10;
	int tam_poblacion = 50;
	double D = 4.0;
	poblacion P = get_poblacion_inicial( tam_poblacion );
	for (int it = 1; it <= iteraciones; ++it){
		cout<<"__________________ Iteracion "<<it<<" __________________"<<endl;
		cout<<"poblacion"<<endl;
		imprimir_poblacion(P);	

		cout<<"seleccion"<<endl;
		poblacion seleccionados = seleccion(P);
		//imprimir_poblacion(seleccionados);
		
		cout<<"____hijos_____"<<endl;
		poblacion hijos = cruzamiento_blx(seleccionados);//hijos (1)	
		imprimir_poblacion(hijos);
		//agregar hijos a la poblacion
		for (auto I: hijos){//
			P.push_back(I);
		}
		mutar(P);
		cout<<"Nueva Poblacion"<<endl;
		
		poblacion new_P;
		vector<poblacion> fronteras = get_fronteras(P);
		
		/* Nueva Poblacion*/
		int cont =0;
		int fl=1;
		int index_f=0;
		while(fl){
			for (auto F : fronteras){
				index_f++;
				//cout<<"frontera: "<<index_f<<endl;
				for(int i=0; i<F.size(); i++){//frontera ordenada cresiente
					if(i==0 || i==F.size()-1){
						//cout<<i+1<<") "<<fx(F[i])<<" "<<gx(F[i])<<"	"<<"	|"<<F[i].fitness<<endl;
						new_P.push_back(F[i]);
						cont++;
					}else{
						if( distancia(F[i-1], F[i+1]) > D){//distancia
							//cout<<i+1<<") "<<fx(F[i])<<" "<<gx(F[i])<<"	"<<"	|"<<F[i].fitness<<endl;
							new_P.push_back(F[i]);
							cont++;
						}
					}
					if(cont == tam_poblacion){fl=0; break;}
				}
				if(cont == tam_poblacion){fl=0; break;}
			}
		}
		P=new_P;
		imprimir_poblacion(P);
	}
}


void test(){
	poblacion P = get_poblacion_inicial( 100 );
	see("frontera"); 
	vector<poblacion> fronteras = get_fronteras(P);
	for( auto I:P){
		cout<<fx(I)<<" "<<gx(I)<<endl;
	}

	for (auto F : fronteras){
		cout<<"__________frontera__________"<<endl;
		for(auto I : F){
			cout<<fx(I)<<" "<<gx(I)<<endl;
		}
	}
}


int main(){
	//test();
	run();
	return 0;
}