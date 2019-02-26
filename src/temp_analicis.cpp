#include"ArbolesAVL.h"
#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<algorithm>
#include<vector>;
using namespace std;


const float ntop = 50.0;//Definición del top

//Clase de la cual van a ser los elementos correspondientes a los individuos
class persona{
public:
	map<string,int> *apuntador;
	string indv;
};

// funcion coeficiente similitud. 
float similitud(persona persona1,persona persona2){	//Revisar el iterador, no funciones bien
	map<string,int>::iterator it;
	int cont = 0;
	for(it = persona1.apuntador->begin(); it != persona1.apuntador->end();  it++){
		if(!(persona2.apuntador->find(it->first) == persona2.apuntador->end())){
			cont++;
		}
	}

	float indice = cont/ntop;
	return indice;
}

//Recolección del top

//Clase para almacenar los valores de un mapa
	class sort_map
{
  public:
	string key;
	int val;
};

//Condición para hacer el ordenamiento
bool Sort_by(const sort_map& a ,const sort_map& b){
	return a.val > b.val;
}


void llenartop( map<string,int> & mapa,map<string,int> & topmapa){
	map<string,int>::iterator it;
	vector< sort_map > v;
	vector< sort_map >::iterator itv;
	sort_map sm;

	for (it = mapa.begin(); it != mapa.end(); ++it){
		sm.key = it->first;
		sm.val = it->second;
		v.push_back(sm);
	}

	sort(v.begin(),v.end(),Sort_by);

	for(itv = v.begin(); itv != v.begin()+ntop; itv++){
	    topmapa.insert(std::pair<string,int>(itv->key,itv->val));
	}

}




//Función para imprimir el histograma
void imprimirmapa(map <string, int> &tabela) {
  map<string, int>::iterator it;
  for (it = tabela.begin(); it != tabela.end(); ++it) {
    cout << it->first << ": " << it->second << endl;
  }
}

void imprimirmapaapun(map <string, int> *tabela) {
  map<string, int>::iterator it;
  for (it = tabela->begin(); it != tabela->end(); ++it) {
    cout << it->first << ": " << it->second << endl;
  }
}


//LLena el arbol con el lexicón
void llenarlexicon(ArbolesAVL<string> & arbol){
	ifstream stopwords;
	string stopw;
	stopwords.open("data/stopwords-es.txt");
	while(getline(stopwords,stopw)){
		arbol.insertar(stopw);
	}
	stopwords.close();
}


//Llena el mapa con las palabras
void llenarhistogramapal(map<string, int> & mapa, ArbolesAVL<string> lexicon){

	ifstream doc;
		doc.open("data/Twitter_API.txt");
		string linea;
		string tweet= "";
		while(getline(doc, linea)){
			for(int i = 0; i<linea.length(); i++){
				if(linea[i]=='"'&&linea[i+1]=='t'&&linea[i+2]=='e'&&linea[i+3]=='x'&&linea[i+4]=='t'){
					int e = i+8;
					while(true){
						if(linea[e]=='"')break;
						else{
						tweet.push_back(linea[e]);
						e++;
						}

					}
					if(!(tweet[0]=='R'&&tweet[1]=='T'&&tweet[2]==' ')){
						cout << tweet << endl;//Remover posteriormente
						//En esta linea se puede guardar el tweet en una estructura.

						//Contrucción del histograma.
						string palabra = "";
						for(int it2 = 0; it2 < tweet.length(); it2++ ){
							if(tweet[it2]==' '||tweet[it2]==','||tweet[it2]==';'||tweet[it2]==':'||tweet[it2]=='('||tweet[it2]==')'||tweet[it2]=='.'||tweet[it2]=='-'){
								if(!palabra.empty()){    //Se comprueba que no esté vacia la palabra.
									if(!lexicon.buscar(palabra)){    //Se comprueba que no esté en el lexicón de stopwords
										if(mapa.find(palabra) == mapa.end()){
										    mapa.insert(pair<string,int>(palabra,1));
										}else{
											mapa[palabra]++;
										}
										cout << palabra << "---";//Remover posteriormente
									}
								}
								palabra = "";
							}else{
								palabra+= tweet[it2]; //Se van agregando los caracteres para formar la palabra
							}
						}
						//Nuevamente para incluir la ultima palabra
						if(!palabra.empty()){    //Se comprueba que no esté vacia la palabra.
							if(!lexicon.buscar(palabra)){    //Se comprueba que no esté en el lexicón de stopwords
								if(mapa.find(palabra) == mapa.end()){
									mapa.insert(pair<string,int>(palabra,1));
								}else{
									mapa[palabra]++;
								}
								cout << palabra << "---";//Remover posteriormente
							}
						}

					}
					i = i + tweet.size()+7;
					tweet = "";
				}
			}
		}
	doc.close();

}


int main(){

	ArbolesAVL<string> lexicon;		//Arbol para stopwords
	map<string,int> palanali;		//mapa individuo 1
	map<string,int> palanali2;		//mapa individuo 2
	map<string,int> top50;		//Top individuo 1
	map<string,int>top502;		//Top individuo 2
	persona petro1; 	//Indiviudo 1
	persona petro2;		//Individuo 2
	petro1.indv = "Petro"; // Debe asignarse como un dato string que pasan por consola para jalar los tweets

	llenarlexicon(lexicon);		//Se llena el arbol de stopwords
	lexicon.imprimir();		//Prueba

	//Creación mapas de palabras de cada individuo
	llenarhistogramapal(palanali, lexicon);
	llenarhistogramapal(palanali2, lexicon);

	//Creación del top ntop de cada individuo
	llenartop(palanali,top50);
	llenartop(palanali2,top502);

	//Prueba
	imprimirmapa(top50);
	cout << "------"<< endl;
	imprimirmapa(top502);

	cout << similitud(petro1, petro2)<< endl;


}





