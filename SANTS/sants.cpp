#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;
//double Pi= 3.14159;
//double inf = 10000000.00001;

const int TAM_CROM = 10;
char LABELS[]={'A','B','C','D','E','F','G','H','I','J'}; 
double DISTANCIAS[TAM_CROM][TAM_CROM]={
					{	0,	12,	3,	23,	1,	5,	23,	56,	12,	11},
					{	12,	0,	9,	18,	3,	41,	45,	5,	41,	27},
					{	3,	9,	0,	89,	56,	21,	12,	48,	14,	29},
					{	23,	18,	89,	0,	87,	46,	75,	17,	50,	42},
					{	1,	3,	56,	87,	0,	55,	22,	86,	14,	33},
					{	5,	41,	21,	46,	55,	0,	21,	76,	54,	81},
					{	23,	45,	12,	75,	22,	21,	0,	11,	57,	48},
					{	56,	5,	48,	17,	86,	76,	11,	0,	63,	24},
					{	12,	41,	14,	50,	14,	54,	57,	63,	0,	9},
					{	11,	27,	29,	42,	33,	81,	48,	24,	9,	0}
					};


typedef vector<int> vi;
typedef vector<double> vd;
typedef vector< vector<int> > mi;
typedef vector< vector<double> > md;
typedef struct{
	vi camino;
	int ciudad_actual;
	double fitness;
}hormiga;

typedef vector<hormiga> poblacion;

class sis_hormigas{
	private:
		const double alpha=1.0;
		const double beta=1.0;

	private:
		mi D;//matriz de distancias
		md V;//matriz visibilidad
		md F;//matriz feromonas
		poblacion P;
	public:
		sis_hormigas(int n_ants);
		void imprimir_D();
		void imprimir_V();
		void imprimir_F();
		bool visitado(hormiga H, int ciudad);
		double get_prob(hormiga H, int ciudad);
		void recorrer(int ciudad_inicial);
		int elegir_ciudad(hormiga H);


};


sis_hormigas::sis_hormigas(int n_ants){
	D.resize(TAM_CROM);
	V.resize(TAM_CROM);
	F.resize(TAM_CROM);
	for (int i = 0; i < TAM_CROM; ++i){
		D[i].resize(TAM_CROM);
		V[i].resize(TAM_CROM);
		F[i].resize(TAM_CROM);
		for (int j = 0; j < TAM_CROM; ++j){
			D[i][j] = DISTANCIAS[i][j];
			if(i==j){
				V[i][j] = 0.0;
				F[i][j] = 0.0;
			}else{
				V[i][j] = 1.0/D[i][j];
				//F[i][j] = 1.0/D[i][j];
				F[i][j] = 0.1;
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
	for(auto v:V){
		for (auto e: v){
			//cout<<d<<" ";
			printf("%.3f  ", e);
		}
		cout<<endl;
	}
}

void sis_hormigas::imprimir_F(){
	for(auto f:F){
		for (auto e: f){
			//cout<<d<<" ";
			printf("%.2f  ", e);
		}
		cout<<endl;
	}
}

void sis_hormigas::recorrer(int ciudad_inicial){
	cout<<"ciudad_inicial "<<LABELS[ciudad_inicial]<<endl;

	hormiga H;
	H.camino.push_back(ciudad_inicial);
	H.ciudad_actual=ciudad_inicial;
	//para cada ciudad no visitada calculamos sus probabilidades 
	elegir_ciudad(H);
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


	for (int i = 0; i < TAM_CROM; ++i){
		if(!visitado(H,i)){//calcular probabilidade de H.ciudad_Actual a ciudad i
			cout<<LABELS[H.ciudad_actual]<<"-"<<LABELS[i]<<": prob= "<<get_prob(H,i)<<endl; //V[H.ciudad_actual][i]<<endl;

		}
	}

	double suma = 0.0;
	for (int city = 0; city < TAM_CROM; ++city){
		if(!visitado(H,city)){
			suma += pow(F[H.ciudad_actual][city], alpha) * pow(V[H.ciudad_actual][city] ,beta);
		}
	}
	cout<<"____________________________suma: "<<suma<<endl;
	

}

double sis_hormigas::get_prob(hormiga H, int ciudad){
	if(H.ciudad_actual == ciudad){
		return 0;
	}
	double a = pow(F[H.ciudad_actual][ciudad], alpha) * pow(V[H.ciudad_actual][ciudad] ,beta);
	double b = 0.0;
	for (int city = 0; city < TAM_CROM; ++city){
		if(!visitado(H,city)){
			b += pow(F[H.ciudad_actual][city], alpha) * pow(V[H.ciudad_actual][city] ,beta);
		}
	}
	//cout<<"____________________________suma: "<<b<<endl;
	double r = a;///b;
	return r;
}

void run(){
	sis_hormigas SH(2);
	SH.imprimir_D();
	SH.imprimir_V();
	cout<<endl;
	SH.imprimir_F();
	int ciudad_inicial = 0; //ciudades de 0-9
	SH.recorrer(3);

}


int main(){
	run();
	return 0;
}