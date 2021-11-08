/***************************************************
*Luciernagas
*Ultima version:
*	7 de noviembre 2021 
*
*Saul Otero Garcia
***************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
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

double beta0=0;
double gamma1=1;
//Funcion que se va a optimizar
int que_funcion=1;
		/*1: funcion esfera (Por defecto)
		  2: funcion Otra
		  */	

double ALEATORIO(double min, double max)
{
double numero;
numero = (drand48() * (max-min)) + min;
if(numero>max) numero=max;
if(numero<min) numero=min;
	return numero;
} 
/**
 * Función esfera (una funcion que queremos optimizar)
 * 
 * PARÁMETROS:
 * 	una_solucion: un vector del espacio de solucion del problema para el que queremos calcular el valor de la fuincion esfera
 * 	(SE SUPONE QUE HE PASADO UNA SOLUCIÓN FACTIBLE)
 *	
 * RETORNO:
 *	valor de la funcion esfera para el parametro pasado 
 */
double funcion_esfera(double una_solucion[R])
{
	int i;
double suma=0; //Variable del almacenaje sumatorio
	for(i=0;i<R;i++){
		suma += una_solucion[i]*una_solucion[i];
	}
		
	return suma;
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
				
				break;
			default:
				printf("\nFUNCIÓN OBJETIVO NO DEFINIDA");
				printf(" [%d], Revidsa código\n", que_funcion);
				exit(1);
		}//switch
//retorno el fitness que acabod e calcular
return un_fitness;

}
/**
 * Calcular el fitness o calidad de cada partícula
 * 
 * PARÁMETROS:
 * 	lobos: array que contiene las posiciones de las particulas de la manada
 * 	fitness: fitness calculado para cada individuo i
 * 		fitness[i] -> calidad de la solución del individuo i
 * 			(esa solución está almacenada en lobos[i])
 */
void calcular_fitness(double luciernagas[P][R],double fitness[P])
{
int i;
	for ( i = 0; i < P; i++)
	{
		fitness[i]=seleccionar_funcion_fitness(luciernagas[i]);
		
	}
	
}


void iniciar_luciernagas(double luciernagas[P][R], double g[R]){
    int i,j;
	int particula_OPT; //una fila del array de mejores posiciones de las particulas(entre 0 y P-1)
double fitness_OPT;
double otro_fitness; //
    for(i=0;i<P;i++){   
        for(j=0;j<R;j++){
           luciernagas[i][j]=ALEATORIO(pos_min,pos_max);
        }
    }
		//tomo la primera particula como la mejor inicial para determinar el valor de g
	//--guardo su fitness
	fitness_OPT = seleccionar_funcion_fitness(luciernagas [0]);
	//--guardo su posicion en el array de posiciones
	particula_OPT=0;
	for(i=1;i<P;i++)
	{
		otro_fitness=seleccionar_funcion_fitness(luciernagas[i]);
		//si la particula i-esima es mejor que la que he considerado como optima hasta el momento la tomo como nueva particula optima
		if(otro_fitness<fitness_OPT){
			fitness_OPT =otro_fitness;
			particula_OPT=i;
		}
	}
	//ahora valor a g
	for(j=0;j<R;j++)
	{
		g[j]=luciernagas[particula_OPT][j];
	}
}

void determinar_orden(double luciernagas[P][R],double fitness[P],double g[R]) 
{	
	int i,j,k;
	double aux,aux1;
	//Se recorre el vector el número de veces igual al tamaño de este
	for (j = 0; j < P; ++j)
	{
		//recorremos cada valor del vector
		for (i = 0; i < (-1)-j; ++i)
		{
			//Comparamos ese valor con el siguiente, si es mayor que el siguiente se intercambian usando una varible auxiliar
			if (fitness[i]>fitness[i+1])
			{
				aux=fitness[i];
				fitness[i]=fitness[i+1];
				fitness[i+1]=aux;
				for(k=0;k<R;k++){
					aux1=luciernagas[i][k];
					luciernagas[i][k]=luciernagas[i+1][k];
					luciernagas[i+1][k]=aux1;
				}
			}
		}
	}
	for(i=0;i<P; i++)
	{
			//Si su mejor posicion local (o mejor solucion encontrada hasta el momento) es mejor que la solucion asociada al enjambre, la tomo como la nueva solucion del enjambre
			//Esto solo funciona si se minimiza(>)
			if(seleccionar_funcion_fitness(g)>seleccionar_funcion_fitness(luciernagas[i]))
			{ 
				//copio la mejor posicion local de la particula i-esima (copio  b[i] como nuevo valor de g)
				for(j=0; j<R; j++)
				{
					g[j]=luciernagas[i][j];
				}
			}
	}
}


//se mueven todas menos la mejor, la mejor es la posicion 0 y su brillo en la posicion 0 de fitness
void mover_brillante(double luciernagas[P][R]){
double distancia,diferencia[R];
double beta;
double intermedio[R];//se almacenan los valores intermedios del aformula
int i,j,k,dim,count;
double resta,cuenta;
double multi,multi2;


for(i=(P-1);i>0;i--){
    distancia=0;
    for(dim=0;dim<R;dim++){
           diferencia[dim]=0;
        }
	count=i;
    for(k=count-1;k>=0;k--){

        
        for(dim=0;dim<R;dim++){
			resta=luciernagas[i][dim]-luciernagas[k][dim];
			multi=resta*resta;
            distancia+=resta;
            diferencia[dim]= luciernagas[k][dim]-luciernagas[i][dim];
        }
		cuenta=-gamma1*distancia;
		multi2=cuenta*cuenta;
        beta= beta0 * multi2;
        for(dim=0;dim<R;dim++){
            intermedio[dim]=beta * diferencia[dim];
        }
        
    }


    for(j=R;j>0;j--){
        luciernagas[i][j]=luciernagas[i][j]+intermedio[i]+ALEATORIO(-0.5,0.5);
    }
	
}


}
void mover_mas_brillante(double luciernagas[P][R]){
    int i;
    for(i=0;i<R;i++)
    luciernagas[0][i]=luciernagas[0][i]+ALEATORIO(-0.5,0.5);
}
void mostrar_vector(double vector [R])
{
	int i;
	printf("\n POSICION SOLUCION:\n");
	for(i=0;i<R;i++){
		
		printf("\t%.3lf\n",vector[i]);
	}
	printf("\nfitness -> %f\n",seleccionar_funcion_fitness(vector));
}


int main()
{

double luciernagas[P][R];
double fitness[P],g[R];
int t;
srand48(time(NULL));
iniciar_luciernagas(luciernagas,g);
calcular_fitness(luciernagas,fitness);
determinar_orden(luciernagas,fitness,g);

for(t=0;t<t_max;t++){

mover_brillante(luciernagas);
mover_mas_brillante(luciernagas);
calcular_fitness(luciernagas,fitness);
determinar_orden(luciernagas,fitness,g);
}
printf("\nSOLUCIÓN DEL PROBLEMA: \n");
	mostrar_vector(g);

	

return 0;
}