#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;
//double Pi= 3.14159;
double inf = 10000000.00001;

const int TAM_CROM = 4;//5;//10;
char LABELS[]={'A','B','C','D','E','F','G','H','I','J'}; 

double DISTANCIAS[TAM_CROM][TAM_CROM]={
					{	0,	12,	6,	4},
					{	12,	0,	6,	8},
					{	6,	6,	0,	7},
					{	4,	8,	7,	0}
					};

double FLUJO[TAM_CROM][TAM_CROM]={
					{	0,	3,	8,	3},
					{	3,	0,	2,	4},
					{	8,	2,	0,	5},
					{	3,	4,	5,	0}
					};


typedef vector<int> vi;
typedef vector<double> vd;
typedef vector< vector<int> > mi;
typedef vector< vector<double> > md;
typedef struct{
	vi camino;
	int ciudad_actual;
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
	private:
		const double alpha=1.0;
		const double beta=1.0;
		int num_hormigas;
		double p_evaporacion=0.99;//tasa de evaporacion
		double Q=1.0;
		int num_iteraciones = 1;

		double F_min=0.1;
		double F_max=0.9;

	private:
		mi D;//matriz de distancias D
		mi FL; //matriz de flujo
		md V;//matriz visibilidad n
		md F;//matriz feromonas T
		poblacion P;
		hormiga mejor_local;
	public:
		sis_hormigas(int n_hormigas);
		void imprimir_D();
		void imprimir_V();
		void imprimir_F();
		bool visitado(hormiga H, int ciudad);
		void run(int ciudad_inicial);
		int elegir_ciudad(hormiga H);
		int get_ciudad(vd probabilidades);
		void calcular_longitudes();
		void imprimir_camino(hormiga H);
		void evaporar_feromona();
		void depositar_feromona();
		void imprimir_poblacion();
};


sis_hormigas::sis_hormigas(int n_hormigas){
	num_hormigas = n_hormigas;
	mejor_local.longitud=inf;
	D.resize(TAM_CROM);
	FL.resize(TAM_CROM);
	V.resize(TAM_CROM);
	F.resize(TAM_CROM);
	for (int i = 0; i < TAM_CROM; ++i){
		D[i].resize(TAM_CROM);
		FL[i].resize(TAM_CROM);
		V[i].resize(TAM_CROM);
		F[i].resize(TAM_CROM);
		for (int j = 0; j < TAM_CROM; ++j){
			D[i][j] = DISTANCIAS[i][j];
			FL[i][j] = FLUJO[i][j];
			if(i==j){
				V[i][j] = 0.0;
				F[i][j] = 0.0;
			}else{
				V[i][j] = 1.0/D[i][j];
				F[i][j] = F_min;
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

void sis_hormigas::run(int ciudad_inicial){
	cout<<"ciudad inicial "<<LABELS[ciudad_inicial]<<endl;
	for (int i = 0; i < num_iteraciones; ++i){
		for (int j = 0; j < num_hormigas; ++j){
			cout<<"HOTMIGA: "<<i+1<<endl;
			hormiga H;
			H.camino.push_back(ciudad_inicial);
			H.ciudad_actual=ciudad_inicial;
			for (int k = 0; k < TAM_CROM-1; ++k){
				int nueva_ciudad = elegir_ciudad(H);
				H.camino.push_back(nueva_ciudad);	
				H.ciudad_actual = nueva_ciudad;
			}
			imprimir_camino(H);
			P.push_back(H);
		}
		calcular_longitudes();//
		evaporar_feromona();
		depositar_feromona();
		imprimir_poblacion();
		P.clear();
	}
}

bool sis_hormigas::visitado(hormiga H, int ciudad){
	for (int i = 0; i < H.camino.size() ; ++i){
		if( H.camino[i]==ciudad){
			return true;
		}
	}
	return false;
}


int sis_hormigas::elegir_ciudad(hormiga H){
	//sumatoria
	vd probabilidades(TAM_CROM);
	double suma = 0.0;
	for (int city = 0; city < TAM_CROM; ++city){
		if(!visitado(H,city)){
			suma += pow(F[H.ciudad_actual][city], alpha) * pow(V[H.ciudad_actual][city] ,beta);
		}
	}
	//ciudades
	for (int i = 0; i < TAM_CROM; ++i){
		if(!visitado(H,i)){//calcular probabilidade de H.ciudad_Actual a ciudad i
			double a =pow(F[H.ciudad_actual][i], alpha) * pow(V[H.ciudad_actual][i] ,beta);
			probabilidades[i]=a/suma;
			cout<<LABELS[H.ciudad_actual]<<"-"<<LABELS[i]<<": prob = "<< probabilidades[i] <<endl; //V[H.ciudad_actual][i]<<endl;
		}else{
			probabilidades[i]=0;
		}
	}
	return get_ciudad(probabilidades);
}

int sis_hormigas::get_ciudad(vd probabilidades){
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
			cout<<"siguiente ciudad "<< LABELS[i]<<"\n"<<endl;
			return i;
		}
	}
}

void sis_hormigas::calcular_longitudes(){
	for (hormiga &H : P){
		double s = 0;
		for (int i = 1; i < H.camino.size(); ++i){
			int h = H.camino[i-1];
			int k = H.camino[i];
			s+= D[h][k] * FL[h][k];
		}
		H.longitud=s;
	}
	sort(P.begin(), P.end(), Mejor);
	mejor_local = P[0];
}

void sis_hormigas::imprimir_camino(hormiga H){
	cout<<"[ ";
	for (int ciudad : H.camino){
		cout<<LABELS[ciudad]<<" ";
	}
	cout<<"]"<<endl;
}

void sis_hormigas::evaporar_feromona(){
	for (int i = 0; i < TAM_CROM; ++i){
		for (int j = 0; j < TAM_CROM; ++j){
			if(i!=j){
				F[i][j] = (1 - p_evaporacion) * F[i][j];
				if(F[i][j] > F_max){ F[i][j] = F_max; }
				if(F[i][j] < F_min){ F[i][j] = F_min; }
			}	
		}
	}
}

void sis_hormigas::depositar_feromona(){
	for (int j = 1; j < mejor_local.camino.size(); ++j){
		int h = mejor_local.camino[j-1];
		int k = mejor_local.camino[j];
		F[h][k] += 1/mejor_local.longitud;
		if(F[h][k] > F_max){ F[h][k] = F_max; }
		if(F[h][k] < F_min){ F[h][k] = F_min; }
	}
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

	cout<<"\nMejor Hormiga Local: ";
	for (int i = 0; i < mejor_local.camino.size(); ++i){
		cout<<LABELS[mejor_local.camino[i]]<<" ";
	}
	cout<<" costo: "<<mejor_local.longitud<<endl;
}


void run(){
	int ciudad_inicial = 3; //ciudades de 0-9
	int num_hormigas = 3; 
	sis_hormigas SH(num_hormigas);
	SH.imprimir_D();
	SH.imprimir_V();
	SH.imprimir_F();
	SH.run(ciudad_inicial);
}


int main(){
	run();
	
	return 0;
}