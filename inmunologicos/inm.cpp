#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;

typedef vector<int> vi;
typedef vector<double> vd;
typedef struct {
	vi cro; 
	double val[2];
	double fitness;
	double prob;
	double afinidad;
	double mutacion_rate;
}celula;

typedef vector<celula> Poblacion;
#define TAM_CROM 32

bool Mejor(celula a, celula B){
	if(a.fitness < B.fitness) return true; return false;
}

bool isthesame (celula i, celula j) {
	if( i.val[0]==j.val[0] && i.val[1] == j.val[1])
  		return true;
  	return false;
}

double genRandom(double li, double ls){
	return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));
}



void calcular_bin_2_dec(celula &C){
	int Flag = C.cro.size()/2;
	double x =0;
	double y =0;
	for (int j = 0; j < C.cro.size(); ++j){
		if( j < Flag ){
			x=x+(pow(2,j))*C.cro[j];
		}else{
			y=y+(pow(2,j-Flag))*C.cro[j];
		}
	}
	C.val[0]= -5.0 + (10.0)/(pow(2.0,16.0)-1)*x;
	C.val[1]= -5.0 + (10.0)/(pow(2.0,16.0)-1)*y;
}


string get_str_cro(celula C){
	string c="";
	for(auto e :C.cro){
		c+= to_string(e);
	}
	return c;
}

void printcelula(string a, celula C){
	cout<<"x="<<get_str_cro(C)<<"\n";
	cout<<a<<"["<<C.val[0]<<","<<C.val[1]<<"]	fit: "<<C.fitness<<"\n";
}

void print_afinidad(Poblacion P){
	for(celula C:P){
		cout<<"["<<C.val[0]<<","<<C.val[1]<<"]	fit: "<<C.fitness<<"	a= "<<C.afinidad<<endl;
	}
}



void printPoblacion(string a, Poblacion & A){
	cout<<a;
	for (int i = 0; i < A.size(); i++){
		printcelula("",A[i]);
	}
}



void evaluar_celula(celula &a){
	a.fitness = pow( a.val[0],2.0)+pow( a.val[1],2.0);
}

void evaluar_Poblacion(Poblacion &B){
	for (int i = 0; i < B.size(); i++){
		evaluar_celula(B[i]);
	}
}

void calcular_afinidad(Poblacion &P){
	Poblacion temp = P;
	sort(temp.begin(), temp.end(),Mejor);
	double f_min = temp[0].fitness;
	double f_max = temp.back().fitness;
	for(celula &C: P){
		C.afinidad = 1.0 - (C.fitness/(f_max-f_min));
	}
}


void iniciar_poblacion(Poblacion &P, int P_size, double li, double ls){
	cout<<"Poblacion Inicial:"<<endl;
	P.resize(P_size);
	for(int i=0; i<P_size;i++){
		vi c(TAM_CROM);
		for (int i = 0; i < TAM_CROM; ++i){
			c[i]=rand()%2;
		}
		P[i].cro = c;
		calcular_bin_2_dec(P[i]);//calcula x1 y x2
		evaluar_celula(P[i]);
		printcelula("",P[i]); //imprimendo [x1,x2,fit]
	}
}


void reemplazo(Poblacion & G, Poblacion & C, int times,double li, double ls){
	for(int i = 0; i < times; ++i){
		G[i]=C[i];
	}
	for(int i = times; i < G.size(); ++i){
		G[i].val[0]= genRandom(li,ls);
		G[i].val[1]=genRandom(li,ls);
	}
}


Poblacion seleccion(Poblacion &P, int seleccion_size){
	double total;
	for(celula C : P){
		total +=C.fitness;
	}

	double cont=0;
	vd v_pro;//ruleta
	for (int i = 0; i < P.size(); ++i){
		cont += (P[i].fitness*100.0)/total;
		v_pro.push_back(cont); 
	}
	//seleccion
	Poblacion seleccionados;

	for (int i = 0; i < seleccion_size; ++i){
		double s= rand()%100;
		for (int j = 0; j < v_pro.size(); ++j){//verificando a q rango pertenece
			if( s <= v_pro[j] ){
				seleccionados.push_back(P[i]);
				break;
			}
		}
	}
	return seleccionados;
}



void clonar(Poblacion &Clones,celula C, double clone_rate, int P_size, double mutacion_factor){
	int cantidad_clones = P_size *clone_rate;
	for (int i = 0; i < cantidad_clones; ++i){
		double mutacion_rate = exp(mutacion_factor*C.afinidad);
		
		celula CC;
		vi c(TAM_CROM);
		for (int i = 0; i < TAM_CROM; ++i){
			c[i]=rand()%2;
		}
		CC.cro = c;
		CC.mutacion_rate = mutacion_rate;
		calcular_bin_2_dec(CC);//calcula x1 y x2
		evaluar_celula(CC);
		Clones.push_back(CC);
	}
}

void hypermutate(celula &C){
	cout<<"hypermutate"<<endl;
}


int main(){
	srand (time(NULL));
	double li=-5;
	double ls=5;
	int P_size=4; 
	int seleccion_size=2;
	double clone_rate = 0.5;
	double mutacion_factor = -2.5;
	int iteraciones=1;
	Poblacion P;
	Poblacion C;
	iniciar_poblacion(P, P_size,li,ls);
	//sort(G.begin(), G.end(),Mejor);
	//printPoblacion("\nPoblacion Ordenada\n\n",G);
	int it=0;
	while(it < iteraciones){
		C.clear();
		cout<<"----------- ITERACION "<<it<<"-----------"<<endl;
		printPoblacion("\nPoblacion:\n",P);
		cout<<"\nCalcular afinidad:"<<endl;
		calcular_afinidad(P); print_afinidad(P);

		Poblacion Seleccionados = seleccion(P,seleccion_size);
		Poblacion Clones;
		for (celula C : Seleccionados){
		 	clonar(Clones,C,clone_rate,P_size,mutacion_factor);
		} 

		for (celula C : Clones){
			hypermutate(C);
		}
		calcular_afinidad(Clones);





		it++;
	}
}