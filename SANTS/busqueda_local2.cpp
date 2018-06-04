#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;
//double Pi= 3.14159;
double inf = 10000000.00001;

const int NUM_NODOS = 13;//5;//10;
char LABELS[]={'A','B','C','D','E','F','G','H','I','J','K','L','M'};

/*
double DISTANCIAS[NUM_NODOS][NUM_NODOS]={
					{	0,	12,	3,	23,	1},
					{	12,	0,	9,	18,	3},
					{	3,	9,	0,	89,	56},
					{	23,	18,	89,	0,	87},
					{	1,	3,	56,	87,	0}
					};
*/


double DISTANCIAS[NUM_NODOS][NUM_NODOS]={
{0,12,3,23,1,5,23,56,12,11,89,97,52},
{12,0,9,18,3,41,45,5,41,27,16,76,8},
{3,9,0,89,56,21,12,48,14,29,5,91,8},
{23,18,89,0,87,46,75,17,50,42,100,70,15},
{1,3,56,87,0,55,22,86,14,33,31,84,21},
{5,41,21,46,55,0,21,76,54,81,92,37,22},
{23,45,12,75,22,21,0,11,57,48,39,59,22},
{56,5,48,17,86,76,11,0,63,24,55,58,98},
{12,41,14,50,14,54,57,63,0,9,44,18,52},
{11,27,29,42,33,81,48,24,9,0,64,65,82},
{89,16,5,100,31,92,39,55,44,64,0,9,70},
{97,76,91,70,84,37,59,58,18,65,9,0,50},
{52,56,8,15,21,22,22,98,52,82,70,50,0}
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
		md V;//matriz visibilidad n
		md F;//matriz feromonas T
		poblacion P;
		hormiga mejor_local;
		hormiga mejor_global;
		hormiga peor_local;
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
		double get_longitud(hormiga H);
		void busqueda_local(hormiga &H);
};


sis_hormigas::sis_hormigas(int iteraciones){
	num_iteraciones = iteraciones;
	mejor_local.longitud=inf;
	mejor_global.longitud=inf;
	D.resize(NUM_NODOS);
	V.resize(NUM_NODOS);
	F.resize(NUM_NODOS);
	for (int i = 0; i < NUM_NODOS; ++i){
		D[i].resize(NUM_NODOS);
		V[i].resize(NUM_NODOS);
		F[i].resize(NUM_NODOS);
		for (int j = 0; j < NUM_NODOS; ++j){ //recorrer matriz  ij
			D[i][j] = DISTANCIAS[i][j];
			if(i==j){
				V[i][j] = 0.0;
				F[i][j] = 0.0;
			}else{
				V[i][j] = 1.0/D[i][j];
				F[i][j] = feromona_inicial;
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
			cout<<"------HORMIGA: "<<j+1<<"------"<<endl;
			hormiga H;
			H.camino.push_back(nodo_inicial);
			H.nodo_actual=nodo_inicial;
			for (int k = 0; k < NUM_NODOS-1; ++k){
				int nuevo_nodo = elegir_nodo(H);
				H.camino.push_back(nuevo_nodo);	
				actualizar_feromona_online(H,nuevo_nodo);//ONLINE
				H.nodo_actual = nuevo_nodo;
			}
			imprimir_camino(H);
			busqueda_local(H);//busqueda local
			P.push_back(H);
		}
		calcular_longitudes();//
		actualizar_feromona_offline();//OFFLINE
		imprimir_poblacion();
		P.clear();
	}
}

int sis_hormigas::intensificacion(hormiga H){
	//argmax
	cout<<"intensificacion"<<endl;
	double arg_max= -inf;
	int nodo_elegido;
	for (int nodo = 0; nodo < NUM_NODOS; ++nodo){
		if(!visitado(H,nodo)){
			double r = F[H.nodo_actual][nodo] * V[H.nodo_actual][nodo];
			if(r > arg_max){
				arg_max = r;
				nodo_elegido = nodo;
			} 
		}
	}
	cout<<"siguiente nodo "<< LABELS[nodo_elegido]<<"\n"<<endl;
	return nodo_elegido;
}
int sis_hormigas::diversificacion(hormiga H){
	//sumatoria
	cout<<"diversificacion"<<endl;
	vd probabilidades(NUM_NODOS);
	double suma = 0.0;
	vi no_visitados;
	for (int nodo = 0; nodo < NUM_NODOS; ++nodo){
		if(!visitado(H,nodo)){
			suma += pow(F[H.nodo_actual][nodo], alpha) * pow(V[H.nodo_actual][nodo] ,beta);
			no_visitados.push_back(nodo);
		}
	}
	//nodos
	for (int i = 0; i < NUM_NODOS; ++i){//para cada nodo no visitada
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
	cout<<"siguiente nodo "<< LABELS[nuevo_nodo]<<"\n"<<endl;
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
		double s = 0;
		for (int i = 1; i < H.camino.size(); ++i){
			s+= D[ H.camino[i-1] ][ H.camino[i] ];
		}
		H.longitud=s;
	}
	sort(P.begin(), P.end(), Mejor);
	mejor_local = P[0];
	peor_local = P.back();
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
	for (int i = 0; i < NUM_NODOS; ++i){
		for (int j = 0; j < NUM_NODOS; ++j){
			if(i!=j){
				F[i][j] = p_evaporacion * F[i][j];
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

double sis_hormigas::get_longitud(hormiga H){
	float l=0.0;
	for (int i = 1; i < H.camino.size(); ++i){
		int h = H.camino[i-1];
		int k = H.camino[i];
		l += D[h][k];
	}
	return l;
}

void sis_hormigas::busqueda_local(hormiga &HRM){
	int num_sucesores = 10;
	//generar sucesores
	bool flag=true;
	bool se_mejorado = false;
	hormiga H = HRM;
	do{	
		flag = false;
		vector <hormiga> sucesores;
		for (int i = 0; i < num_sucesores; ++i){
			int h = rand()%NUM_NODOS;
			int k = rand()%NUM_NODOS;
			
			vi camino_h = H.camino;
			swap(camino_h[h],camino_h[k]);
			
			hormiga hs;
			hs.nodo_actual = H.nodo_actual;
			hs.camino = camino_h;
			hs.longitud = get_longitud(hs);
			sucesores.push_back(hs);
		}
		//busqueda del mejor sucesor
		cout<<"busqueda_local"<<endl;
		for (hormiga hrm : sucesores){
			if( hrm.longitud < H.longitud){
				flag = true;
				se_mejorado = true;
				H = hrm;
				break;
			}
		}
	}
	while(flag);
	//cout<<"busqueda_local exito_____________________"<<endl;
	//actualizar feromonas
	if(se_mejorado){
		P.push_back(H);
		for (int j = 1; j < H.camino.size(); ++j){
			int h = H.camino[j-1];
			int k = H.camino[j];
			F[h][k] += Q/H.longitud;//actualizacion
		}
	}
}



int main(){
	int iteraciones=30;
	sis_hormigas SH(iteraciones);
	SH.imprimir_D();
	SH.imprimir_V();
	SH.imprimir_F();
	SH.run();
	return 0;
}