/*
 * ArbolBusquedaBinaria.cpp
 *
 */

#ifdef ARBOLAVL_H_

#include<iostream>
#include<cmath>

template <typename TipoDato>
ArbolAVL<TipoDato>::ArbolAVL(){
	raiz = NULL;
}

template <typename TipoDato>
int ArbolAVL<TipoDato>::getAltura(ArbolAVL<TipoDato>::NodoAVL* nodo){
	return nodo == NULL ? 0 : nodo->altura;
}

template <typename TipoDato>
TipoDato ArbolAVL<TipoDato>::minimo(){
	return minimoNodo(raiz)->dato;
}

template <typename TipoDato>
typename ArbolAVL<TipoDato>::NodoAVL * ArbolAVL<TipoDato>::minimoNodo(ArbolAVL<TipoDato>::NodoAVL*nodo) {
	if (nodo == NULL) return NULL;
	if (nodo->izq == NULL) return nodo;
	return minimoNodo(nodo->izq);
}

//maximo
template <typename TipoDato>
TipoDato ArbolAVL<TipoDato>::maximo(){
	return maximoNodo(raiz)->dato;
}

template <typename TipoDato>
typename ArbolAVL<TipoDato>::NodoAVL * ArbolAVL<TipoDato>::maximoNodo(ArbolAVL<TipoDato>::NodoAVL*nodo) {
	if (nodo == NULL) return NULL;
	if (nodo->der == NULL) return nodo;
	return minimoNodo(nodo->der);
}

// sucesor
template <typename TipoDato>
TipoDato ArbolAVL<TipoDato>::sucesor(TipoDato dato){
	NodoAVL * nodo = buscarNodo(raiz, dato);
	if (nodo != NULL){
		NodoAVL * sucesor = sucesorNodo(nodo);
		if (sucesor != NULL)
			return sucesor -> dato;
	}
	std::cout << "El elemento " << dato << " no tiene sucesor en el árbol" << std::endl;
	return NULL;
}

template <typename TipoDato>
typename ArbolAVL<TipoDato>::NodoAVL * ArbolAVL<TipoDato>::sucesorNodo(ArbolAVL<TipoDato>::NodoAVL* nodo) {
	if (nodo->der != NULL) return minimoNodo(nodo->der);

	NodoAVL *padre = getNodoPadre(nodo->dato);
	while (padre!=NULL && nodo == padre->der){
		nodo = padre;
		padre = getNodoPadre(padre->dato);
	}
	return padre;
}


//nodo padre
template <typename TipoDato>
typename ArbolAVL<TipoDato>::NodoAVL * ArbolAVL<TipoDato>::getNodoPadre(TipoDato dato){
	if (raiz -> dato == dato)
		return NULL;
	return getNodoPadre(dato, raiz);
}

template <typename TipoDato>
typename ArbolAVL<TipoDato>::NodoAVL * ArbolAVL<TipoDato>::getNodoPadre(TipoDato dato, ArbolAVL<TipoDato>::NodoAVL * nodo){
	if (nodo == NULL)
		return NULL;
	if( (nodo->izq != NULL && nodo->izq->dato == dato) || (nodo->der != NULL && nodo->der->dato == dato) ){
		return nodo;
	}
	if(dato < nodo->dato)
		return getNodoPadre(dato, nodo->izq);
	else
		return getNodoPadre(dato, nodo->der);
}



// buscar

template <typename TipoDato>
bool ArbolAVL<TipoDato>::buscar(TipoDato dato){
	return buscarNodo(raiz, dato) != NULL;
}


template <typename TipoDato>
typename ArbolAVL<TipoDato>::NodoAVL * ArbolAVL<TipoDato>::buscarNodo(ArbolAVL<TipoDato>::NodoAVL*nodo, TipoDato dato) {
	if (nodo == NULL) return NULL;
	if (dato == nodo->dato) return nodo;
	if (dato < nodo->dato) {
		return buscarNodo(nodo->izq, dato);
	} else {
		return buscarNodo(nodo->der, dato);
	}
}


