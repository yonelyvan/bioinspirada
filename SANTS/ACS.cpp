#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;
//double Pi= 3.14159;
double inf = 10000000.00001;

const int TAM_CROM = 7;//5;//10;
char LABELS[]={'A','B','C','D','E','F','G','H','I','J'}; 

/*
double DISTANCIAS[TAM_CROM][TAM_CROM]={
					{ 0,50,	50,	94, 50},
					{50, 0,	22,	50, 36},
					{50,22,	 0, 44,	14},
					{94,50,	44,	 0,	50},
					{50,36,	14,	50,  0}
					};

double FLUJO[TAM_CROM][TAM_CROM]={
					{0,0,2,0,3},
					{0,0,0,3,0},
					{2,0,0,0,0},
					{0,3,0,0,1},
					{3,0,0,1,0}
					};
*/
double DISTANCIAS[TAM_CROM][TAM_CROM]{
	{0,35,71,99,71,75,41},
	{35,0,42,80,65,82,47},
	{71,42,0,45,49,79,55},
	{99,80,45,0,36,65,65},
	{71,65,49,36,0,31,32},
	{75,82,79,65,31,0,36},
	{41,47,55,65,32,36,0}
};


double FLUJO[TAM_CROM][TAM_CROM]{
	{0,2,0,0,0,0,2},
	{2,0,3,0,0,1,0},
	{0,3,0,0,0,1,0},
	{0,0,0,0,3,0,1},
	{0,0,0,3,0,0,0},
	{0,1,1,0,0,0,0},
	{2,0,0,1,0,0,0}
};

typedef vector<int> vi;
typedef vector<double> vd;
typedef vector< vector<int> > mi;
typedef vector< vector<double> > md;
typedef struct{
	vi camino;
	int nodo_actual;
	double longitud;
}hormiga;

typedef vector<hormiga> poblacion;
double getRandom(double li, double ls){  return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));}

bool Mejor(hormiga a, hormiga b){
	if(a.longitud < b.longitud)//< minimizar | > maximizar
		return true;
	return false;
}



class sis_hormigas{
	public:
		double p_evaporacion=0.99;// rho tasa de evaporacion
		double alpha=1.0;
		double beta=1.0;
		double Q = 1.0;
		double q0 = 0.7;
		double phi = 0.05;
		double feromona_inicial = 0.1;
		
		int num_hormigas = 4; 
		int nodo_inicial = 0; //nodoes de [ ]
		int num_iteraciones;

	private:
		mi D;//matriz de distancias D
		mi FL; //matriz de flujo
		md V;//matriz visibilidad n
		md F;//matriz feromonas T
		poblacion P;
		hormiga mejor_local;
		hormiga mejor_global;
	public:
		sis_hormigas(int iteraciones);
		void imprimir_D();
		void imprimir_V();
		void imprimir_F();
		bool visitado(hormiga H, int nodo);
		void run();
		int elegir_nodo(hormiga H);
		int intensificacion(hormiga H);
		int diversificacion(hormiga H);
		int get_nodo(vd probabilidades);
		void calcular_longitudes();
		void imprimir_camino(hormiga H);
		void actualizar_feromona_offline();
		void actualizar_feromona_online(hormiga H, int nodo);
		void imprimir_poblacion();
};


sis_hormigas::sis_hormigas(int iteraciones){
	num_iteraciones = iteraciones;
	mejor_local.longitud=inf;
	mejor_global.longitud=inf;
	D.resize(TAM_CROM);
	FL.resize(TAM_CROM);
	V.resize(TAM_CROM);
	F.resize(TAM_CROM);
	for (int i = 0; i < TAM_CROM; ++i){
		D[i].resize(TAM_CROM);
		FL[i].resize(TAM_CROM);
		V[i].resize(TAM_CROM);
		F[i].resize(TAM_CROM);
		for (int j = 0; j < TAM_CROM; ++j){ //recorrer matriz  ij
			D[i][j] = DISTANCIAS[i][j];
			FL[i][j] = FLUJO[i][j];
			if(i==j){
				V[i][j] = 0.0;
				F[i][j] = 0.0;
			}else{
				F[i][j] = feromona_inicial;
				//construccion de matriz de visibilidad
				double e=0;
				for (int k = 0; k < TAM_CROM; ++k){
					e += FLUJO[k][i] * DISTANCIAS[j][k];
				}
				V[i][j] = 0;
				if(e!=0){
					V[i][j] = 1.0/e;
				}
			}
		}
	}
}

