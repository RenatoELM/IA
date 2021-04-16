#include<iostream>
#include<vector>
using namespace std;

template<class G>
class arista {
public:
	typedef typename G::nodo nodo;
	typedef typename G::E E;
	vector<nodo*> nodos;
	E peso;
	bool dir;
	arista(nodo* inicio, nodo* fin, E ps, bool direccion) {
		nodos.push_back(inicio);
		nodos.push_back(fin);
		peso = ps;
		dir = direccion;
	}
};

template<class G>
class nodo {
public:
	typedef typename G::arista arista;
	typedef typename G::N N;
	vector<arista*> links;
	N val;
	int color = 0;
};

template<class N, class E>
class grafo {
public:
	typedef grafo<N, E> self;
	typedef arista<self> arista;
	typedef nodo<self> nodo;
	typedef N N;
	typedef E E;
	vector<nodo*> c_nodos;
	bool ins_nodo(N valor);
	bool ins_arista(N inicio, N fin, E peso, bool direccion);
	bool rem_nodo(N valor);
	bool remove_arista_dos(nodo* inicio, nodo* fin, E peso);
	bool remove_arista_uno(N inicio, N fin, E peso);
	void coloracion();
	void print_color();
	void pathfinding(N inicio, N fin);
};

template<class N, class E>
bool grafo<N, E>::ins_nodo(N valor) {
	bool encontrado = false;
	for (int i = 0; i < c_nodos.size(); i++) {
		if (c_nodos[i]->val == valor) {
			encontrado = true;
		}
	}
	if (!encontrado) {
		nodo* p = new nodo;
		p->val = valor;
		c_nodos.push_back(p);
		return true;
	}
	else {
		return false;
	}
}

template<class N, class E>
bool grafo<N, E>::ins_arista(N inicio, N fin, E peso, bool direccion) {
	nodo* begin = NULL;
	nodo* end = NULL;
	for (int i = 0; i < c_nodos.size(); i++) {
		if (c_nodos[i]->val == inicio) {
			begin = c_nodos[i];
		}
		if (c_nodos[i]->val == fin) {
			end = c_nodos[i];
		}
	}
	if (begin && end) {
		arista* p = new arista(begin, end, peso, direccion);
		begin->links.push_back(p);
		end->links.push_back(p);
		return true;
	}
	return false;
}

template<class N, class E>
bool grafo<N, E>::rem_nodo(N valor) {
	nodo* aux = NULL;
	for (int i = 0; i < c_nodos.size(); i++) {
		if (c_nodos[i]->val == valor) {
			aux = c_nodos[i];
			c_nodos.erase(c_nodos.begin() + i);
		}
	}
	if (aux) {
		int i = 0;
		nodo* ayuda = NULL;
		while (!aux->links.empty()) {
			ayuda = aux->links[0]->nodos[1];
			remove_arista_dos(aux, ayuda, aux->links[0]->peso);
		}
		aux->val = NULL;
		delete aux;
		return true;
	}
	else {
		cout << endl << "nodo no encontrado" << endl;
		return false;
	}
}

template<class N, class E>
bool grafo<N, E>::remove_arista_dos(nodo* inicio, nodo* fin, E peso) {
	arista* aux = NULL;
	for (int i = 0; i < fin->links.size(); i++) {
		if (fin->links[i]->peso == peso && fin->links[i]->nodos[0]->val == inicio->val) {
			aux = fin->links[i];
			fin->links.erase(fin->links.begin() + i);
		}
	}
	for (int i = 0; i < inicio->links.size(); i++) {
		if (inicio->links[i]->peso == peso) {
			aux = inicio->links[i];
			inicio->links.erase(inicio->links.begin() + i);
		}
	}
	if (aux) {
		aux->dir = NULL;
		aux->nodos[0] = aux->nodos[1] = NULL;
		aux->peso = NULL;
		delete aux;
		return true;
	}
	return false;
}

