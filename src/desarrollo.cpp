#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <vector>
#include <cctype>
#include "twitcurl.h" //Libreria externa utilizada, puede verse completa en el siguiente enlace : https://github.com/swatkat/twitcurl
#include "oauthlib.h"
#include <algorithm>
#include "ArbolAVL.h"
using namespace std;

//Clase de la cual van a ser los elementos correspondientes a los individuos
class persona {
public:
	map<string, int> *apuntop;
	string indv;
};

//Clase para almacenar los valores de un mapa
class sort_map
{
public:
	string key;
	int val;
};


FILE _iob[] = { *stdin,*stdout,*stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
extern "C" void __imp__set_output__format(void) {}

//Declaración  de funciones creadas
float similitud(map<string, int> & mapa1, map<string, int> & mapa2);
void imprimirmapa(map <string, int> &tabela);
void llenartop(map<string, int> & mapa, map<string, int> & topmapa);
bool Sort_by1(const sort_map &a, const sort_map &b);
void llenarhistogramapal(map<string, int> & mapa, ArbolAVL<string>  & lexicon);
void llenarlexicon(ArbolAVL<string> & arbol);
void llenar(string tmpStr, map<string, int> & mapa, twitCurl & twitterObj, ArbolAVL<string>  & lexicon);
void txt_mapa(map<string, int> & mapa, string palabra);
void guardar(map<string, int> & mapa, string palabra);
void creargrafopal(string candidato, map<string, int> mapa);
void creargraficaCS(float coef, float coef2);




const float ntop = 100.0;//Definición del top

int main() {
	
	twitCurl twitterObj;
	char tmpBuf[1024];
	
	//Inicio de sesión en twitter app
	twitterObj.setTwitterUsername("jumianse");
	twitterObj.setTwitterPassword("jumianse1234");

	
	//Declaración de las llaves y tokens de accesso de la app de twitter
	
	cout << "test" << endl;
	string consumerKey = "ZFCEdnBJas6eZC8VjiZjsbo6a";
	string consumerKeysecret = "sXGyCSoqFUvvl3doDxsQ1z8ZcjXCG6firChRKWFQWiZdiHk5Tw";
	string myOAuthAccessTokenKey = "999023718406991874-X28I2Nj6KwxhsSyjDUUCRjKNviw3ZAS";
	string myOAuthAccessTokenSecret = "14WknAYL6GOs5lePcLt4yR2dkXcuJk5XPJ5ldPKwxcILj";

	// Setting Keys - Poniendo los tokens , username y password

	//Registro para acceder a la app creada
	
	twitterObj.getOAuth().setConsumerKey(consumerKey);
	twitterObj.getOAuth().setConsumerSecret(consumerKeysecret);
	twitterObj.getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
	twitterObj.getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);

	//Crear Lexicon
	ArbolAVL<string> lexicon;		//Arbol para stopwords
	llenarlexicon(lexicon);		//Se llena el arbol de stopwords
	//lexicon.imprimir();		//Remueva los caracteres "//" de esta linea y la siguiente si desea probar
	//system("pause");


	//Crear individuos
	map<string, int> palanalip;		//mapa individuo 1
	map<string, int> palanalif;		//mapa individuo 2
	map<string, int> palanalid;		//mapa individuo 3

	map<string, int>topp;		//Top individuo 1
	map<string, int>topf;		//Top individuo 2
	map<string, int>topd;		//Top individuo 3

	persona petro;		//Indiviudo 1
	persona fajardo;	//Individuo 2
	persona duque;		//Individuo 3
	
	petro.indv = "petro";
	fajardo.indv = "fajardo";
	duque.indv = "duque";

	petro.apuntop = &topp;
	fajardo.apuntop = &topf;
	duque.apuntop = &topd;



	// Obtener histograma de palabras en tweets para cada individuo
	llenar(petro.indv, palanalip, twitterObj, lexicon);
	llenar(fajardo.indv, palanalif, twitterObj, lexicon);
	llenar(duque.indv, palanalid, twitterObj, lexicon);
	//imprimirmapa(palanalip);
	//system("pause");


	//Obtener el top N de cada individuo
	llenartop(palanalip, topp);
	llenartop(palanalif, topf);
	llenartop(palanalid, topd);
	//imprimirmapa(topp);
	//system("pause");

	//Obtener coeficientes de similitud
	float CS_P_D = similitud(topp, topd);
	float CS_P_F = similitud(topp, topf);
	float CS_D_F = similitud(topd, topf);

	cout << "Coef. Petro-Duque: "<< CS_P_D << endl;
	cout << "Coef. Petro-Fajardo: " << CS_P_F << endl;
	cout << "Coef. Duque-Fajardo: " << CS_D_F << endl;

	system("pause");
	
	creargrafopal(petro.indv, *(petro.apuntop));//Graficar top de palabras para Petro

	creargraficaCS(CS_P_D, CS_P_F); Graficar realaciones de similitud de Petro
	
//Llamadas de sistema para compilar los archivos escritos
	system("\"C:\\Program Files\\MiKTeX 2.9\\miktex\\bin\\x64\\pdflatex.exe\" -output-directory C:\\data\\  C:\\data\\toppalind.tex");
	system("\"C:\\Program Files\\MiKTeX 2.9\\miktex\\bin\\x64\\pdflatex.exe\" -output-directory C:\\data\\  C:\\data\\coefsimil.tex");

	system("\"C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroRD32.exe\" C:\\data\\toppalind.pdf");
	system("\"C:\\Program Files (x86)\\Adobe\\Acrobat Reader DC\\Reader\\AcroRD32.exe\" C:\\data\\coefsimil.pdf");
	system("pause");
	return 0;
}



