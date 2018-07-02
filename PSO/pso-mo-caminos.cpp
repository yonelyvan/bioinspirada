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

typedef particula particula;

typedef vector<particula> poblacion;


bool Mejor(particula a, particula b){
	if(a.fitness < b.fitness)//< minimizar | > maximizar
		return true;
	return false;
}


const int TAM_CROM = 5;
char labels_cities[]={'A','B','C','D','E','F','G','H','I','J'}; 
int cities[]={0,1,2,3,4,5,6,7,8,9}; 
double w[]={1,1};//wfx, wgx

double DISTANCES[10][10]={
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
double COSTS[10][10]={	
					{	0,	22,	47,	15,	63,	21,	23,	16,	11,	9},
					{	22,	0,	18,	62,	41,	52,	13,	11,	26,	43},
					{	47,	18,	0,	32,	57,	44,	62,	20,	8,	36},
					{	15,	62,	32,	0,	62,	45,	75,	63,	14,	12},
					{	63,	41,	57,	62,	0,	9,	99,	42,	56,	23},
					{	21,	52,	44,	45,	9,	0,	77,	58,	22,	14},
					{	23,	13,	62,	75,	99,	77,	0,	30,	25,	60},
					{	16,	11,	20,	63,	42,	58,	30,	0,	66,	85},
					{	11,	26,	8,	14,	56,	22,	25,	66,	0,	54},
					{	9,	43,	36,	12,	23,	14,	60,	85,	54,	0}
					};
//DECBA
/*
double DISTANCES[TAM_CROM][TAM_CROM]={
									{0,1,3,4,5},
									{1,0,1,4,8},
									{3,1,0,5,1},
									{4,4,5,0,2},
									{5,8,1,2,0}};
*/			

//distancias
double fx(particula I){  
	double s=0;
	for (int j = 0; j < I.cro.size()-1; ++j){
		int p1 = I.cro[j];
		int p2 = I.cro[j+1];
		s+=DISTANCES[p1][p2];
	}
	return s;
}
//costos
double gx(particula I){ 
	double s=0;
	for (int j = 0; j < I.cro.size()-1; ++j){
		int p1 = I.cro[j];
		int p2 = I.cro[j+1];
		s+=COSTS[p1][p2];
	}
	return s;
}

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


double get_fitness(particula I){
	//return fx(I);
	return w[0]*fx(I) + w[1]*gx(I);
}


poblacion get_poblacion(int tam_poblacion){
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

particula get_particula(){
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
	return I;
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
			return false;//true;
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
void calcular_velocidades(poblacion &P, poblacion repo_g){
	for (int i = 0; i < P.size(); ++i){
		particula mejor_l = P[ rand()%P.size() ]; //mejor local
		particula mejor_g = repo_g[ rand()%repo_g.size() ]; //mejor global

		vvi ml = restar(mejor_l,P[i]);
		vvi mg = restar(mejor_g,P[i]);
		//cout<<i<<") pbest-x(t-1) = [ "<<print_velocidad(ml)<<"]"<<endl;
		//cout<<" pbest-x(t-1) + gbest - x(t-1) = [ "<<print_velocidad(mg)<<"]"<<endl;
		P[i].velocidad = unir(ml,mg); //unir(unir(P[i].velocidad, ml ), mg); //
		//cout<<" [ "<<print_velocidad(P[i].velocidad)<<"]"<<endl;

		P[i] = swaps(P[i] , P[i].velocidad);
		P[i].fitness = get_fitness(P[i]);
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



vector<poblacion> get_poblacion_frontera(poblacion P){
	double D = 8.0;
	int tam_poblacion = P.size();
	vector<poblacion> fronteras = get_frontier(P);
	poblacion F = fronteras[0];
	poblacion P_resto = fronteras[1];
	poblacion P_frontera;
	for(int i=0; i<F.size(); i++){//frontera ordenada cresiente
		if(i==0 || i==F.size()-1){
			//cout<<i+1<<") "<<fx(F[i])<<" "<<gx(F[i])<<"	"<<"	|"<<F[i].fitness<<endl;
			P_frontera.push_back(F[i]);
		}else{
			if( distancia(F[i-1], F[i+1]) > D){//distancia
				//cout<<i+1<<") "<<fx(F[i])<<" "<<gx(F[i])<<"	"<<"	|"<<F[i].fitness<<endl;
				P_frontera.push_back(F[i]);
			}
		}
	}
	vector<poblacion> R;
	R.push_back( P_frontera );
	R.push_back( P_resto );
	return R;
}
//===============================================

void imprimir_soluciones_no_dominales(poblacion P){
	cout<<"\nsoluciones_no_dominales"<<endl;
	int i =0;
	for( auto p:P){
		cout<<i+1<<") "<<p_cro(p)<<"	"<<fx(p)<<" "<<gx(p)<<"	"<<"	|"<<p.fitness<<endl;
		i++;
	}
}


void run(){
	int N =20;
	int num_it=10;
	poblacion P;
	poblacion repo_g;
	poblacion repo_per;
	particula mejor_g;
	particula mejor_l;
	
	P = get_poblacion(N);
	vector<poblacion> r=get_poblacion_frontera(P);
	repo_g = r[0]; //actualizar repositorio global
	repo_per = r[1]; //actualizar repositorio personal
	imprimir_poblacion(P);

	int it =0;
	while (it<num_it){
		calcular_velocidades(P, repo_g);

		vector<poblacion> r=get_poblacion_frontera(P);
		repo_g = r[0]; //actualizar repositorio global
		repo_per = r[1]; //actualizar repositorio personal
		
		cout<<"________________"<<it<<"________________"<<endl;
		imprimir_poblacion(P);
		imprimir_soluciones_no_dominales(repo_g);
		it++;
	}
}



int main(){
	run();	
	return 0;
}




