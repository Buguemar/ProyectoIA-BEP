 #include "includes.h"
#include <typeinfo>
#include "global.h"

int main (int argc, char *argv[]){
	if(Leer_entradas(argc, argv)){
		srand(time(NULL));
		//SE LEE EL ARCHIVO DE INSTANCIAS Y SE CARGAN LOS DATOS EN LAS ESTRUCTURAS CORRESPONDIENTES
		lectura(); 														
		if (a_evacuar > a_albergar){
			cout << "IMPOSIBLE ENCONTRAR SOLUCIÓN" << endl;
			cout << "LA CANTIDAD DE PERSONAS A EVACUAR SUPERA LA CAPACIDAD DE LOS ALBERGUES" << endl;
			return 0;
		}
		//MEDICIÓN DE TIEMPO 
		clock_t t_ini, t_fin;											
		double tiempo_forward;
		t_ini = clock();
		Forward_Check();
		t_fin = clock();
		tiempo_forward= (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
		cout << "EL TIEMPO TOTAL DE EJECUCION FUE: " << tiempo_forward <<  endl;
	}else{
		cout << "HA OCURRIDO UN ERROR, CHEQUEE LOS ARGUMENTOS EN MAKEFILE E INTENTE NUEVAMENTE\n";
	}
	return 0;
}


//*******************************************************************************************
//---------------------------------------ARCHIVOS-------------------------------------------
//*******************************************************************************************

//FUNCIÓN: LECTURA()
//ENTRADAS: NINGUNA  
//DESCRIPCIÓN: LECTURA DE ARCHIVO DE CONFIGURACIÓN
//SALIDA: NINGUNA
void lectura(){	    
    FILE *instancia;	
	cout << archivo_entrada << endl;
	instancia = fopen(archivo_entrada, "r");
	fscanf(instancia,"%d: ", &q_buses);
	fscanf(instancia,"%d\n", &cap_b);
	fscanf(instancia,"%d: ", &q_dep);
	//RESERVA DE ESPACIO PARA EL ARREGLO DE DEPOSITOS/ORÍGENES DE BUSES
	buses_dep = new int[q_dep];
	int dep = 0;
	int bdep;
	while(dep < q_dep){
		int temp= (q_dep-1);
		if (dep == temp) {
			fscanf(instancia, "%d\n", &bdep);
			buses_dep[dep]= bdep;
			dep++;
		}
		else{
			fscanf(instancia, "%d ", &bdep);
			buses_dep[dep]= bdep;			
			dep++;	
		} 
    }  
	fscanf(instancia,"%d: ", &q_puntos);
	fscanf(instancia,"%d: ", &a_evacuar);
	//RESERVA DE ESPACIO PARA EL ARREGLO DE CANTIDAD DE PERSONAS EN ESPERA (A EVACUAR) EN CADA PUNTO DE ENCUENTRO
	en_espera = new int[q_puntos];
	int pe = 0;
	int esperando;
	while(pe < q_puntos){
		if (pe == (q_puntos-1)) {
			fscanf(instancia, "%d\n", &esperando);
			en_espera[pe]= esperando;
			pe++;
		}
		else{
			fscanf(instancia, "%d ", &esperando);
			en_espera[pe]= esperando;
			pe++;
		} 
    }
	fscanf(instancia,"%d: ", &q_albergues);
	fscanf(instancia,"%d: ", &a_albergar);
	//RESERVA DE ESPACIO PARA EL ARREGLO DE CAPACIDADES DE CADA ALBERGUE MEDIDO EN NÚMERO DE PERSONAS 
	cap_albergues = new int[q_albergues];
	cap_original = new int[q_albergues];
	int ps = 0;
	int disponible;
	while(ps < q_albergues){
		if (ps == (q_albergues-1)) {
			fscanf(instancia, "%d\n\n", &disponible);
			cap_albergues[ps]= disponible;
			cap_original[ps]=disponible;
			ps++;
		}
		else{
			fscanf(instancia, "%d ", &disponible);
			cap_albergues[ps]= disponible;
			cap_original[ps]=disponible;			
			ps++;	
		} 
    }
    //RESERVA DE ESPACIO PARA LA MATRIZ DE DISTANCIAS INICIALES, ES DECIR, DESDE DEPÓSITOS HACIA PUNTOS DE ENCUENTRO
    dist_ini = new int*[q_dep];
    for(int i=0 ; i< q_dep ; i++){										//matriz de dimensiones q_dep x q_puntos 									
		dist_ini[i] = new int[q_puntos];
    } 
    int nodos_bi = q_puntos + q_albergues;
    //RESERVA DE ESPACIO PARA LA MATRIZ DE DISTANCIAS BIDIRECCIONALES, ES DECIR, DESDE PUNTOS DE ENCUENTRO HACIA ALBERGUES Y VICEVERSA
	dist_bi = new int*[nodos_bi];								
    for(int i=0 ; i< nodos_bi ; i++)									//matriz de dimensiones nodos_bi x nodos_bi	
		dist_bi[i] = new int[nodos_bi];
    //LECTURA DE DISTANCIAS DESDE CADA DEPÓSITO HACIA CADA UNO DE LOS PUNTOS DE ENCUENTRO
    int d;
    int punto;
    int temporal;    
    for (int i=0; i<q_dep; i++){
		punto=0;													
		fscanf(instancia, "%d: ", &temporal);
		while(punto < q_puntos){
			if (punto == (q_puntos-1)){									
				fscanf(instancia, "%d\n", &d);
				dist_ini[i][punto] = d;
				punto++;
			}
			else {
				fscanf(instancia, "%d ", &d);
				dist_ini[i][punto] = d;
				punto++;
			}
		}
	}
	//LECTURA DE DISTANCIAS DESDE CADA PUNTO DE ENCUENTRO HACIA CADA UNO DE LOS ALBERGUES
	d=0;    
    int tempp=0;    											
    for (int i=0; i<q_puntos; i++){							
		int albergue=0;
		fscanf(instancia, "%d: ", &tempp);
		while(albergue < q_albergues){						
			if (albergue == (q_albergues-1)){				
				fscanf(instancia, "%d \n", &d);
				dist_bi[i][albergue + q_puntos] = d;
				dist_bi[albergue + q_puntos][i] = d;				
				punto++;
			}
			else {
				fscanf(instancia, "%d ", &d);
				dist_bi[i][albergue + q_puntos] = d;
				dist_bi[albergue + q_puntos][i] = d;
				punto++;
			}
			albergue++;
		}
	}
	//SE RELLENAN LAS DISTANCIAS ENTRE NODOS DEL MISMO TIPO COMO INALCANZABLES, DECIR, CON MAX
	for(int i=0; i< q_puntos;i++) {
		for(int j=0; j < q_puntos; j++)
			dist_bi[i][j] = MAX;
    }
    for(int i=q_puntos; i< nodos_bi;i++) {
		for(int j=q_puntos; j < nodos_bi; j++)
			dist_bi[i][j] = MAX;
    }
}

//FUNCIÓN: LEER_ENTRADAS(INT ARGC, CHAR **ARGV)
//ENTRADAS: (LEÍDAS POR CONSOLA)  
//DESCRIPCIÓN: SE LEEN LAS ENTRADAS POR CONSOLA  
//SALIDA: 
//		INT: 1 EN CASO DE AUSENCIA DE ERRORES
int Leer_entradas(int argc, char **argv){
	archivo_entrada = (char *)(argv[1]); 								//archivo con la instancia del problema									
	archivo_salida = (char *)(argv[2]);									//archivo con la salida del problema (mejor solución encontrada)
	return 1;
}

//FUNCIÓN: ESCRITURA()
//ENTRADAS: NINGUNA
//DESCRIPCIÓN: ESCRITURA DE ARCHIVO TXT DE SALIDA 
//SALIDA: NINGUNA
void escritura(){
	ofstream writtt;
	writtt.open(archivo_salida);
	int t_max = 0;
	vector<int> buses_max;
    int b=0; 																		  
    while(b < (int) q_buses){ 											//Se itera en el arreglo de buses
		if (BUSES[b].t_recorrido > t_max){
			t_max = BUSES[b].t_recorrido;
		}																		
		writtt << b+1 << " " << BUSES[b].t_recorrido;					//escritura de la ruta 
		for(size_t i = 0; i != BUSES[b].path.size(); i++){
			if (i %2 ==0)
				writtt << " (" << BUSES[b].path[i]+1 << ", " ; 			//se escribe cada cliente
			else 
				writtt << BUSES[b].path[i]-q_puntos+1 << ") ";
		}
		writtt << "\n";
		b++;
    }
    
    for (int i=0; i< q_buses ; i++){									//Se detecta el/los buses de tiempo máximo/
		if (BUSES[i].t_recorrido == t_max){
			buses_max.push_back(i);
		}
	}
	
	for (size_t i=0; i!=buses_max.size();i++ ){							//Se escriben en el archivo de salida los buses de tiempo máximo
		writtt << buses_max[i]+1 << " ";
	}
	writtt << "\n";
	for (int i=0; i<q_albergues; i++){									//se obtiene la cantidad de evacuados en cada albergue
		int dif=0;
		dif= cap_original[i]-cap_albergues[i];
		writtt << dif << "\n";
	}
    writtt.close();
}

//*******************************************************************************************
//-------------------------FUNCIONES PARA FORWARD CHECKING-----------------------------------
//*******************************************************************************************

//FUNCIÓN: FORWARD_CHECK()
//ENTRADAS: NINGUNA  
//DESCRIPCIÓN: APLICA FORWARD CHECKING SOBRE EL SISTEMA PARA ENCONTRAR SOLUCIONES FACTIBLES AL PROBLEMA DE BEP A TRAVÉS DE LA 
//			   REDUCCIÓN DEL DOMINIO PARA CADA VARIABLE. 
//SALIDA: NINGUNA
void Forward_Check(){ 
	BUSES = new bus[q_buses];	
	vector<int> nodos_viajes;
	int n_total= q_puntos + q_albergues;	
	int iteracion =2;
	int t_time=0;
	int TMIN=MAX;
	
	for (int i=0; i < q_buses ; i++)
		inicializar_bus(&BUSES[i], cap_b, n_total);		
	for (int i=0; i<n_total; i++) 										//se arma el vector de nodos disponibles 
		nodos_viajes.push_back(i);
	nodos_viajes_bi=nodos_viajes;

	
	//DETECTA LA PRIMERA SOLUCIÓN DESDE PILA VACIA		
	while(existe_espera() or nodos_no_terminales()) {				
		for (int i=0; i<q_buses; i++){	
			bloque asig_pila;
			init_bloque(&asig_pila, n_total);
			asig_pila.en_espera2= new int[q_puntos];
			asig_pila.cap_albergues2= new int[q_albergues];			
			
			vector<int> domin;				
			
			if (BUSES[i].path.size() == 0){	
				domin= add_paths(& BUSES[i], nodos_viajes_bi);
			}
			else{
				domin= add_paths(& BUSES[i], nodos_viajes_bi);
			}
			if (domin.size() !=0 )	{
				//AGREGAR A LA PILA CADA UNA DE LAS ASIGNACIONES
				
				for (size_t j=0; j!= domin.size(); j++){
					asig_pila.dominio.push_back(domin[j]);
				}
				for (size_t j=0; j!= nodos_viajes_bi.size(); j++){
					asig_pila.nodos.push_back(nodos_viajes_bi[j]);
				}
				asig_pila.asignado=BUSES[i].path.back();
				for (int k=0; k< q_puntos; k++){
					asig_pila.en_espera2[k]= en_espera[k];
				}
				for (int k=0; k< q_albergues; k++){
					asig_pila.cap_albergues2[k]= cap_albergues[k];
				}				
				asig_pila.b_iesimo=i;
				asig_pila.evacuados=BUSES[i].pasajeros;
				pila_construct.push(asig_pila);
			}
		}
		iteracion+=1;
	}
											
	setear_tiempo_buses();												//escribe el tiempo de la primera solucion
	escritura(); 														//escribe la primera solucion
	for (int i=0; i< q_buses ; i++) {
		int tiempo_bus = BUSES[i].t_recorrido;
		if (tiempo_bus > t_time){										//busca el tiempo máximo de los buses de la solución 
			t_time=BUSES[i].t_recorrido;
		}	
	}	
	TMIN=t_time;
	
		
	int iteraciones=1;		
	//BUSCA SOLUCIONES MIENTRAS NO HAYA TERMINADO DE BUSCAR EN TODO EL ESPACIO
	while(!terminar){ 
		//-------LIMPIAR------//
		
		//if (iteraciones == 100-1 ){  									//DESCOMENTAR BLOQUE SI SE DESEA HACER SEGUIMIENTO DEL TIEMPO DE EJECUCIÓN PARA 'X' ITERACIONES
		//	terminar=true;
		//	break;
		//}
		int nuevo;
		bool continuar=true;
		int buss;	
		bloque poped;
		init_bloque(&poped, (q_puntos+q_albergues));
		while(!pila_construct.empty() and continuar ){
			if (pila_construct.empty())
				terminar=true;
			poped=pila_construct.top();
			if (poped.dominio.size()<2){
				BUSES[poped.b_iesimo].path.erase(BUSES[poped.b_iesimo].path.end()-1);
				pila_construct.pop();			
			} 
			else {
				pila_construct.pop();
				restablecer_sistema(poped);						
				continuar = false;
				int actual= poped.asignado;
				for (size_t k=0; k!=poped.dominio.size(); k++){
					if (poped.dominio[k] != actual){
						nuevo= poped.dominio[k];
						break;
					}
				}				
				poped.asignado=nuevo;
				BUSES[poped.b_iesimo].path.erase(BUSES[poped.b_iesimo].path.end()-1);
				BUSES[poped.b_iesimo].path.push_back(poped.asignado);
				//REESTABLECER NÚMERO DE PERSONAS
				if (nuevo < q_puntos){
					en_espera[nuevo]-= poped.evacuados;
					if (en_espera[nuevo]==0){
						//ELIMINAR
						nodos_viajes_bi.erase(remove(nodos_viajes_bi.begin(), nodos_viajes_bi.end(), nuevo), nodos_viajes_bi.end()); 
					}
				}
				else{
					cap_albergues[nuevo-q_puntos]-= poped.evacuados;					
					if (cap_albergues[nuevo-q_puntos]==0){
						//ELIMINAR
						nodos_viajes_bi.erase(remove(nodos_viajes_bi.begin(), nodos_viajes_bi.end(), nuevo), nodos_viajes_bi.end());
					}
				}				
				poped.dominio.erase(remove(poped.dominio.begin(), poped.dominio.end(), actual), poped.dominio.end());
				pila_construct.push(poped);
			}
		}
		if (pila_construct.empty()){
			terminar=true;
			break;
		}
		buss = poped.b_iesimo+1;
		//-------LIMPIAR------//

		//------EJECUTAR-------//
		while(existe_espera() or nodos_no_terminales()) {			
			for (int i=buss; i<q_buses; i++){	
				bloque asig_pila;
				init_bloque(&asig_pila, n_total);
				asig_pila.en_espera2= new int[q_puntos];
				asig_pila.cap_albergues2= new int[q_albergues];			
				vector<int> domin;				
				
				if (BUSES[i].path.size() == 0){
					domin= add_paths(& BUSES[i], nodos_viajes_bi);
				}
				else{
					domin= add_paths(& BUSES[i], nodos_viajes_bi);
				}
				if (domin.size() !=0 )	{
					//AGREGAR A LA PILA CADA UNA DE LAS ASIGNACIONES
					for (size_t j=0; j!= domin.size(); j++){
						asig_pila.dominio.push_back(domin[j]);
					}
					for (size_t j=0; j!= nodos_viajes_bi.size(); j++){
						asig_pila.nodos.push_back(nodos_viajes_bi[j]);
					}
					asig_pila.asignado=BUSES[i].path.back();
					for (int k=0; k< q_puntos; k++){
						asig_pila.en_espera2[k]= en_espera[k];
					}
					for (int k=0; k< q_albergues; k++){
						asig_pila.cap_albergues2[k]= cap_albergues[k];
					}
					
					asig_pila.b_iesimo=i;
					asig_pila.evacuados=BUSES[i].pasajeros;
					pila_construct.push(asig_pila);
				}
			}
			buss = 0;
		}
		setear_tiempo_buses();
		
		//ESCRIBIR EL RESULTADO DE LA RUTA MÁS ÓPTIMA HASTA EL MOMENTO 
		t_time =0; 														//resetea tiempo de buses
		for (int i=0; i< q_buses ; i++) {
			int tiempo_bus = BUSES[i].t_recorrido;
			if (tiempo_bus > t_time){									//busca el tiempo máximo de los buses de la solución 
				t_time=BUSES[i].t_recorrido;
			}	
		}	
		if (t_time < TMIN){ 											//el menor de los máximos
			TMIN = t_time;
			escritura();
			cout << "ATENCIÓN! Se detecta nuevo tiempo óptimo "<< t_time << endl;
		}		
		iteraciones++;
	}
	cout << "PROCESO EJECUTADO CON " << iteraciones+1 << " ITERACIONES." << endl;
	cout << "LA MEJOR SOLUCIÓN INDICA QUE LA EVACUACIÓN SE COMPLETA EN " << TMIN << " UNIDADES DE TIEMPO" << endl;
}

//FUNCIÓN: EXISTE_ESPERA()
//ENTRADAS: NINGUNA  
//DESCRIPCIÓN: VERIFICA SI AUN QUEDAN PERSONAS POR EVACUAR EN EL SISTEMA
//SALIDA: 
//		BOOL: INDICA 1 EN EL CASO DE QUE AÚN EXISTAN PERSONAS A EVACUAR EN NODOS TIPO 'PUNTO DE ENCUENTRO' Y 0 EN EL CASO CONTRARIO.
bool existe_espera() {										
	int personas=0;																	
	for (int i=0; i<q_puntos;i++){
		personas += en_espera[i];
		if (personas != 0)
			return true;
	}	
	return false;
}

//FUNCIÓN: NODOS_NO_TERMINALES()
//ENTRADAS: NINGUNA  
//DESCRIPCIÓN: VERIFICA QUE TODOS LOS BUSES HAYAN TERMINADO SU RECORRIDO EN NODOS TIPO 'ALBERGUE'
//SALIDA: 
//		BOOL: INDICA 1 EN EL CASO DE QUE NO TODOS LOS BUSES HAYAN TERMINADO SU RECORRIDO EN ALBERGUES Y 0 EN EL CASO CONTRARIO,
//			  ES DECIR, SI TODOS LOS BUSES ESTÁN EN ALGÚN ALBERGUE
bool nodos_no_terminales(){
	int no_cumplen=0;
	if (existe_espera()){
		return true;
	}
	else {
		for (int i=0; i<q_buses ; i++){
				if (BUSES[i].path.back() < q_puntos)
					no_cumplen++;
		}
	}	
	if (no_cumplen ==0) 
		return false;
	else 
		return true;
}

//FUNCIÓN: RESTABLECER_SISTEMA(BLOQUE &POPED)
//ENTRADAS: 
//		BLOQUE &POPED: ESTRUCTURA TIPO BLOQUE QUE CONTIENE LOS DATOS NECESARIOS PARA RECONFIGURAR LOS NODOS DISPONIBLES,
//					   CAPACIDAD DE ALBERGURS Y NÚMERO DE PERSONAS EN ESPERA
//DESCRIPCIÓN: SE RECONFIGURA EL SISTEMA PARA VOLVER A UN INSTANTE PREVIO CON SU RESPECTIVO ESTADO
//SALIDA: NINGUNA
void restablecer_sistema(bloque &poped){
	int zero;
	for (int i=0; i<q_puntos; i++){
		en_espera[i]=poped.en_espera2[i];
	}
	for (int i=0; i<q_albergues; i++){
		cap_albergues[i]=poped.cap_albergues2[i];
	}
	nodos_viajes_bi.erase(nodos_viajes_bi.begin(), nodos_viajes_bi.end());
	
	for (size_t i=0; i!=poped.nodos.size(); i++){
		nodos_viajes_bi.push_back(poped.nodos[i]);
	}
	if (poped.asignado < q_puntos){		
		zero= en_espera[poped.asignado];							
		en_espera[poped.asignado] += poped.evacuados;
		if (zero ==0 and en_espera[poped.asignado]!=0)
			nodos_viajes_bi.push_back(poped.asignado);
	}
	else{
		zero= cap_albergues[poped.asignado-q_puntos];
		cap_albergues[poped.asignado-q_puntos] += poped.evacuados;
		if (zero ==0 and cap_albergues[poped.asignado-q_puntos]!=0)
			nodos_viajes_bi.push_back(poped.asignado);
	}
	sort(nodos_viajes_bi.begin(), nodos_viajes_bi.end());
}

//FUNCIÓN: DET_DEPOSITO(INT I)
//ENTRADAS: 
//		INT I: ENTERO QUE IDENTIFICA AL BUS I-ÉSIMO
//DESCRIPCIÓN: DETERMINAR EL DEPÓSITO DESDE DONDE INICIÓ EL BUS I-ÉSIMO SU RECORRIDO
//SALIDA: 
//		INT : ENTERO QUE IDENTIFICA AL DEPÓSITO I-ÉSIMO DEL BUS EN CUESTIÓN 
int det_deposito(int i){
	int cantidad=0;
	for (int j=0; j<q_dep;j++){
		cantidad+=buses_dep[j];
		if (cantidad>i)
			return j;
	}
	return cantidad;
}

//FUNCIÓN: ADD_PATHS(BUS *BUS, VECTOR<INT> &NODOS_VIAJES_BI)
//ENTRADAS: 
//		BUS *BUS: ARREGLO DE TIPO BUS
//		VECTOR<INT> &NODOS_VIAJES_BI: VECTOR CON LOS NODOS DISPONIBLES A VISITAR
//DESCRIPCIÓN: ASIGNA SECUENCIALMENTE (A UN DETERMINADO BUS) EL SIGUIENTE NODO A VISITAR SEGUN LA DISPONIBILIDAD DE ÉSTE
//SALIDA: 
//		VECTOR<INT>: NODOS DISPONIBLES A VISITAR EN EL ACTUAL VIAJE
vector<int> add_paths(bus *BUS, vector<int> &nodos_viajes_bi){
	vector <int> dom;
	if (BUS->path.size() % 2 == 0)	{								
		for(size_t i = 0; i<nodos_viajes_bi.size() ; i++){ 	
			if( nodos_viajes_bi[i] < q_puntos )
				dom.push_back(nodos_viajes_bi[i]);
		}		
	}
	else{																
		for(size_t i = 0; i<nodos_viajes_bi.size() ; i++){ 
			if( nodos_viajes_bi[i] >= q_puntos )
				dom.push_back(nodos_viajes_bi[i]);
		}
	}
	if (dom.size()!=0){	
		//ASIGNAR Y REDUCIR PERSONAS EN ESPERA EN NODO CORRESPONDIENTE
		if (BUS->path.size() % 2 == 0) {								
			if (en_espera[dom.front()]> BUS->capacidad){
				en_espera[dom.front()]-= BUS->capacidad;
				BUS->pasajeros = BUS->capacidad;
			}
			else{
				BUS->pasajeros =en_espera[dom.front()];
				en_espera[dom.front()]=0;
				nodos_viajes_bi.erase(remove(nodos_viajes_bi.begin(), nodos_viajes_bi.end(), dom.front()), nodos_viajes_bi.end());
			}
		}
		else {
			if (cap_albergues[dom.front()-q_puntos]> BUS->capacidad){
				cap_albergues[dom.front()-q_puntos]-= BUS->capacidad;
				BUS->pasajeros = BUS->capacidad;
			}
			else{
				BUS->pasajeros =cap_albergues[dom.front()-q_puntos];
				cap_albergues[dom.front()-q_puntos]=0;					//eliminar el nodo evacuado de nodos_viajes
				nodos_viajes_bi.erase(remove(nodos_viajes_bi.begin(), nodos_viajes_bi.end(), dom.front()), nodos_viajes_bi.end());
			}	
		}	
		BUS->path.push_back(dom.front());
	}
	return dom;
}

//FUNCIÓN: SETEAR_TIEMPO_BUSES()
//ENTRADAS: NINGUNA
//DESCRIPCIÓN: CONFIGURA EL ATRIBUTI T_RECORRIDO DE CADA UNO DE LOS BUSES EN BASE A LA RUTA SEGUIDA POR ESTE 
//SALIDA: NINGUNA
void setear_tiempo_buses(){
	for (int i=0; i<q_buses ; i++){		
		//distancia inicial (dist_ini): desde deposito i-ésimo a primer punto de encuentro visitado
		int dist = dist_ini[det_deposito(i)][BUSES[i].path[0]];
		int antes= BUSES[i].path[0];
		int tiempo = dist;
		for (size_t j = 1; j!= BUSES[i].path.size() ; j++){
			int dist = dist_bi[antes][BUSES[i].path[j]];
			antes= BUSES[i].path[j];
			tiempo += dist; 	
		}
		BUSES[i].t_recorrido = tiempo;
	}
}
