#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//CONSTANTES
#define m 3
#define j_max 5
#define R 4  //número de dimensiones del espacio de solución SE LLAMA R AL TAMAÑO DE LA POBLACION
#define P m*ranas_por_grupo//número de individuos del enjambre 
#define ranas_por_grupo 3
/*----VARIABLES GLOBALES----*/
int t_max = 20; //número máximo de iteraciones de un algoritmo de enjambre

/*----VARIABLES ESPECÍFICAS PARA PSO----*/

//intervalo válido para los componentes de una posición
//(la componente puede tomar varios valores en [pos_min, pos_max])
double pos_min = -5.12,
	   pos_max =  5.12;

/*----VARIABLES ESPECÍFICAS PARA SFLA----*/
//int m; //numero de memeplexes
int Jmax= 10; //numero de iteraciones de mejora que se aplican a un memeplex
int Dmax=5;
//función que se va a optimizar
int que_funcion=1;







double ALEATORIO(double min, double max){
	double numero;
	numero = drand48() * (max-min) + min;

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
	int i; //contador
	double suma= 0; //var. para almacenar el resultado del sumatorio (inicialmente 0)

	for(i=0; i<R; i++)
	{
		suma += (una_solucion[i] * una_solucion[i]);

	}		
		
	return suma; 
}

/**
 * Calcular el fitness o calidad de cada partícula
 * 
 * PARÁMETROS:
 * 	x: array que contiene las posiciones de las particulas del enjambre
 * 	fitness: fitness calculado para cada individuo i
 * 		fitness[i] -> calidad de la solución del individuo i
 * 			(esa solución está almacenada en x[i])
 */


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
void calcular_fitness (double x[P][R], double fitness[P])
{
	int i; //contador	
	for(i=0; i<P; i++)
	{
		fitness[i]= seleccionar_funcion_fitness(x[i]);
	}
}






	/**
 * Inicicializar la población de ranas. Para cada rana:
 * 	-la posición: aleatoria de un intervalo dado
 *
 * PARÁMETROS:
 * 	x: array que contiene las posiciones de las ranas del enjambre
 * 		x[i] -> posicion de la rana i (entre 0 y P-1)
 *		x[i][j] -> componente j de la posición de la rana i (j entre 0 y R-1)
 *  g: mejor solución del enjambre
 */
void iniciar_enjambre(double x[P][R], double g[R]){
	int i, j; //contadores
	double fitness_mejor[P];
	
	//PROTOTIPOS 
	int particula_OPT; //una fila del array de mejores posiciones de las particulas (entre 0 y P-1)
	double fitness_OPT, otro_fit; //fitness de la rana

	for(i=0; i<P; i++) //para cada individuo (rana)
	{
		for(j=0; j<R; j++) //para cada componente de la posición de un individuo (rana)
		{
			//doy un valor a una componente del vector posición de la rana i
			x[i][j] = ALEATORIO (pos_min, pos_max);

			//tomo la primera rana como su mejor inicial para determinar el valor g
			//guardo su fitness
			fitness_OPT= seleccionar_funcion_fitness(x[0]);
			//guardo su posición 
			particula_OPT= 0;
		}

		//calculo el valor inicial de g (mejor solución global)
		fitness_mejor[i]= seleccionar_funcion_fitness(x[i]);
	}

	
	for(i=1; i<P; i++) //para cada particula a partir de la segunda
	{
		//si la particula i-ésima es mejor que la que he considerado como optima hasta el momento,
		//la tomo como nueva partícula óptima
		otro_fit= seleccionar_funcion_fitness(x[0]);
		if(otro_fit < fitness_OPT)
		{
			fitness_OPT= otro_fit;
			particula_OPT= i;
		}
	}

	//doy valor a g
	for(j=0; j<R; j++) //para cada componente de una partícula
	{
		g[j]= x[particula_OPT][j];
	}
}






void ordenar_ranas(double x[P][R],double fitness[P],double g[R])
{
	//se ordena de mayor menor
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
					aux1=x[i][k];
					x[i][k]=x[i+1][k];
					x[i+1][k]=aux1;
				}

			}
		}
	}
	for(i=0;i<P; i++)
	{
		//Si su mejor posicion local (o mejor solucion encontrada hasta el momento) es mejor que la solucion asociada al enjambre, la tomo como la nueva solucion del enjambre
		//Esto solo funciona si se minimiza(>)
		if(seleccionar_funcion_fitness(g)>seleccionar_funcion_fitness(x[i]))
		{ 
			//copio la mejor posicion local de la particula i-esima (copio  b[i] como nuevo valor de g)
			for(j=0; j<R; j++)
			{
				g[j]=x[i][j];
			}
		}
	}
}