//Funciones

//Función que llena el mapa de cada individuo con la extracción de tweets n veces
void llenar(string tmpStr, map<string, int> & mapa, twitCurl & twitterObj, ArbolAVL<string>  & lexicon) {

	//Obtener las palabras contenidas en el histórial que se han recolectado previamente
	txt_mapa(mapa, tmpStr);
//Acceso a los nuevos tweets
	for (int i = 0; i <= 10; i++) {
		 
		string replyMsg = "";
		if (twitterObj.search(tmpStr))
		{
			twitterObj.getLastWebResponse(replyMsg);
			printf("\ntwitterClient:: twitCurl::search web response:\n%s\n", replyMsg.c_str());
		}
		else
		{
			twitterObj.getLastCurlError(replyMsg);
			printf("\ntwitterClient:: twitCurl::search error:\n%s\n", replyMsg.c_str());
		}


		ofstream out("Twitter_API.txt");
		out << replyMsg;
//LLamada de la función para llenar histograma principal
		llenarhistogramapal(mapa, lexicon); // LLena el histograma que tanto se repiten las palabras en los tweets, con los datos obtenidos en cada loop

		out.close();
	}

	//Guardar en el historial las nuevas palabras ingresadas
	guardar(mapa, tmpStr);


}

//LLena el arbol con el lexicón
void llenarlexicon(ArbolAVL<string> & arbol) {
	ifstream stopwords;
	string stopw;
	stopwords.open("data/stopwords-es.txt");
	while (getline(stopwords, stopw)) {
		arbol.insertar(stopw);
	}
	stopwords.close();
}


//Llena el mapa con las palabras
void llenarhistogramapal(map<string, int> & mapa, ArbolAVL<string>  & lexicon) {

	ifstream doc;
	doc.open("Twitter_API.txt");
	string linea;
	string tweet = "";
	while (getline(doc, linea)) {
		for (int i = 0; i<linea.length()-5; i++) {
			if (linea[i] == '"'&&linea[i + 1] == 't'&&linea[i + 2] == 'e'&&linea[i + 3] == 'x'&&linea[i + 4] == 't') {
				int e = i + 8; 		//Función para saltar caracteres innecesarios
				while (true) {
					if (linea[e] == '"')break;	//Condición finalizadora del tweet
					else {
						tweet.push_back(linea[e]);	//Formación del tweet a revisar
						e++;
					}

				}
				if (!(tweet[0] == 'R'&&tweet[1] == 'T'&&tweet[2] == ' ')) {
					//cout << tweet << endl;	//Verificación
					
					//Llenado del histograma.
					string palabra = "";
					for (int it2 = 0; it2 < tweet.length(); it2++) {
						if (tweet[it2] == ' ' || tweet[it2] == ',' || tweet[it2] == ';' || tweet[it2] == ':' || tweet[it2] == '(' || tweet[it2] == ')' || tweet[it2] == '.' || tweet[it2] == '-') {
							if (!palabra.empty()) {    //Se comprueba que no esté vacia la palabra.
								if (!lexicon.buscar(palabra)) {    //Se comprueba que no esté en el lexicón de stopwords
									if (mapa.find(palabra) == mapa.end()) {//Se comprueba si la palabra no estaba antes en el mapa
										for (int i = 0; i < palabra.length(); i++) {
											palabra[i] = tolower(palabra[i]);//Se pasa la palabra a minuscula
										}

										mapa.insert(pair<string, int>(palabra, 1));
									}
									else {
										mapa[palabra]++;	//Aumenta el valor de palabras ya ingresadas
									}
									//cout << palabra << "---";		//Verificación
								}
							}
							palabra = "";
						}
						else {
							palabra += tweet[it2]; //Se van agregando los caracteres para formar la palabra
						}
					}
					//Nuevamente para incluir la ultima palabra
					if (!palabra.empty()) {    //Se comprueba que no esté vacia la palabra.
						if (!lexicon.buscar(palabra)) {    //Se comprueba que la palabra no esté en el lexicón de stopwords
							if (mapa.find(palabra) == mapa.end()) { //Se comprueba si la palabra no estaba antes en el mapa
										for (int i = 0; i < palabra.length(); i++) {
											palabra[i] = tolower(palabra[i]);//Se pasa la palabra a minuscula
										}
								mapa.insert(pair<string, int>(palabra, 1));
							}
							else {
								mapa[palabra]++;	//Aumenta el valor de palabras ya ingresadas
							}
							//cout << palabra << "---";		//Verificación
						}
					}

				}
				i = i + tweet.size() + 7;
				tweet = "";
			}
		}
	}
	doc.close();

}



