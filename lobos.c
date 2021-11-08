#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define que_funcion 1
#define P 10
#define R 4
int t_max =100;/*Numero máximo de iteraciones de un algoritmo*/

double pos_min = -5.12,
       pos_max =5.12;

double seleccionar_funcion_fitness(double posicion[R]);
void calcular_fitness(double lobos[P][R],double fitness[P]);
double ALEATORIO(double min, double max);
double funcion_esfera(double una_solucion[R]);
void iniciar_lobos(double lobos[P][R], double g[R]);
/**
 * Genera numeros aleatorios en el intervalo
 * 
 *
 * PARÁMETROS:
 * 	min: valor menor del intervalo
 *  max: valor mayor del intervalo
 */
double ALEATORIO(double min, double max)
{
double numero;
numero = drand48() * (max-min) + min;
//comprbar
if(numero>max){
 numero=max;
}
if(numero<min){ 
numero=min;
}

	return numero;
} 

/**
 * Inicicializar la población de lobos. Para cada lobo:
 * 	-la posición: aleatoria de un intervalo dado
 *
 * PARÁMETROS:
 * 	lobos: array que contiene las posiciones de los lobos de la manada
 * 		lobos[i] -> posicion del lobo i (entre 0 y P-1)
 *		lobos[i][j] -> componente j de la posición del lobo i (j entre 0 y R-1)
 */
void iniciar_lobos(double lobos[P][R], double g[R]){
int i,j;
int particula_OPT; //una fila del array de mejores posiciones de las particulas(entre 0 y P-1)
double fitness_OPT;
double otro_fitness; //
	for(i=0;i<P;i++){
		for(j=0;j<R;j++){
			lobos[i][j]=ALEATORIO(pos_min,pos_max);
		}
	}
	//tomo la primera particula como la mejor inicial para determinar el valor de g
	//--guardo su fitness
	fitness_OPT = seleccionar_funcion_fitness(lobos [0]);
	//--guardo su posicion en el array de posiciones
	particula_OPT=0;
	for(i=1;i<P;i++)
	{
		otro_fitness=seleccionar_funcion_fitness(lobos[i]);
		//si la particula i-esima es mejor que la que he considerado como optima hasta el momento la tomo como nueva particula optima
		if(otro_fitness<fitness_OPT){
			fitness_OPT =otro_fitness;
			particula_OPT=i;
		}
	}
	//ahora valor a g
	for(j=0;j<R;j++)
	{
		g[j]=lobos[particula_OPT][j];
	}
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
void calcular_fitness(double lobos[P][R],double fitness[P])
{
int i;
	for ( i = 0; i < P; i++)
	{
		fitness[i]=seleccionar_funcion_fitness(lobos[i]);
		
	}
	
}
/**
*Ordenar los lobos
* de mayor a menor
*PARÁMETROS:
 * 	lobos: array que contiene las posiciones de los lobos del enjambre
 * 		lobos[i] -> posicion del lobo i (entre 0 y P-1)
 *		lobos[i][j] -> componente j de la posición del lobo i (j entre 0 y R-1)
 *	fitness: fitness calculado para cada individuo i
 * 		fitness[i] -> calidad de la solución del individuo i
 * 			(esa solución está almacenada en lobos[i])
*/
void determinar_orden(double lobos[P][R],double fitness[P], double g[R]) 
{	
	int i,j,k;
	double aux,aux1;
	//Se recorre el vector el número de veces igual al tamaño de este
	for (j = 0; j < P; ++j)
	{
		//recorremos cada valor del vector
		for (i = 0; i < (-1)-j; ++i)
		{
			//Comparamos ese valor con el siguiente, si es menor que el siguiente se intercambian usando una varible auxiliar
			if (fitness[i]<fitness[i+1])
			{
				aux=fitness[i];
				fitness[i]=fitness[i+1];
				fitness[i+1]=aux;
				for(k=0;k<R;k++){
					aux1=lobos[i][k];
					lobos[i][k]=lobos[i+1][k];
					lobos[i+1][k]=aux1;
				}

			}
		}
	}
		for(i=0;i<P; i++)
		{
				//Si su mejor posicion local (o mejor solucion encontrada hasta el momento) es mejor que la solucion asociada al enjambre, la tomo como la nueva solucion del enjambre
				//Esto solo funciona si se minimiza(>)
				if(seleccionar_funcion_fitness(g)>seleccionar_funcion_fitness(lobos[i]))
				{ 
					//copio la mejor posicion local de la particula i-esima (copio  b[i] como nuevo valor de g)
					for(j=0; j<R; j++)
					{
						g[j]=lobos[i][j];
					}
				}
		}
}



/**
*Actualizar los lobos
*
*PARÁMETROS:
 * 	lobos: array que contiene las posiciones de los lobos del enjambre
 * 		lobos[i] -> posicion del lobo i (entre 0 y P-1)
 *		lobos[i][j] -> componente j de la posición del lobo i (j entre 0 y R-1)
 *	a: factor de exploración
*/
void actualizar_posicion(double lobos[P][R],double a){
 
 double A1,A2,A3,C1,C2,C3,X1,X2,X3;
 double Da,Db,Dd;

 int i,j;
 double alfa[R],beta[R],delta[R];
 //GUardo las tres mejores posiciones de cada dimension en el instante t para el calculo de las nnuevas posiciones
 for(j=0;j<R;j++){
 	alfa[j]=lobos[0][j];
 	beta[j]=lobos[1][j];
 	delta[j]=lobos[2][j];
 	}
 
 
 
	 for(i=0;i<P;i++){
		 for(j=0;j<R;j++){
			A1=2*a*ALEATORIO(0,1)-a;
			C1=2*ALEATORIO(0,1);
			A2=2*a*ALEATORIO(0,1)-a;
			C2=2*ALEATORIO(0,1);
			A3=2*a*ALEATORIO(0,1)-a;	
			C3=2*ALEATORIO(0,1);
			Da=C1*alfa[j]-lobos[i][j];
			Db=C2*beta[j]-lobos[i][j];
			Dd=C3*delta[j]-lobos[i][j];
			if(Da<0) Da=Da*-1;
			if(Db<0) Db=Db*-1;
			if(Dd<0) Dd=Dd*-1;
			X1=alfa[j]-A1*Da;
			X2=beta[j]-A2*Db;
			X3=delta[j]-A3*Dd;
			lobos[i][j]=(X1+X2+X3)/3;
		 }
	}
}
void mostrar_vector(double vector [R])
{
	int i;
	printf("\n POSICION SOLUCION:\n");
	for(i=0;i<R;i++){
		
		printf("\t%.3lf\n",vector[i]);
	}
	printf("\nfitness -> %lf\n",seleccionar_funcion_fitness(vector));
}
void main(){
 double a =2,t=0;
 double lobos[P][R];
 double fitness[P];
 double g[R];
srand48(time(NULL));
 iniciar_lobos(lobos,g);
 calcular_fitness(lobos,fitness);
 determinar_orden(lobos,fitness,g);
 do{
 actualizar_posicion(lobos,a);
 
 a = 2 - ((2*t)/t_max);
 calcular_fitness(lobos,fitness);
 determinar_orden(lobos,fitness,g);
 
 t++;
 }while(a!=0);
mostrar_vector(g);

}
