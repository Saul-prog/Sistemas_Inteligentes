/***************************************************
*PSO
*Ultima version:
*	28 de septiembre 2021 
*
*Saul Otero Garcia
***************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//Constantes
#define R 4 //Numero de dimensiones del espacio de solucion
#define P 10 //Numero de individuos del enjambre




//Variables globales
int t_max =20;/*Numero máximo de iteraciones de un algoritmo*/

/********VARIABLES ESPECIFICAS DE PSO************/
// intervalo valido para las componentes de una posicion
//la componente puede tomar valores en [pos_min,pos_max]
double pos_min = -5.12,
       pos_max =5.12;
// intervalo valido para las componentes de una velocidad
//la componente puede tomar valores en [v_min,v_max]
double v_min = -4,
	   v_max= 4;

//Funcion que se va a optimizar
int que_funcion=1;
		/*1: funcion esfera (Por defecto)
		  2: funcion Otra
		  */	   
//parametros de PSO para actualizar la velocidad
double w = 0.729;//inercia de pso
double f1= 2.05; //componente cognitiva de pso
double f2= 2.05; //componenete social		  
/************************************************/

/***PROTOTIPOS****/
double ALEATORIO(double min, double max);
void iniciar_enjambre(double x[P][R],double v[P][R],double b[P][R], double g[R]);


void PSO();
double seleccionar_funcion_fitness(double posicion[R]);
double funcion_esfera(double una_solucion[R]);
void calcular_fitnes(double x[P][R], double fitness[P]);
void actualizar_g(double g[R],double b[P][R]);
void actualizar_bi(double enjambre_posicion[P][R],double fitness[P],double enjambre_posOptima[P][R]);
void acualizar_xi(double x[P][R],double v[P][R]);
/**************************************************/

/*******************************************
 * Generar un real en un intervalo dado
 * Parametros
 * 		min: limite inferior
 * 		max: limite superior
 * Retorno
 *  numero generado
*/
double ALEATORIO(double min, double max)
{
double numero;
numero = (drand48() * (max-min)) + min;
if(numero>max) numero=max;
if(numero<min) numero=min;
	return numero;
} 



/*
*Inicializar la poblaciond e particuals. Para cada particula
* -la posicion: aleatorio en un intervalo dado
* -la velocidad: aleatorio en un intervalo dado
* -la mjeor poscicion personal: igual a la posicion inicial
*Tambien doy valor inicial a la mejor solucion del enjambre
*(seria la mejor solucion del conjunto inicial definido en esta funcion)
*Parametros:
*	x:	array que contiene las posiciones de las particulas del enjambre
*						x[i]->posicion de la particula i   
*						x[i][j]->componente j de la posicion de la particula i (j entre 0 y R-1)
*   v: array que contiene las velocidades de las particulas del enjambre
*						v[i]->posicion de la particula i   
*						v[i][j]->componente j de la posicion de la particula i (j entre 0 y R-1)
*   b: array que contiene la mejor posicion personal de cada aprticula
*/
void iniciar_enjambre(double x[P][R],double v[P][R],double b[P][R], double g[R])
{
int i,j;
double fitness_mejor[P];//variable para determinar 
int particula_OPT; //una fila del array de mejores posiciones de las particulas(entre 0 y P-1)
double fitness_OPT;
double otro_fitness; //
	for(i=0;i<P;i++)//para cada particula
	{
		for(j=0;j<R;j++)//para cada componente de una aprticula
		{
			//dar a una componente del vector posicion del la particula i
			x[i][j]=ALEATORIO(pos_min,pos_max);
			/*Tomo la posion inicial de la particula como su mejor posicion hasta el momento*/
			b[i][j]=x[i][j];
			//dar a una componente del vector velocidad del la particula i
			v[i][j]=ALEATORIO(v_min,v_max);
		}
	
	}

	//tomo la primera particula como la mejor inicial para determinar el valor de g
	//--guardo su fitness
	fitness_OPT = seleccionar_funcion_fitness(b [0]);
	//--guardo su posicion en el array de posiciones
	particula_OPT=0;
	//para cada particula  apartir de la segunda
	for(i=1;i<P;i++)
	{
		otro_fitness=seleccionar_funcion_fitness(b[i]);
		//si la particula i-esima es mejor que la que he considerado como optima hasta el momento la tomo como nueva particula optima
		if(otro_fitness<fitness_OPT){
			fitness_OPT =otro_fitness;
			particula_OPT=i;
		}
	}
	//ahora valor a g
	for(j=0;j<R;j++)
	{
		g[j]=b[particula_OPT][j];
	}
}

