#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;


typedef struct{
	double val[2];
	double fEval;
	double fit;
	int cont;
	double prob;
}Sol;
typedef vector<Sol> poblacion;

bool compare(Sol a, Sol b){
	if(a.fit < b.fit){
		return false; 
	} 
	return true;
}


double genRandom(double li, double ls){
	return li + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(ls-li)));
}

void printSol(string s,Sol a){
	cout<<s;
	cout<<a.val[0]<<"  "<<a.val[1]<<"  "<<a.fEval<<"  "<<a.fit<<" "<<a.cont<<endl;
}

void printSolProb(string s,Sol a){
	cout<<s;
	cout<<a.val[0]<<"  "<<a.val[1]<<"  "<<a.fEval<<"  "<<a.fit<<"  Prob: "<< a.prob<<" cont: "<<a.cont<<endl;
}

void printpoblacionProb(string s,poblacion P){
	cout<<s;
	for(int i=0;i<P.size();i++){
		printSolProb(to_string(i)+")   ",P[i]);
	}
}

void printpoblacion(string s,poblacion E){
	cout<<s;
	for(int i=0;i<E.size();i++){
		printSol(to_string(i)+")   ",E[i]);
	}
}

void calcfEvalSol(Sol &a){
	 a.fEval=pow(a.val[0],2)+pow(a.val[1],2);
}

void calcFit(Sol &a){
	if(a.fEval>=0)
		a.fit=1/(1+a.fEval);
	else
		a.fit=1+abs(a.fEval);
}

void evaluar_poblacion(poblacion &P){
	for(auto &s : P){
		calcfEvalSol(s);
		calcFit(s);
	}
}


poblacion get_poblacion_inicial(int CS){
	poblacion P;
	for(int i=0;i<CS;i++){
		Sol s;
		s.val[0]= genRandom(-5,5);
		s.val[1]= genRandom(-5,5);
		s.fEval=0;
		s.fit=0;
		s.cont=0;
		P.push_back(s);
	} 
	return P;
}


poblacion genCandidatas(poblacion P,int CS, int D, int SN){
	poblacion cand;
	for(int i=0;i<CS;i++){
		double fi=genRandom(-1,1);//rand
		int j = round((genRandom(0,D-1)));
		int k = round(genRandom(0,SN-1));
		while(k==i)
			 k = round(genRandom(0,SN-1));
		if(k!=i){
			Sol s;
			s.val[(j+1)%2] = P[i].val[(j+1)%2];
			s.val[j]       = P[i].val[j]+ fi * ( P[i].val[j] - P[k].val[j] );
			s.fEval=0;
			s.fit=0;
			s.cont=0;
			cand.push_back(s);
		}
	}
	return cand;
}

void genNuevaObs(poblacion & P, int pos,int D, int SN){
	// vi,j = xi,j + fi · (xi,j − xk,j)
	cout<<"\n>>> pos elegida: "<<pos<<endl;
	double fi=genRandom(-1,1);
	int j = round(genRandom(0,D-1));
	int k = round(genRandom(0,SN-1));
	while(k==pos)
		k = round(genRandom(0,SN-1));
	if(k!=pos){
		Sol s;
		s.val[(j+1)%2] = P[pos].val[(j+1)%2];
		s.val[j]       = P[pos].val[j]+ fi * (P[pos].val[j]-P[k].val[j]);
		s.fEval=0;
		s.fit=0;
		s.cont=0;
		calcfEvalSol(s);
		calcFit(s);
		
		if(s.fit < P[pos].fit ){
			s.cont++; 
			P[pos].cont++;	
		}
		else{
			s.cont=0; P[pos].cont=0;	
			P[pos]=s;
		}
		printSol("Imprimiendo primera opcion \n",s);
		printpoblacion("\nNuevos Mejores \n", P);
	}
}

void comppoblacion(poblacion & P, poblacion & cand){
	for(int i=0;i<P.size();i++){
		if( cand[i].fit <= P[i].fit ){
			cand[i].cont++;
		}
	}
}
poblacion bestSols(poblacion &P , poblacion &cand){
	poblacion B;
	for(int i=0;i<P.size();i++){
		if(cand[i].fit < P[i].fit){
			P[i].cont++;
			B.push_back(P[i]);
		}
		else
			B.push_back(cand[i]);	
	}
	return B;
}

int probSeleccion(poblacion & P){
	double ruleta=0; 
	for(int i=0;i<P.size();i++)	{
		ruleta += P[i].fit;
	}
	for(int i=0;i<P.size();i++){
		P[i].prob=P[i].fit/ruleta;
	}
	double acu=0;
	double r=genRandom(0,1);
	for(int i=0;i<P.size();i++){
		acu+=P[i].prob;
		if(r<acu)
			return i;
	}
	return P.size()-1;
}

Sol CalcBest(poblacion & A){
	sort(A.begin(), A.end(),compare);
	return A[0];
}

void excedLimt(poblacion & P, int L ){
	for(auto &y: P){
		if(y.cont > L){
			for(int j=0;j<2;j++){
				y.val[j]= min(y.val[0],y.val[1])+ genRandom(0,1)*(max(y.val[0],y.val[1])-min(y.val[0],y.val[1]));
			}
			y.fEval=0;
			y.fit=0;
			y.cont=0;
			calcfEvalSol(y);calcFit(y);
		}

	}
}

int main(){
	srand (time(NULL));
	int CS=4; //tamaño de colmena
	int D=2 ; //Numero de variables
	int L=(CS*D)/2; //num de iteracciones antes de abandonar la fuente
	int MCN=1; //Maximo numero de ciclos que itera el algorimo
	int SN=3; //Numero de Soluciones
	poblacion cand, best;
	
	poblacion P = get_poblacion_inicial(CS);//poblacion inicial
	evaluar_poblacion(P);//evaluar poblacion
	printpoblacion("Calculando Iniciales fEval y fitness\n", P);
	int ite = 0;
	Sol mejor_s;//mejor

	while(ite<MCN){
		cout<<"____________________ Iteracion "<<ite<<"____________________"<<endl;
		cand.clear();
		printpoblacion("\nCalculando Iniciales fEval y fitness \n", P);
		
		cand = genCandidatas(P,CS,D,SN);//producir nuevas soluciones --
		evaluar_poblacion(cand);//valuar
		comppoblacion(P,cand);
		printpoblacion("\nCalculando Candidatos fEval y fitness \n", cand);
		
		P = bestSols(P,cand);
		printpoblacion("\nMejores Soluciones \n", P);	
		
		int obs=0;
		while(obs<SN){
			cout<<"\n------ Observadora "<<obs<<" ------"<<endl;
			double solElegida= probSeleccion(P);
			printpoblacionProb("\n Mejores Soluciones con Probabilidad \n", P);
			genNuevaObs(P,solElegida,D,SN);//--
			obs++;
		}
		
		excedLimt(P,L);//--
		printpoblacionProb("\nCiclo Terminado , Comprobando si se acabo recursos \n", P);
		sort(P.begin(),P.end(),compare);
		if( P[0].fit < mejor_s.fit )
			mejor_s= P[0];
		printSol("\nEl mejor acumulado\n", mejor_s);
		ite++;		
	}
	return 0;
}