/**
* x: array con las posiciones de todas las ranas
* x[i]  -> fila que almacena la posición de la rana i-ésima(i entre 0 y P-1)
* x[i][j] -> una compoente del vector de posición del a rana i-ésima
*/
void procesar_memeplexes(double X[P][R], double fitness[P], double g[R]){
	//ya se cual es la mejor y cual la peor
	int best=0, worst=P-1;//indices de mejor y peor
	double D2,D3,aleatorio;
	int i,j,meme,xb,xw,k=0;
	double x_prima[R]; //Posicion candidatas para la peor rana
	double fitness_max, fitness_min;
	for(meme=0; meme<R; meme++){
		fitness_max=-1;
		fitness_min=1000;
		
		for(i=0;i<j_max;i++){
			if(meme==i%m){
				if(fitness[i]<fitness_min){
					fitness_min=fitness[i];
					xb=i;
				}
				if(fitness[i]>fitness_max){
					fitness_max=fitness[i];
					xw=i;
				}
			}
		}
		for(j=0;j<R;j++){
			aleatorio=ALEATORIO(-1,1);
			D2=aleatorio*(X[xb][j]-X[xw][j]);
			if(D2>Dmax) D2=Dmax;
			if(D2<-Dmax) D2=-Dmax;
			x_prima[j]=X[xw][j]+D2;
		}
		if(seleccionar_funcion_fitness(x_prima)<seleccionar_funcion_fitness(X[xw])){
			fitness[xw]=seleccionar_funcion_fitness(x_prima);
		}else{
			for(j=0;j<R;j++){
				aleatorio=ALEATORIO(-1,1);
				D3=aleatorio*(g[j]-X[xw][j]);
				if(D3>Dmax)  D3=Dmax;
				if(D3<-Dmax) D3=-Dmax;
				x_prima[j]=X[xw][j]+D3;
			}
			if(seleccionar_funcion_fitness(x_prima)<seleccionar_funcion_fitness(X[xw])){
				fitness[xw]=seleccionar_funcion_fitness(x_prima);
			}else{
				for(j=0;j<R;j++)
				X[xw][j]=ALEATORIO(pos_min,pos_max);
			}
		}
		
	}



}


/**void grupos(x[P][R]){
	int i;
	//dos vectores apra almacenar en cada uno los valores (xb la mejor, xw la peor) de cada memeplex 
	double xb[m],xw[m];
	int mem;//a que memeplex pertenece
	//recorro todo el array y para cada rana perteneciente al grupo y redirijo según el memeplex al que pertenezca

	for(i=0;i<P;i++){
		mem=i%m;
		
	}



}*/
void mostrar_vector(double vector [R])
{
	int i;
	printf("\n POSICION SOLUCION:\n");
	for(i=0;i<R;i++){
		
		printf("\t%.3lf\n",vector[i]);
	}
	printf("\nfitness -> %f\n",seleccionar_funcion_fitness(vector));
}



void main()
{
	srand48(time(NULL));
	int t; //contador
	double x[P][R]; //posicion de las ranas
					//x[i] -> posicion de la rana i (entre 0 y P-1)
					//x[i][j] -> componente j de la posición de la rana i (j entre 0 y R-1)

	double fitness[P]; //fitness de las posiciones de las ranas que están almacenadas en el array x

	double g[R]; //mejor solución global (1 para el enjambre)

	//inicializar la población de partículas
	iniciar_enjambre(x, g);

	//muestro los valores iniciales almacenados en x y el fitness de cada fila
	//mostrar_posiciones_actuales(x);
	
	//bucle principal del algoritmo
	for(t=0; t<t_max; t++)
	{
		
		//calcular el fitness(eje x) de cada partícula del enjambre
		calcular_fitness(x, fitness);

        ordenar_ranas(x,fitness,g);
		// Los memeplexes son las posiciones n*m del array x 
        //generar_memeplexes(x,memeplexes);
		
		procesar_memeplexes(x,fitness,  g);

		//muestro los valores almacenados en x al concluir la iteración actual del algoritmo
		//printf("\nSOLUCIONES iter. %d -----------\n", t);
		//mostrar_posiciones_actuales(x);


	}
	
	//mostrar resultado final
	printf("\nSOLUCIÓN DEL PROBLEMA: \n");
	mostrar_vector(g);
}