template<class N, class E>
bool grafo<N, E>::remove_arista_uno(N inicio, N fin, E peso) {
	nodo* begin = NULL;
	nodo* end = NULL;
	for (int i = 0; i < c_nodos.size(); i++) {
		if (c_nodos[i]->val == inicio) {
			begin = c_nodos[i];
		}
		if (c_nodos[i]->val == fin) {
			end = c_nodos[i];
		}
	}
	if (begin && end) {
		remove_arista_dos(begin, end, peso);
		return true;
	}
	else {
		cout << endl << "nodo no encontrado" << endl;
		return 0;
	}
}

template<class N, class E>
void grafo<N, E>::coloracion() {
	int color_aux = 1;
	for (int i = 0; i < c_nodos.size(); i++) {
		if (c_nodos[i]->color == 0) {
			c_nodos[i]->color = color_aux;

			for (int j = 0; j < c_nodos[i]->links.size(); j++) {
				if (c_nodos[i]->links[j]->nodos[1]->color == 0) {
					c_nodos[i]->links[j]->nodos[1]->color = -1;
				}
			}
			for (int j = i + 1; j < c_nodos.size(); j++) {
				if (c_nodos[j]->color == 0) {
					for (int k = 0; k < c_nodos[j]->links.size(); k++) {
						if (c_nodos[j]->links[k]->nodos[1]->color == 0 && c_nodos[j]->links[k]->nodos[1] != c_nodos[j]) {
							c_nodos[j]->links[k]->nodos[1]->color = -1;
						}
					}
				}
				if (c_nodos[j]->color == 0) {
					c_nodos[j]->color = color_aux;
				}
			}
			for (int j = i + 1; j < c_nodos.size(); j++) {
				if (c_nodos[j]->color == -1) {
					c_nodos[j]->color = 0;
				}
			}

			color_aux += 1;
		}
	}
}

template<class N, class E>
void grafo<N, E>::print_color() {
	int cont = 1;
	int mayor = 0;;
	for (int i = 0; i < c_nodos.size(); i++) {
		cout << "color: " << cont << " : ";
		for (int j = 0; j < c_nodos.size(); j++) {
			if (c_nodos[j]->color == cont) {
				cout << c_nodos[j]->val << " ";
			}
			if (c_nodos[j]->color > mayor) {
				mayor = c_nodos[j]->color;
			}
		}
		cout << endl;
		cont += 1;
		if (cont > mayor) {
			break;
		}
	}
}

template<class N, class E>
void grafo<N, E>::pathfinding(N inicio, N fin) {
	nodo* begin = NULL;
	nodo* end = NULL;
	for (int i = 0; i < c_nodos.size(); i++) {
		if (c_nodos[i]->val == inicio) {
			begin = c_nodos[i];
		}
		if (c_nodos[i]->val == fin) {
			end = c_nodos[i];
		}
	}
	for (int i = 0; i < begin->links.size(); i++) {
		if (begin->links[i]->nodos[1] == end) {
			cout << "camino es de:" << endl << begin->val << " a " << end->val << endl << "por la arista de valor: " << begin->links[i]->peso;
		}
	}
	bool find = false;
	for (int i = 0; i < begin->links.size(); i++) {
		if (begin->links[i]->nodos[1] == end) {
			//cout << "camino es de:" << endl << begin->val << " a " << end->val << endl << "por la arista de valor: " << begin->links[i]->peso;
			find = true;
		}
		else {
			for (int j = 0; j < )
		}
	}
}

int main() {
	grafo<int, int> uno;
	int valor;
	int valor_2;
	int peso;
	int opcion;
	uno.ins_nodo(1);
	uno.ins_nodo(2);
	uno.ins_nodo(3);
	uno.ins_nodo(4);
	uno.ins_nodo(5);
	uno.ins_nodo(6);

	uno.ins_arista(1, 2, 1, 1);
	uno.ins_arista(1, 4, 1, 1);
	uno.ins_arista(2, 4, 1, 1);
	uno.ins_arista(2, 3, 1, 1);
	uno.ins_arista(2, 5, 1, 1);
	uno.ins_arista(3, 5, 1, 1);
	uno.ins_arista(5, 6, 1, 1);
	cout << endl;
	uno.pathfinding(1, 4);


	return 0;
}