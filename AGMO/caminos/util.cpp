#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;

using namespace std;

typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<vi> vvi;
typedef map<int,int> mii;


int inf = 9999;

//10X10
double DISTANCIAS[10][10]={
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
double COSTOS[10][10]={	
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


int CAMINO[] = {0,1,2,3,4,5,6,7,8,9};//
char labels[] = {'A','B','C','D','E','F','G','H','I','J'};//
float w[]={1,1};

vf get_fitness(vvi p){
	vf f;
	for (int i = 0; i < p.size(); ++i){
		float s_fx =0;
		float s_gx =0;
		for (int j = 0; j < p[0].size()-1; ++j){
			int p1 = p[i][j];
			int p2 = p[i][j+1];
			s_fx+= DISTANCIAS[p1][p2];
			s_gx+= COSTOS[p1][p2];
		}
		f.push_back( w[0]*s_fx + w[1]*s_gx );
	}
	return f;
}

float get_fitness(vi I){
	float s=0;
	for (int j = 0; j < I.size()-1; ++j){
		int p1 = I[j];
		int p2 = I[j+1];
		s+=DISTANCIAS[p1][p2];
	}
	return s;
}

/****************/

void print_vi(vi v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
		//cout<<labels[v[i] ]<<" ";
	}
	cout<<endl;
}

void print_vi_labels(vi v){
	for (int i = 0; i < v.size(); ++i){
		//cout<<v[i]<<" ";
		cout<<labels[v[i] ]<<" ";
	}
	cout<<endl;
}


void print_vf(vf v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
	}
	cout<<endl;
}

void print_vvi(vvi v){
	for (int i = 0; i < v.size(); ++i){
		print_vi(v[i]);
	}
}

void print_vvi_labels(vvi v){
	for (int i = 0; i < v.size(); ++i){
		print_vi_labels(v[i]);
	}
}

int suma_vf(vf v){
	int s=0;
	for (int i = 0; i < v.size(); ++i){
		s+=v[i];
	}
	return s;
}

int suma_vi(vi v){
	int s=0;
	for (int i = 0; i < v.size(); ++i){
		s+=v[i];
	}
	return s;
}

void imprimir_poblacion(vvi p){
	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		print_vi_labels(p[i]);
	}
}


void imprimir_fit_poblacion(vvi p){
	vf vfitness = get_fitness(p);
	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		for (int j = 0; j < p[0].size(); ++j){
			//cout<<p[i][j]<<" ";
			cout<<labels[p[i][j]]<<" ";
		}
		cout<<" - "<<vfitness[i]<<endl;;
	}
}


void imprimir_fit_ruleta_poblacion(vvi p){
	vf f = get_fitness(p);
	int total = suma_vf(f);
	vf v_pro;//ruleta
	for (int i = 0; i < f.size(); ++i){
		v_pro.push_back((f[i]*100.0)/total); 
	}

	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		for (int j = 0; j < p[0].size(); ++j){
			//cout<<p[i][j]<<" ";
			cout<<labels[p[i][j]]<<" ";

		}
		cout<<" - "<<f[i]<<" - "<<v_pro[i]<<endl;;
	}
}


bool find(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return true;
		}
	}
	return false;
} 

int find_poss(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return i;
		}
	}
	return -1;
} 

