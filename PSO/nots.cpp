#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;
double Pi= 3.14159;
double inf = 10000000.00001;

typedef struct{
	vector<int> cro;
	double fitness;
}individuo;

typedef vector<individuo> poblacion;
typedef vector<double> vd;
typedef vector<int> vi;

int TAM_CROM = 10;
char labels_cities[]={'A','B','C','D','E','F','G','H','I','J'}; 
int cities[]={0,1,2,3,4,5,6,7,8,9}; 
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

int getRandom(int li, int ls){  return li + static_cast <int> (rand()) /( static_cast <int> (RAND_MAX/(ls-li)));}
//distancias
double fx(individuo I){  
	double s=0;
	for (int j = 0; j < I.cro.size()-1; ++j){
		int p1 = I.cro[j];
		int p2 = I.cro[j+1];
		s+=DISTANCES[p1][p2];
	}
	return s;
}
//costos
double gx(individuo I){ 
	double s=0;
	for (int j = 0; j < I.cro.size()-1; ++j){
		int p1 = I.cro[j];
		int p2 = I.cro[j+1];
		s+=COSTS[p1][p2];
	}
	return s;
}

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
#define PROB_MUT 5 // 0.5-1% [0-100]Probabilidad de Mutación: 0.05
#define PROB_CRU 80 //Probabilidad de Cruzamiento: 0.9

double get_fitness(individuo I){
	return w[0]*fx(I) + w[1]*gx(I);
}


bool es_valido(individuo &I){
	return true;
}


poblacion get_poblacion_inicial(int tam_poblacion){
	poblacion P;
	for (int i = 0; i < tam_poblacion; ++i){
		individuo I;
		for (int j = 0; j < TAM_CROM; ++j){
			I.cro.push_back(cities[j]);//0,1,..,9 //ciudades ordenadas
		}
		for (int j = 0; j < TAM_CROM; ++j){
			int n = rand()%5;
			int m = rand()%5;
			swap(I.cro[n],I.cro[m]);	
		}	
		I.fitness = get_fitness(I);
		P.push_back(I);
	}
	return P;
}

string p_cro(individuo I){
	string c="";
	for(auto i:I.cro){
		c+=labels_cities[i];
	}
	return c;
}

void imprimir_poblacion(poblacion P){
	for (int i = 0; i < P.size(); ++i){
		individuo I = P[i];
		//cout<<i+1<<") "<<" "<<fx(I)<<" "<<gx(I)<<"	"<<"	|"<<I.fitness<<endl;
		cout<<i+1<<") "<<p_cro(I)<<"	"<<fx(I)<<" "<<gx(I)<<"	"<<"	|"<<I.fitness<<endl;
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


int find_poss(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return i;
		}
	}
	return -1;
} 



poblacion PMX(individuo P1,individuo P2){
	vi p1 = P1.cro; 
	vi p2 = P2.cro;  

	int point1 = 3;
	int point2 = 6;
	for (int i = 0; i < TAM_CROM; ++i){
		if( point1<=i && i<point2){ //sublista
			int val_label_p1 = p1[i];  
			int val_label_p2 = p2[i];
			int dir1=find_poss(p1,val_label_p2);
			int dir2=find_poss(p2,val_label_p1);
			p1[dir1]=val_label_p1;
			p2[dir2]=val_label_p2;
			swap(p1[i],p2[i]); 
		}	
	}

	poblacion hijos;
	individuo h1;
	h1.cro = p1;
	h1.fitness = get_fitness(h1);

	individuo h2;
	h2.cro = p2;
	h2.fitness = get_fitness(h2);

	hijos.push_back(h1);
	hijos.push_back(h2);
	return hijos;
}

poblacion cruzamiento(poblacion &P){
	poblacion hijos;
	for (int i = 0; i < P.size()/2;){
		int h = rand()%P.size();
		int k = rand()%P.size();
		int prob = rand()%100;
		if( prob <PROB_CRU){
			poblacion r = PMX(P[h],P[k]);
			hijos.push_back(r[0]); //hijo1
			hijos.push_back(r[1]); //hijo2
			i++;
		}

	}
	return hijos;
}

void mutar(poblacion &P){
	for (int i = 0; i < P.size();++i){
		int pro_mut = rand()%100;
		if(pro_mut <= PROB_MUT ){
			cout<<"mutacion en individuo: "<< i+1<<endl;;
			int c1 = rand()%TAM_CROM;
			int c2 = rand()%TAM_CROM;	
			swap(P[i].cro[c1],P[i].cro[c2]);
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
	int iteraciones = 2;
	int tam_poblacion = 100;
	double D = 20.0;
	poblacion P = get_poblacion_inicial( tam_poblacion );
	for (int it = 1; it <= iteraciones; ++it){
		cout<<"__________________ Iteracion "<<it<<" __________________"<<endl;
		cout<<"poblacion"<<endl;
		imprimir_poblacion(P);	
		cout<<"seleccion"<<endl;
		poblacion seleccionados = seleccion(P);
		imprimir_poblacion(seleccionados);
		
		cout<<"____hijos_____"<<endl;
		poblacion hijos = cruzamiento(seleccionados);//hijos (1)	
		imprimir_poblacion(hijos);
		//agregar hijos a la poblacion
		for (auto I: hijos){//
			P.push_back(I);
		}
		see("______________________");
		imprimir_poblacion(P);
		cout<<"mutacion___________"<<endl;
		mutar(P);
		imprimir_poblacion(P);
		cout<<"Nueva Poblacion"<<endl;
		
		poblacion new_P;
		vector<poblacion> fronteras = get_fronteras(P);
		
		// Nueva Poblacion
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
	poblacion P = get_poblacion_inicial( 10 );
	imprimir_poblacion(P);
}


int main(){
	//test();
	run();
	return 0;
}