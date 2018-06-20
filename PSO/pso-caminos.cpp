#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;

typedef vector<double> vd;
typedef vector<int> vi;
typedef vector<vi> vvi;

typedef struct{
	vi cro;
	vvi velocidad; 
	double fitness;
}particula;

typedef vector<particula> poblacion;


bool Mejor(particula a, particula b){
	if(a.fitness < b.fitness)//< minimizar | > maximizar
		return true;
	return false;
}


const int TAM_CROM = 5;
char labels_cities[]={'A','B','C','D','E','F','G','H','I','J'}; 
int cities[]={0,1,2,3,4,5,6,7,8,9}; 

/*
double DISTANCES[TAM_CROM][TAM_CROM]={
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
*/

double DISTANCES[TAM_CROM][TAM_CROM]={
									{0,1,3,4,5},
									{1,0,1,4,8},
									{3,1,0,5,1},
									{4,4,5,0,2},
									{5,8,1,2,0}};
					

//distancia
double fx(particula I){  
	double s=0;
	for (int j = 0; j < I.cro.size()-1; ++j){
		int p1 = I.cro[j];
		int p2 = I.cro[j+1];
		s+=DISTANCES[p1][p2];
	}
	return s;
}

double get_fitness(particula I){
	return fx(I);
	//return w[0]*fx(I) + w[1]*gx(I);
}

poblacion get_poblacion_inicial(int tam_poblacion){
	poblacion P;
	for (int i = 0; i < tam_poblacion; ++i){
		particula I;
		for (int j = 0; j < TAM_CROM; ++j){
			I.cro.push_back(cities[j]);//0,1,..,9 //ciudades ordenadas
		}
		for (int j = 0; j < TAM_CROM; ++j){
			int n = rand()%TAM_CROM;
			int m = rand()%TAM_CROM;
			swap(I.cro[n],I.cro[m]);	
		}	
		I.fitness = get_fitness(I);
		P.push_back(I);
	}
	return P;
}

string p_cro(particula I){
	string c="";
	for(auto i:I.cro){
		c+=labels_cities[i];
	}
	return c;
}

void imprimir_poblacion(poblacion P){
	for (int i = 0; i < P.size(); ++i){
		particula I = P[i];
		cout<<i+1<<") "<<p_cro(I)<<"	|Costo Actual: "<<I.fitness<<endl;
		//cout<<i+1<<") "<<p_cro(I)<<"	"<<fx(I)<<" "<<gx(I)<<"	"<<"	|"<<I.fitness<<endl;
	}
}

particula mejor(poblacion P){
	sort(P.begin(), P.end(), Mejor);
	return P[0];
}

bool particulas_iguales(particula a, particula b){
	for (int i = 0; i < a.cro.size(); ++i){
		if( a.cro[i] != b.cro[i] ){
			return false;
		}
	}
	return true;
}

int get_position(particula a, int val){
	for (int i = 0; i < a.cro.size(); ++i){
		if(a.cro[i]==val){
			return i;
		}
	}
	return -1;
}

vvi restar(particula a, particula b){
	vvi ss;
	if(particulas_iguales(a,b)){
		return ss;	
	}
	for (int i = 0; i < a.cro.size();++i){
		if(a.cro[i] != b.cro[i]){
			int index = get_position(b,a.cro[i]);//buscar en b
			swap( b.cro[i],b.cro[index] );
			//guardar swap
			vi so;
			so.push_back(i);
			so.push_back(index);
			so.push_back(1);//probabilidad phi
			ss.push_back(so);
			//cout<<"SO "<<i<<" "<<index<<endl;
		}
		//cout<<"a : "<<p_cro(a)<<endl;	
		//cout<<"b : "<<p_cro(b)<<"\n"<<endl;
	}
	return ss;
}

bool repetido(vvi V,vi a){
	for (vi t:V){
		if( t[0]==a[0] && t[1]==a[1]){
			return true;
		}
	}
	return false;
}

vvi unir(vvi a, vvi b){
	for (vi bi : b){
		if( !repetido(a,bi)){
			a.push_back(bi);
		}
	}
	return a;
}

particula swaps(particula a, vvi ss){
	for (vi so:ss){
		swap(a.cro[so[0]], a.cro[so[1]]); //sin prob
	}
	return a;
}

string print_velocidad(vvi A){
	string r = "";
	for(vi a:A){
		r+="(";
		r+= to_string(a[0]);
		r+=",";
		r+= to_string(a[1]);
		r+=",";
		r+= to_string(a[2]);
		r+=") ";
	}
	return r;
}

//La velocidad de una partícula es una secuencia de intercambio de posiciones (swap).
void calcular_velocidades(poblacion &P, particula mejor_l, particula mejor_g){
	for (int i = 0; i < P.size(); ++i){
		// w*P[i].vx
		vvi ml = restar(mejor_l,P[i]);
		vvi mg = restar(mejor_g,P[i]);
		cout<<i<<") pbest-x(t-1) = [ "<<print_velocidad(ml)<<"]"<<endl;
		cout<<" pbest-x(t-1) + gbest - x(t-1) = [ "<<print_velocidad(mg)<<"]"<<endl;
		vvi vml= unir(P[i].velocidad,ml );
		P[i].velocidad =  unir(vml, mg );//unir y eliminar repetidos
		cout<<" [ "<<print_velocidad(P[i].velocidad)<<"]"<<endl;

		P[i] = swaps(P[i] , P[i].velocidad);
		P[i].fitness = get_fitness(P[i]);
	}
}


void run(){
	int tan_pob=8;
	int num_it = 3;
	poblacion P, Ppri;
	P = get_poblacion_inicial(tan_pob);
	imprimir_poblacion(P);
	particula mejor_g;
	particula mejor_l;
	mejor_g = mejor(P);
	mejor_l = mejor_g;
	for (int it = 0; it < num_it; ++it){
		cout<<"________________"<<it<<"________________"<<endl;
		cout<<"Mejor : "<<p_cro(mejor_g)<<"	fitness= "<<mejor_g.fitness<<endl;
		cout<<"Cúmulo de Particulas Siguietes"<<endl;
		restar(P[0],P[1]);
		calcular_velocidades(P,mejor_l, mejor_g);
		imprimir_poblacion(P);
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