/*
 * ArbolAVL.h

 */

#ifndef ARBOLAVL_H_
#define ARBOLAVL_H_

template <typename TipoDato>
class ArbolAVL {
public:
	/*
	* Tipo: Nodo de Árbol de Búsqueda Binaria
	* ----------
	* Estructura para almacenar cada nodo del árbol.
	*/

	struct NodoAVL {
		TipoDato dato;
		NodoAVL *izq;
		NodoAVL *der;
		int altura;
	};

	// Máximo imbalance permitido
	static const int MAX_IMBALANCE = 1;

	//Constructores
	ArbolAVL();

	ArbolAVL(TipoDato datoIni);

	//~ArbolBusquedaBinaria();

	//Métodos públicos

	int getAltura(NodoAVL* nodo);

	bool buscar(TipoDato dato);

	NodoAVL* buscarNodo(NodoAVL* nodo, TipoDato dato);


	TipoDato minimo();

	NodoAVL* minimoNodo(NodoAVL * nodo);

	TipoDato maximo();

	NodoAVL* maximoNodo(NodoAVL * nodo);

	TipoDato predecesor(TipoDato dato);

	NodoAVL* predecesorNodo(NodoAVL* nodo, TipoDato dato);

	TipoDato sucesor(TipoDato dato);

	NodoAVL* sucesorNodo(NodoAVL* nodo);

	NodoAVL* getNodoPadre(TipoDato dato);

	NodoAVL* getNodoPadre(TipoDato dato, NodoAVL * nodo);

	void insertar(TipoDato dato);

	void insertarNodo(NodoAVL * & nodo, TipoDato dato);

	void eliminar(TipoDato dato);

	void eliminarNodo(NodoAVL * & nodo, TipoDato dato);

	void imprimir();

	void imprimirArbol(NodoAVL * nodo);

	void balancear(NodoAVL * & nodo);

	void rotarHijoIzq(NodoAVL * & nodo);

	void rotarHijoDer(NodoAVL * & nodo);

	void dobleRotacionHijoIzq(NodoAVL * & nodo);

	void dobleRotacionHijoDer(NodoAVL * & nodo);



	void imprimirPreOrden();

	void imprimirArbolPreOrden(NodoAVL * nodo);

	void imprimirPostOrden();

	void imprimirArbolPostOrden(NodoAVL * nodo);



private:
	/* Atributos */
	NodoAVL *raiz; 	/* Apuntador al nodo raíz del árbol */
};



#endif /* ARBOLAVL_H_ */

#include "ArbolAVL.cpp"