//Recolección del top

//Condición para hacer el ordenamiento
bool Sort_by1(const sort_map& a, const sort_map& b) {
	return a.val > b.val;
}
//Función que llena el top definido
void llenartop(map<string, int> & mapa, map<string, int> & topmapa) {
	map<string, int>::iterator it;
	vector< sort_map > v;
	vector< sort_map >::iterator itv;
	sort_map sm;
//Llenado del vector
	for (it = mapa.begin(); it != mapa.end(); ++it) {
		sm.key = it->first;
		sm.val = it->second;
		v.push_back(sm);
	}
//Ordenamiento del vector
	std::sort(v.begin(), v.end(), Sort_by1);
//Llenado del mapa top
	for (itv = v.begin(); itv != v.begin() + ntop; itv++) {
		topmapa.insert(std::pair<string, int>(itv->key, itv->val));
	}

}

//Impresión de mapas, para pruebas
void imprimirmapa(map <string, int> &tabela) {
	map<string, int>::iterator it;
	for (it = tabela.begin(); it != tabela.end(); ++it) {
		cout << it->first << ": " << it->second << endl;
	}
}

//Coeficiente de similitud, este busca el número de palabras en común
float similitud(map<string, int> & mapa1, map<string, int> & mapa2) {	//Revisar el iterador, no funciona bien
	map<string, int>::iterator it;
	float cont = 0;
	for (it = mapa1.begin(); it != mapa1.end(); it++) {
		if (mapa2.find(it->first) != mapa2.end()) {
			cont++;
		}
	}

	float indice = cont / ntop;
	return indice;
}

//Funciones para actualizar el historial de palabras de cada individuo
void guardar(map<string, int> & mapa, string palabra) {
	std::map<string, int>::iterator it;
	std::ofstream archivo;
	if (palabra == "petro") {
		archivo.open("petro.txt");
		if (archivo.is_open()) {
			for (it = mapa.begin(); it != mapa.end(); it++) {
				archivo << it->first << "," << it->second << endl;
				archivo.flush();
			}
		}
		else {
			cerr << "No se pudo guardar" << endl;
		}
	}
	else if (palabra == "duque") {
		archivo.open("duque.txt");
		if (archivo.is_open()) {
			for (it = mapa.begin(); it != mapa.end(); it++) {
				archivo << it->first << "," << it->second << endl;
				archivo.flush();
			}
		}
		else {
			cerr << "no se pudo guardar" << endl;
		}
	}
	else if (palabra == "fajardo") {
		archivo.open("fajardo.txt");
		if (archivo.is_open()) {
			for (it = mapa.begin(); it != mapa.end(); it++) {
				archivo << it->first << "," << it->second << endl;
				archivo.flush();
			}
		}
		else {
			cerr << "No se pudo guardar" << endl;
		}

	}
	archivo.close();
}