/*
*Funcion esfera (una funcion que queremos optimizar)
*Parametros:
*	una_solucion: un vector del espacion de solucion del problema para el que queremos calcular el valor de la funcion esfera
*   (se supone que he pasado una posicion factible)
*Retorno:
*    valor de la funcion esfera para el parametro pasado
*/

double funcion_esfera(double una_solucion[R])
{
int i;
double suma=0; //Variable del almacenaje sumatorio
	for(i=0;i<R;i++)
	{
		suma += (una_solucion[i]*una_solucion[i]);
	}
	return suma;
}

//Una segunda funcion apra probar mi programa
double funcion_Otra(double una_solucion[R])
{
	
}
double seleccionar_funcion_fitness(double posicion[R])
{
double un_fitness; //Fitness de la posicion pasada como parametro
switch(que_funcion)
		{
			case 1: //esfera
				un_fitness=funcion_esfera(posicion);
				break;
			case 2: //Otra funcion
				un_fitness=funcion_Otra(posicion);
				break;
			default:
				printf("\nFUNCIÓN OBJETIVO NO DEFINIDA");
				printf(" [%d], Revidsa código\n", que_funcion);
				exit(1);
		}//switch
//retorno el fitness que acabod e calcular
return un_fitness;

}

/*
*Calcular el fitnes o calidad de cada particula
*Parametros:
*	enjambre_posicion: array que contiene las posiciones de las aprticulas del enjambre
*   fitness: fitness calculado para cada individuo
*            fitnes[i]-> calidad de la solucion del individuo i(esa solucion esta almacenada en x[i])
*Retorno:
*    valor de la funcion esfera para el parametro pasado
*/
void calcular_fitnes(double x[P][R], double fitness[P])
{
int i;
	for ( i = 0; i < P; i++)
	{
		fitness[i]=seleccionar_funcion_fitness(x[i]);
		
	}
	
}

/**
*Actualizar la mejor solucion del enjambre
*(la solucion del problema)
*
*Parametros
* g:mejor posicon(=igual solucion) encontrada hasta el momento por el enjambre
* b: mejor posicion de cada particula hasta cada momento
*/
void actualizar_g(double g[R],double b[P][R])
{
int i,j; //contador
	//para cada particula
		for(i=0;i<P; i++)
		{
				//Si su mejor posicion local (o mejor solucion encontrada hasta el momento) es mejor que la solucion asociada al enjambre, la tomo como la nueva solucion del enjambre
				//Esto solo funciona si se minimiza(>)
				if(seleccionar_funcion_fitness(g)>seleccionar_funcion_fitness(b[i]))
				{ 
					//copio la mejor posicion local de la particula i-esima (copio  b[i] como nuevo valor de g)
					for(j=0; j<R; j++)
					{
						g[j]=b[i][j];
					}
				}
		}
}




/*
*Actualiza la mejor posicion personal de cada particula
*Parametros:
*	enjambre_posicion: array que contiene las posiciones de las aprticulas del enjambre
*   fitness: fitness de cada fila
*    enjambre_posOptima: mejor posicion de cada particula hasta el momento       
*Retorno:
*    valor de la funcion esfera para el parametro pasado
*/
void actualizar_bi(double enjambre_posicion[P][R],double fitness[P],double enjambre_posOptima[P][R])
{
int i,j;//Contadores
//Para cada particula
	for ( i = 0; i < P; i++)
	{
		//Falta hacer maximizaciones
		//MINIMIZAR se usa <
		if(fitness[i] < seleccionar_funcion_fitness(enjambre_posOptima[i]))
		{
			for(j=0;j<R;j++)
			{
				enjambre_posOptima[i][j]=enjambre_posicion[i][j];
			}
		}
	}
	
}

