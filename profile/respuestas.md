## Profiling con time
**Cuando compilamos sin flags de optimización y corremos el ejecutable con time genera el siguiente output:**

real	0m0.714s
user	0m0.660s
sys	0m0.048s

**Con el flag -O1 y time:**

real	0m0.477s
user	0m0.412s
sys	0m0.060s

**Con el flag -O2**

real	0m0.002s
user	0m0.000s
sys	0m0.000s

**Con el flag -O3**

real	0m0.002s
user	0m0.000s
sys	0m0.000s

Vemos que a medida que aumento el grado de optimización, el ejecutable tarda menos en correr. Algo a tener en cuenta es que 
los tiempos que tire el time no son reproducibles, supongo que esto tiene que ver con la "incerteza" de la medición y 
además con la cantidad de procesos que está corriendo la computadora en el momento que se ejecuta el programa.

**Time sólo da info de cuánto tarda el programa en correr, no es útil para saber dónde están los cuellos de botella
del programai (donde es más lento)**

##Profilling con gprof
Pasos para usar gprof
1) Compilar con gcc y el flag -pg
2) Correr el ejecutable --> esto genera el archivo gmon.out
3) gprof program.e gmon.out > profile.info --> esto me genera la info del profiling en el archivo profile.info

**Sin flags de optimización**

   %   cumulative   self              self     total           
  time   seconds   seconds    calls  ns/call  ns/call  name    
  51.01      0.36     0.36 25000000    14.28    14.28  first_assign
  33.05      0.59     0.23                             main
  15.81      0.70     0.11 25000000     4.43     4.43  second_assign

**Con -O1**
 
   %   cumulative   self              self     total          
  time   seconds   seconds    calls  ns/call  ns/call  name    
  51.96      0.17     0.17                             main
  42.79      0.31     0.14 25000000     5.65     5.65  first_assign
   4.58      0.33     0.02 25000000     0.61     0.61  second_assign
   1.53      0.33     0.01                             frame_dummy

**Con -O2**

  Each sample counts as 0.01 seconds.
  no time accumulated

**Con -O3**

  Each sample counts as 0.01 seconds.
  no time accumulated


El gprof nos tira mucha más info que hacer un "time". Divide el análisis en las funciones que tiene el programa, en este caso 
3, first_assign, main y second_assign. En la primera columna muestra el porcentaje de tiempo usado por la función en la
corrida. La segunda **cumulative seconds** es el tiempo acumulado en segundos. La tercera **self seconds** es el tiempo en
segundos sin acumular. La cuarta **calls** es la cantidad de veces que fue llamada la función. La quinta **self ns/call** nos
da el número promedio que le lleva a la función por llamado. La sexta **total ns/call** es algo parecido. y la sexta **name**
es el nombre de la función que se está analizando.
Comparando el primero y el segundo caso se puede ver que la optimización baja los tiempos de las tres funciones del programa.
La función que más se optimiza es la second_assign que pasa de 0.11s a 0.02s representando un porcentaje del 4% del tiempo de
cómputo. Otra cosa a observar es que en el primer caso la función first_assign era la que ocupaba el 50% del tiempo corriendo,
sin embargo, luego de la optimización con -O1 el que está más tiempo trabajando es el main (si bien lso tiempos de las 3 
funciones disminuyen.


##Ejercico de FORTRAN
Compilé el programa 2 con el flag -pg para usar el gprof. La data arrojada por el gprof es la siguiente:

  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 53.78      1.02     1.02 50000000     0.00     0.00  bad_assign_
 27.94      1.55     0.53 50000000     0.00     0.00  good_assign_
 17.40      1.88     0.33        1     0.33     1.88  MAIN__
  1.05      1.90     0.02                             frame_dummy

El programa consiste en una suma de matrices elemento a elemento. Esta suma la hace dos veces pero de diferente forma. Primero
sumo por columna y en el segundo caso suma por renglones. El primer caso resulta prácticamente el doble de rápido que el
segundo (se puede ver en los tiempos que toman las funciones bad_assign y good_assign). Esto se debe a la manera en la que
fortran acomoda las matrices. Es al revés que C, fortran las acomoda por columna. Esto hace que cuando busco un elemento de 
una matriz para operar, es más probable que tenga en caché a los elementos de una misma columna, y NO a los de una misma fila.
Por eso es que si hago la suma sobre las filas primero es menos eficiente, porque es más probable que tenga cache misses.
En este ejercicio se puede ver la influencia del caché en la performance del código.