void sis_hormigas::imprimir_D(){
	for(auto v:D){
		for (auto d: v){
			cout<<d<<" ";
		}
		cout<<endl;
	}
}

void sis_hormigas::imprimir_V(){
	cout<<endl;
	for(auto v:V){
		for (auto e: v){
			//cout<<d<<" ";
			printf("%.4f  ", e);
		}
		cout<<endl;
	}
}

void sis_hormigas::imprimir_F(){
	cout<<endl;
	for(auto f:F){
		for (auto e: f){
			//cout<<d<<" ";
			printf("%.4f  ", e);
		}
		cout<<endl;
	}
}

void sis_hormigas::run(){
	cout<<"nodo inicial "<<LABELS[nodo_inicial]<<endl;
	for (int i = 0; i < num_iteraciones; ++i){
		cout<<"\n_________ Iteracion "<<i<<" _________"<<endl;
		for (int j = 0; j < num_hormigas; ++j){
			cout<<"HOTMIGA: "<<i+1<<endl;
			hormiga H;
			H.camino.push_back(nodo_inicial);
			H.nodo_actual=nodo_inicial;
			for (int k = 0; k < TAM_CROM-1; ++k){
				int nuevo_nodo = elegir_nodo(H);
				H.camino.push_back(nuevo_nodo);	
				actualizar_feromona_online(H,nuevo_nodo);
				H.nodo_actual = nuevo_nodo;
			}
			imprimir_camino(H);
			P.push_back(H);
		}
		calcular_longitudes();//
		actualizar_feromona_offline();
		imprimir_poblacion();
		P.clear();
	}
}

int sis_hormigas::intensificacion(hormiga H){
	//argmax
	double arg_max= -inf;
	int nodo_elegido;
	for (int nodo = 0; nodo < TAM_CROM; ++nodo){
		if(!visitado(H,nodo)){
			double r = F[H.nodo_actual][nodo] * V[H.nodo_actual][nodo];
			if(r > arg_max){
				arg_max = r;
				nodo_elegido = nodo;
			} 
		}
	}
	return nodo_elegido;
}
int sis_hormigas::diversificacion(hormiga H){
	//sumatoria
	vd probabilidades(TAM_CROM);
	double suma = 0.0;
	vi no_visitados;
	for (int nodo = 0; nodo < TAM_CROM; ++nodo){
		if(!visitado(H,nodo)){
			suma += pow(F[H.nodo_actual][nodo], alpha) * pow(V[H.nodo_actual][nodo] ,beta);
			no_visitados.push_back(nodo);
		}
	}
	//nodos
	for (int i = 0; i < TAM_CROM; ++i){//para cada nodo no visitada
		if(!visitado(H,i)){	//calcular probabilidade de H.nodo_Actual a nodo i
			double a =pow(F[H.nodo_actual][i], alpha) * pow(V[H.nodo_actual][i] ,beta);
			probabilidades[i]= a/suma; //calculo
			cout<<LABELS[H.nodo_actual]<<"-"<<LABELS[i]<<": prob = "<< probabilidades[i] <<endl; //V[H.nodo_actual][i]<<endl;
		}else{
			probabilidades[i]=0;
		}
	}
	
	int nuevo_nodo = get_nodo(probabilidades); 
	if(nuevo_nodo < 0){
		nuevo_nodo = no_visitados[ rand()%no_visitados.size() ];
	}
	return nuevo_nodo;
}

