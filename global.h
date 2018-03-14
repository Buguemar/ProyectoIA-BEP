//*******************************************************************************************
//-----------------------------MÉTODOS DEL STRUCT -------------------------------------------
//*******************************************************************************************
struct bus {
	vector<int> path;  													//SECUENCIA DE NODOS QUE VISITA EL BUS
	vector<int> dominio;												//SECUENCIA DE NODOS ALCANZABLES DADA LA POSICIÓN DEL BUS EN EL GRAFO
	int pasajeros;          											//CANTIDAD DE PASAJEROS EN EL VIAJE I-ÉSIMO DEL BUS
	int capacidad;      	    										//CAPACIDAD DEL BUS
	int t_recorrido;    												//TIEMPO DE SERVICIO DEL BUS
} ;
void inicializar_bus(bus *b, int c, int n){
	b->path.reserve(n); 												//A LO MÁS, UN BUS EVACÚA A TODAS LAS PERSONAS 
	b->dominio.reserve(n);
	b->pasajeros = 0;
	b->capacidad = c;
	b->t_recorrido = 0;
}
//*******************************************************************************************
//--------------------------- PILA DE RETORNO EN FORWARD CHECKING----------------------------
//*******************************************************************************************
struct bloque{
	vector<int> dominio;  												//NODOS QUE COMPONEN EL DOMINIO QUE VISITA
	vector<int> nodos; 
	int *en_espera2;
	int *cap_albergues2; 
	int asignado;          												//NODO ASIGNADO AL BUS I-ÉSIMO
	int b_iesimo;
	int evacuados;	
};

void init_bloque(bloque *block, int n){
	block->dominio.reserve(n); 											//RESERVA ESPACIO DE, A LO MÁS, N NODOS  
	block->nodos.reserve(n); 	
	block->asignado=0;
}
//*******************************************************************************************
//---------------------------PARAMETROS Y DATOS GLOBALES-------------------------------------
//*******************************************************************************************
int *demandas; 				
int **dist_ini;
int **dist_bi; 
int q_buses;
int cap_b;
int q_dep;
int *buses_dep;
bus *BUSES;
int TMIN;
int q_puntos;
int a_evacuar;    
int *en_espera;			
int q_albergues;
int a_albergar;
int *cap_albergues;		
int *cap_original;
stack <bloque> pila_construct; 
vector<int> nodos_viajes_bi;
bool terminar = false; 
int nodos;
char * archivo_entrada;
char * archivo_salida;

//*******************************************************************************************
//-----------------------------------------FUNCIONES-----------------------------------------
//*******************************************************************************************
bool existe_espera() ;
bool nodos_no_terminales();
vector<int> add_paths(bus *BUS, vector<int> &nodos_viaje_init);
void lectura();
void escritura();
int Leer_entradas(int argc, char **argv);
int det_deposito(int i);
void Forward_Check();
void restablecer_sistema(bloque &poped);
void setear_tiempo_buses();