template <typename TipoDato>
void ArbolAVL<TipoDato>::insertar(TipoDato dato){
	insertarNodo(raiz, dato);
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::insertarNodo(ArbolAVL<TipoDato>::NodoAVL * & nodo, TipoDato dato){
	if (nodo == NULL) {
		nodo = new NodoAVL;
		nodo->dato = dato;
		nodo->altura = 1;
		nodo->izq = nodo->der = NULL;
	} else {
		if (dato != nodo->dato) {
			if (dato < nodo->dato) {
				insertarNodo(nodo->izq, dato);
			} else {
				insertarNodo(nodo->der, dato);
			}
		}
	}

	balancear(nodo);

}


template <typename TipoDato>
void ArbolAVL<TipoDato>::eliminar(TipoDato dato){
	eliminarNodo(raiz, dato);
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::eliminarNodo(ArbolAVL<TipoDato>::NodoAVL * & nodo, TipoDato dato){
	if (nodo == NULL)
		return;
	if(dato < nodo->dato ){
		eliminarNodo(nodo->izq, dato);
	}else if(dato > nodo->dato){
		eliminarNodo(nodo->der, dato);
	}else if(nodo->izq != NULL && nodo->der != NULL){ // caso 2 hijos
		nodo -> dato = minimoNodo(nodo->der)->dato;	  // reemplaza dato del nodo a eliminar por el dato del minimo nodo en el subarbol derecho
		eliminarNodo(nodo->der, dato);		  // elimina dato que quedó duplicado en el subárbol derecho
	}else{											  // caso 1 hijo
		NodoAVL *nodoViejo;
		nodo = (nodo->izq != NULL) ? nodo->izq : nodo->der;
		delete nodoViejo;
	}

	balancear(nodo);
}



template <typename TipoDato>
void ArbolAVL<TipoDato>::imprimir(){
	std::cout << "- - -" << std::endl;
	imprimirArbol(raiz);
	std::cout << "- - -" << std::endl;
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::imprimirArbol(ArbolAVL<TipoDato>::NodoAVL *nodo){
	if (nodo != NULL) {
		imprimirArbol(nodo->izq);
		std::cout << nodo->dato << std::endl;
		imprimirArbol(nodo->der);
	}
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::imprimirPreOrden(){
	std::cout << "- - -" << std::endl;
	imprimirArbolPreOrden(raiz);
	std::cout << "- - -" << std::endl;
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::imprimirArbolPreOrden(ArbolAVL<TipoDato>::NodoAVL *nodo){
	if (nodo != NULL) {
		std::cout << nodo->dato << std::endl;
		imprimirArbolPreOrden(nodo->izq);
		imprimirArbolPreOrden(nodo->der);
	}
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::imprimirPostOrden(){
	std::cout << "- - -" << std::endl;
	imprimirArbolPostOrden(raiz);
	std::cout << "- - -" << std::endl;
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::imprimirArbolPostOrden(ArbolAVL<TipoDato>::NodoAVL *nodo){
	if (nodo != NULL) {
		imprimirArbolPostOrden(nodo->izq);
		imprimirArbolPostOrden(nodo->der);
		std::cout << nodo->dato << std::endl;
	}
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::balancear(ArbolAVL<TipoDato>::NodoAVL * & nodo){
	if( nodo == NULL )
		return;

	if( getAltura( nodo->izq ) - getAltura( nodo->der ) > MAX_IMBALANCE ){
		if( getAltura( nodo->izq->izq ) >= getAltura( nodo->izq->der ) )
			rotarHijoIzq( nodo );
		else
			dobleRotacionHijoIzq( nodo );
	}
	else if( getAltura( nodo->der ) - getAltura( nodo->izq ) > MAX_IMBALANCE ){
		if( getAltura( nodo->der->der ) >= getAltura( nodo->der->izq ) )
			rotarHijoDer( nodo );
		else
			dobleRotacionHijoDer( nodo );
	}
	nodo->altura = (std::max)( getAltura( nodo->izq ), getAltura( nodo->der ) ) + 1;
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::rotarHijoIzq(ArbolAVL<TipoDato>::NodoAVL * & a){

	NodoAVL *new_a = a->izq;
	a->izq = new_a->der;
	new_a->der = a;

	a->altura = (std::max)(getAltura(a->izq), getAltura(a->der) ) + 1;
	new_a->altura = (std::max)(getAltura(new_a->izq), getAltura(new_a->der) ) + 1;

	a = new_a;
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::rotarHijoDer(ArbolAVL<TipoDato>::NodoAVL * & a){

	NodoAVL *new_a = a->der;
	a->der = new_a->izq;
	new_a->izq = a;

	a->altura = (std::max)(getAltura(a->izq), getAltura(a->der) ) + 1;
	new_a->altura = (std::max)(getAltura(new_a->izq), getAltura(new_a->der) ) + 1;

	a = new_a;
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::dobleRotacionHijoIzq(ArbolAVL<TipoDato>::NodoAVL * & a){
	rotarHijoDer(a -> izq);
	rotarHijoIzq(a);
}

template <typename TipoDato>
void ArbolAVL<TipoDato>::dobleRotacionHijoDer(ArbolAVL<TipoDato>::NodoAVL * & a){
	rotarHijoIzq(a -> der);
	rotarHijoDer(a);
}

#endif