int sis_hormigas::elegir_nodo(hormiga H){
	double q= getRandom(0.0,1.0);
	if( q<=q0 ){
		return intensificacion(H);
	}else{
		return diversificacion(H);
	}
}

int sis_hormigas::get_nodo(vd probabilidades){
	vd pro(probabilidades.size());
	//suma = 1	
	pro[0]=probabilidades[0];
	for (int i = 1; i < probabilidades.size(); ++i){
		pro[i] = pro[i-1] + probabilidades[i] ;
		//see(pro[i]);
	}
	//aleatorio
	double aleatorio = getRandom(0,1);
	see(aleatorio)
	for (int i = 0; i < pro.size(); ++i){
		if( aleatorio <= pro[i] ){
			cout<<"siguiente nodo "<< LABELS[i]<<"\n"<<endl;
			return i;
		}
	}
	return -1;
}

void sis_hormigas::calcular_longitudes(){
	for (hormiga &H : P){
		double s=0;
		for (int i = 0; i < TAM_CROM; ++i){
			for (int j = 0; j < TAM_CROM; ++j){
				if(i!=j){
					s+= FL[i][j] * D[ H.camino[i] ][H.camino[j] ];
				}
			}
		}
		H.longitud=s;
	}
	sort(P.begin(), P.end(), Mejor);
	mejor_local = P[0];
	//mejor global
	if(mejor_local.longitud < mejor_global.longitud){
		mejor_global = mejor_local;
	}
}

void sis_hormigas::imprimir_camino(hormiga H){
	cout<<"[ ";
	for (int nodo : H.camino){
		cout<<LABELS[nodo]<<" ";
	}
	cout<<"]"<<endl;
}

void sis_hormigas::actualizar_feromona_offline(){
	//evaporacion
	for (int i = 0; i < TAM_CROM; ++i){
		for (int j = 0; j < TAM_CROM; ++j){
			if(i!=j){
				F[i][j] = p_evaporacion * F[i][j];
				F[j][i] = F[i][j]; //espejo
			}	
		}
	}
	//depositar feromona
	for (int j = 1; j < mejor_global.camino.size(); ++j){
		int h = mejor_global.camino[j-1];
		int k = mejor_global.camino[j];
		
		F[h][k] += (1.0-p_evaporacion ) * 1/mejor_global.longitud;
		F[k][h] = F[h][k];//espejo
	}
}

void sis_hormigas::actualizar_feromona_online(hormiga H, int nodo){
	int i = H.nodo_actual;
	int j = nodo;
	F[i][j] = (1 - phi)*F[i][j] + phi*feromona_inicial;
	F[j][i] = F[i][j];//espejo
}

void sis_hormigas::imprimir_poblacion(){ 
	cout<<"Poblacion: "<<endl;
	int orm=1;

	for( hormiga &H: P){
		cout<<"Hormiga "<<orm<<": "; orm++;
		for (int i = 0; i < H.camino.size(); ++i){
			cout<<LABELS[H.camino[i]]<<" ";
		}
		cout<<" costo: "<<H.longitud;
		cout<<endl;
	}
	//
	cout<<"\nMejor Hormiga Local: ";
	for (int i = 0; i < mejor_local.camino.size(); ++i){
		cout<<LABELS[mejor_local.camino[i]]<<" ";
	}
	cout<<" costo: "<<mejor_local.longitud<<endl;
	//
	cout<<"Mejor Hormiga Global: ";
	for (int i = 0; i < mejor_global.camino.size(); ++i){
		cout<<LABELS[mejor_global.camino[i]]<<" ";
	}
	cout<<" costo: "<<mejor_global.longitud<<endl;
}



bool sis_hormigas::visitado(hormiga H, int nodo){
	for (int i = 0; i < H.camino.size() ; ++i){
		if( H.camino[i]==nodo){
			return true;
		}
	}
	return false;
}

int main(){
	int iteraciones=10;
	sis_hormigas SH(iteraciones);
	SH.imprimir_D();
	SH.imprimir_V();
	SH.imprimir_F();
	SH.run();
	return 0;
}