//Función que actualiza el mapa de la ejecución con el historial de ejecuciones anteriores de cada individuo
void txt_mapa(map<string, int> & mapa, string palabra) {
	std::map<string, int>::iterator it;
	std::ifstream archivo;
	string linea;
	string delimitador = ",";
	if (palabra == "petro") {
		archivo.open("petro.txt");
		if (archivo.is_open()) {
			cout << "lo pudo abrir para leer" << endl;
		}
		else {
			cerr << "No se pudo abrir" << endl;
		}
	}
	else if (palabra == "duque") {
		archivo.open("duque.txt");
		if (archivo.is_open()) {
			cout << "lo pudo abrir para leer" << endl;
		}
		else {
			cerr << "No se pudo abrir" << endl;
		}
	}
	else if (palabra == "fajardo") {
		archivo.open("fajardo.txt");
		if (archivo.is_open()) {
			cout << "lo pudo abrir para leer" << endl;
		}
		else {
			cerr << "No se pudo abrir" << endl;

		}

	}
	while (getline(archivo, linea)) {
		string palabra = linea.substr(0, linea.find(delimitador));
		string entero = linea.substr(linea.find(delimitador) + 1, linea.length());
		int entero1 = std::stoi(entero);
		mapa.insert(pair<string, int>(palabra, entero1));
	}
	archivo.close();
}


//Función que escribe grafica para mostrar el top de palabras
void creargrafopal(string candidato, map<string, int> mapa) {

	//Extracción de las primeras 8 palabras.
	map<string, int>::iterator it;
	vector< sort_map > vec;
	sort_map sm;

	for (it = mapa.begin(); it != mapa.end(); ++it) {
		sm.key = it->first;
		sm.val = it->second;
		vec.push_back(sm);
	}
//Ordenamiento
	std::sort(vec.begin(), vec.end(), Sort_by1);

//Control de casos especiales que pueden producir errores al compilar en latex
	for (int i = 0; i < 10; i++) {
		string palabra = vec[i].key;
		for (int e = 0; e < palabra.length(); e++) {
			if (palabra[e] == '\\') {
				palabra[e] = 'b';
			}
			else if (palabra[e] == '0' || palabra[e] == '1' || palabra[e] == '2' || palabra[e] == '3' || palabra[e] == '4'
				|| palabra[e] == '5' || palabra[e] == '6' || palabra[e] == '7' || palabra[e] == '8' || palabra[e] == '9') {
				palabra[e] = 'n';
			}

			vec[i].key = palabra;

		}
	}

		std::ofstream archivo;
		archivo.open("C:/data/toppalind.tex");
		if (archivo.is_open()) {
			archivo << "\\documentclass{article}" << endl;
			archivo << "\\usepackage{tikz}" << endl;
			archivo << "\\usepackage[active,tightpage]{preview}" << endl;
			archivo << "\\PreviewEnvironment{tikzpicture}" << endl;
			archivo << "\\setlength\\PreviewBorder{10pt}" << endl;
			archivo << "\\definecolor{lavander}{cmyk}{0,0.48,0,0}" << endl;
			archivo << "\\definecolor{violet}{cmyk}{0.79,0.88,0,0}" << endl;
			archivo << "\\definecolor{burntorange}{cmyk}{0,0.52,1,0}" << endl;
			archivo << "\\def\\lav{lavander!90}" << endl;
			archivo << "\\def\\oran{orange!30}" << endl;
			archivo << endl;
			archivo << endl;
			archivo << "\\tikzstyle{peers}=[draw,circle,violet,bottom color=\\lav, top color= white, text=violet,minimum width=10pt]" << endl;
			archivo << "\\tikzstyle{superpeers}=[draw,rectangle,burntorange, left color=\\oran, text=violet,minimum width=40pt]" << endl;
			archivo << "\\tikzstyle{legendsp}=[rectangle, draw, burntorange, rounded corners, thin,bottom color=\\oran, top color=white, text=burntorange, minimum width=2.5cm]" << endl;
			archivo << "\\tikzstyle{legend_general}=[rectangle, rounded corners, thin, burntorange, fill= white, draw, text=violet, minimum width=2.5cm, minimum height=0.8cm]" << endl;
			archivo << endl;
			archivo << endl;
			archivo << "\\begin{document}" << endl;
			archivo << "\\begin{tikzpicture}[auto, thick]" << endl;
			archivo << endl;
			archivo << "  % Poner superpeer para el elemento de candidato" << endl;
			archivo << "  \\foreach \\place/\\name in {{(5,2)/" << candidato << "}}" << endl;
			archivo << "    \\node[superpeers] (\\name) at \\place {\\name};" << endl;
			archivo << endl;
			archivo << "   % Palabras del top del candidato" << endl;
			archivo << "   \\foreach \\place/\\name in {{(5,5)/" << vec[0].key << "},{(5,-1)/" << vec[1].key << "},{(7.5,4.5)/" << vec[2].key << "},{(7.5,-0.5)/" << vec[3].key << "},{(2.5,4.5)/" << vec[4].key << "},{(2.5,-0.5)/" << vec[5].key << "},{(2,2)/" << vec[6].key << "},{(8,2)/" << vec[7].key << "}}" << endl;
			archivo << "   \\node[peers] (\\name) at \\place {\\name};" << endl;
			archivo << endl;
			archivo << "%Lineas" << endl;
			archivo << "   \\foreach \\source/\\dest in { " << candidato << "/" << vec[0].key << ", " << candidato << "/" << vec[1].key << ", " << candidato << "/" << vec[2].key << ", " << candidato << "/" << vec[3].key << ", " << candidato << "/" << vec[4].key << ", " << candidato << "/" << vec[5].key << ", " << candidato << "/" << vec[6].key << ", " << candidato << "/" << vec[7].key << "}" << endl;
			archivo << "   \\path (\\source) edge (\\dest);" << endl;
			archivo << endl;
			archivo << "   % Legends" << endl;
			archivo << "   \\node[legend_general] at (1,6) {\\small{\\textsc{Top candidato}}};" << endl;
			archivo << endl;
			archivo << "\\end{tikzpicture}" << endl;
			archivo << "\\end{document}" << endl;


		}
		else {
			cerr << "No se pudo guardar" << endl;
		}
		archivo.close();

	}