/**
*Funcion que actualiza la funciond e ca
*Parametros:
*	x:	array que contiene las posiciones de las particulas del enjambre
*						x[i]->veloidad de la particula i   
*						x[i][j]->componente j de la velocidad de la particula i (j entre 0 y R-1)
*   v: array que contiene las velocidades de las particulas del enjambre
*						v[i]->velocidad de la particula i   
*						v[i][j]->componente j de la velocidad de la particula i (j entre 0 y R-1)
*/
void acualizar_xi(double x[P][R],double v[P][R])
{
int i,j;
	for(i=0;i<P;i++)
	{
		for(j=0;j<R;j++)
		{	
			//actualizo la posicion
			x[i][j]+= v[i][j];
			//se ajusta el nuevo valor para que entre en el intervalo
			if(x[i][j]>pos_max)
			{
				x[i][j]=pos_max;
			}
			if(x[i][j]<pos_min)
			{
				x[i][j]=pos_min;
			}
		}
	}
}
void actualizar_vi(double x[P][R], double v [P][R],double b[P][R],double g[R])
{
	int i,j;//Contador
	//falta por definir w, f1,f2

	for(i=0;i<P;i++)
	{
		for(j=0;j<R;j++)
		{
			v[i][j]=w*v[i][j] + f1*ALEATORIO(0,1)*(b[i][j]-x[i][j]) + f2*ALEATORIO(0,1)*(g[j]-x[i][j]);
			//Tras actualizar la velocidad de la particula debo ajustar el nuevo valor al intervalo valido definido por [v_min,v_max]
			if(v[i][j]>v_max)
			{
				v[i][j]=v_max;
			}
			if(v[i][j]<v_min)
			{
				v[i][j]=v_min;
			}
				
		}
	}
}

void mostrar_vector(double vector [R])
{
	int i;
	printf("\nPOSICION SOLUCION:\n");
	for(i=0;i<R;i++)
		printf("\t%.3lf\n",vector[i]);
	printf("\tfitness -> %lf\n",seleccionar_funcion_fitness(vector));
	
}
void mostrar_posiciones_actuales(double x[P][R])
{
int i;
for(i=0;i<P;i++)

	mostrar_vector(x[i]);
}


void PSO()
{
int t; //Contador de iteraciones del algoritmo

//Variables para almacenar los datos de las particulas
double enjambre_posicion[P][R];	//x
								//pisicion de las particulas
								//enjambre_posicion[i]->posicion de la particula i
								//enjambre_posicion[i][j]->componente j de la posicion de la particula i (j entre 0 y R-1)

double enjambre_velocidad[P][R];//v 
double enjambre_posOptima[P][R];//b
//fitness de las posiciones de las particulas que estan almacenadas en el array enjambre_posicion
double fitness[P];
//mejor posicion global 
double g[R];


	//iniciar la poblacion de particulas 
	iniciar_enjambre(enjambre_posicion,enjambre_velocidad,enjambre_posOptima,g);
	//Se muestra el valor inicial y su fitness
	mostrar_posiciones_actuales(enjambre_posicion);
	//Bucle principal del algoritmo
	
	for(t=0; t< t_max; t++)
	{
		//calcular el fitness de cada particula
		calcular_fitnes(enjambre_posicion,fitness); 
		//Actualizar la mejor solucion personal de cada particula
		//la primera iteracion de enjambre_posOptima es una indeterminacion
		actualizar_bi(enjambre_posicion,fitness,enjambre_posOptima);
		//Actualizar la mejor solucion global
		
		actualizar_g(g, enjambre_posOptima);
		//Atualizar la velocidad de cada particula
		actualizar_vi(enjambre_posicion,enjambre_velocidad,enjambre_posOptima,g);
		//Atualizar la posicion de cada particula
		acualizar_xi(enjambre_posicion,enjambre_velocidad);
	}
	
	//mostrar resultado final
	mostrar_vector(g);
}


int main()
{
srand48(time(NULL));

	PSO();

return 0;

	


}