//Función que crea la grafica que representa los coeficientes de similitud
	void creargraficaCS(float coef, float coef2) {

		float distancia1 = (1 - coef) * 2;	//Calculos requeridos que producen las coordenadas correspondientes
		float distancia2 = (1 - coef2) * 2;

		std::ofstream archivo;
		archivo.open("C:/data/coefsimil.tex");
		if (archivo.is_open()) {
			archivo << "\\documentclass{article}" << endl;
			archivo << "\\usepackage{tikz}" << endl;
			archivo << "\\usepackage[active,tightpage]{preview}" << endl;
			archivo << "\\PreviewEnvironment{tikzpicture}" << endl;
			archivo << "\\setlength\\PreviewBorder{10pt}" << endl;
			archivo << "\\definecolor{lavander}{cmyk}{0,0.48,0,0}" << endl;
			archivo << "\\definecolor{violet}{cmyk}{0.79,0.88,0,0}" << endl;
			archivo << "\\definecolor{burntorange}{cmyk}{0,0.52,1,0}" << endl;
			archivo << "\\def\\lav{lavander!90}" << endl;
			archivo << "\\def\\oran{orange!30}" << endl;
			archivo << endl;
			archivo << endl;
			archivo << "\\tikzstyle{levelone} = [draw, circle, blue, bottom color = white, text = white, minimum width = 120pt]" << endl;
			archivo << "\\tikzstyle{leveltwo} = [draw, circle, blue, bottom color = white, text = white, minimum width = 80pt]" << endl;
			archivo << "\\tikzstyle{levelthree} = [draw, circle, blue, bottom color = white, text = white, minimum width = 40pt]" << endl;
			archivo << "\\tikzstyle{superpeers} = [draw, circle, burntorange, left color = \\oran,text = violet, minimum width = 15pt]" << endl;
			archivo << "\\tikzstyle{legendsp} = [rectangle, draw, burntorange, rounded corners,thin, bottom color = \\oran, top color = white,text = burntorange, minimum width = 2.5cm]" << endl;
			archivo << endl;
			archivo << "\\begin{document}" << endl;
			archivo << "\\begin{tikzpicture}[auto, thick]" << endl;
			archivo << endl;
			archivo << "  \\foreach \\place/\\name in {(0,0)/lone}" << endl;
			archivo << "    \\node[levelone] (\\name) at \\place {};" << endl;
			archivo << endl;
			archivo << "  \\foreach \\place/\\name in {(0,0)/ltwo}" << endl;
			archivo << "    \\node[leveltwo] (\\name) at \\place {};" << endl;
			archivo << endl;
			archivo << "  \\foreach \\place/\\name in {(0,0)/lthree}" << endl;
			archivo << "    \\node[levelthree] (\\name) at \\place {};" << endl;
			archivo << endl;
			archivo << "% Place super peers" << endl;
			archivo << "  \\foreach \\place/\\name in {{(0,0)/P}, {(" << distancia1 << ",0)/D}, {(" << -distancia2 << ",0)/F}}" << endl;
			archivo << "    \\node[superpeers] (\\name) at \\place {\\name};" << endl;
			archivo << endl;
			archivo << "%Legends" << endl;
			archivo << "   \\node[legendsp] at (3,-2) {\\small{D:" << coef << ", F:" << coef2 << "}};" << endl;
			archivo << endl;
			archivo << "\\end{tikzpicture}" << endl;
			archivo << "\\end{document}" << endl;

		}
		else {

			cerr << "No se pudo guardar" << endl;
		}
		archivo.close();

	}


