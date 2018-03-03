# debug 

##Corriendo segfault con gdb, sin compilar con flag:

 run:
 Program received signal SIGSEGV, Segmentation fault.
 0x00000000004005eb in main ()

 next:
 Single stepping until exit from function main,
 which has no line number information.
 
 Program terminated with signal SIGSEGV, Segmentation fault.
 The program no longer exists.

##Corriendo segfault con gdb, compilado con flag -g:

 Program received signal SIGSEGV, Segmentation fault. 
 0x00000000004005eb in main (argc=1, argv=0x7fffffffd438) at add_array_segfault.c:19 
 19	    b[i] = i;

##cuando lo compilás con el flag -g, aparte de tirarte el error te tira dónde está, en este caso se encuentra en el main de
##add_array_segfault en la linea 19. Y me printea esa linea abajo.

Análisis del código:

int *a, *b;               **Acá se declara a y b como punteros**
int n = 3;
int i, sum;
for (i = 0; i < n; i++) {
a[i] = i;                 **El problema estaba en esta linea**
b[i] = i;

Cuando quiere asignarle el valor a "a" y "b" en el for, no sabe dónde mierda están! Porque nunca se lo dije! Solo los declaré
como un puntero de enteros pero nada más. 

##SOLUCIÓN 
Necesito "allocatear" el espacio de memoria que voy a usar, eso lo puedo hacer de dos formas:

##Primera forma: le doy un espacio estático (en el stack)

 int n = 3;            **defino el n primero**
 int a[n], b[n];       **defino a y b con la dimensión de n**

Con esta solución el programa le asigna un espacio de memoria en el stack, por lo tanto cuando arranque el for ese espacio ya
está asignado y no tiene problemas en darle valores.

##Segunda forma: allocateo dinámico (en el heap)

 int *a, *b;                   a y b son solo punteros, en el stack ocupan 8 bytes pero no se sabe aún dónde están
 int n = 3;                    porque no lo he hecho aún. El puntero "vive" en el stack.   
 int i, sum;
 a = malloc(n*sizeof(int));    allocateo la memoria que necesitan a y b
 b = malloc(n*sizeof(int));    ahora ya no son solo un puntero en el stack sino que ya tienen asignado el espacio de memoria
                               que necesitan  en el heap. Ahora cuando a y b entren al for no tendrán ningún problema.
 bla bla

 free(a)                       **tengo que liberar el espacio que ocupaban a y b luego de que los usé!(antes del return**
 free(b)
 return
 
**Cualquiera de las dos formas son practicamente iguales en eficiencia, ya que la única diferencia en memoria son los 8 bytes
que ocupan en el stack cuando hago el malloc. PERO, el allocateo dinámico me permite asignar el espacio de memoria que yo
quiera! A diferencia del stack, que tiene solo 8 megas por defecto.**


##Static

Cuando compilamos el static.c te larga un ejecutable que podés correr. Cuando corrés el ejecutable, la suma no da 6 como en
el caso del segfault y encima cada vez que se corre da un valor distinto. 

Cuando lo corro con gdb y le hago un break en la función add_array puedo ir analizando lo que va haciendo con el comando n
(next) que me permite ir avanzando paso a paso. Ahí descubro que al loop del for lo hace 5 veces, cuando los arrays son de
dimensión 3. Esto se puede ver en el código:

 int add_array(int *a, int *b, int n){
   int sum = 0;
   int i = 0;
 //  for (i = 0; i <= n + 1; i++) {     //este loop está para el ojete, se pasa del vector y hecha moco
   for (i=0; i < n; i++) {
     sum += abs(a[i]);
     sum += abs(b[i]);
   };
   return sum;

#SOLUCIÓN
cuando se reemplaza la linea del for comentada, por la que se encuentra abajo (que hace que el loop se haga 3 veces) se 
corrige el error. Ahora el resultado es el correcto y es reproducible. Antes daba cualquier cosa porque el programa hacía la 
suma de los vectores perono frenaba al final de cada vector, sino que continuaba porque el loop lo decía, entonces accedía a
un lugar de memoria en el stack contiguo al tercer elemento del vector, ese lugar seguramente estaba ocupado con otra cosa,
lo que metía un error aleatorio en la suma cada vez que se corría el programa.

##Dynamic
El código tiene el mismo error que en el caso anterior, sin embargo, cuando se compila para producir el ejecutable y luego se
lo ejecuta, el resultado es correcto y reproducible (al menos para un buen número de veces, osea, todas las que probé).
Funciona, pero, ¿Por qué? La respuesta está en que el allocateo de memoria es dinámico, está en el heap, y en este tipo de
allocateo se suele limpiar cuando se asigna el espacio, por lo tanto es muy probable que el espacio alrededor esté vacío. 
Cuando uno corre el programa con el debugger gdb, el comportamiento es el mismo, hace 5 veces el loop, sin embargo, devuelve
el resultado correcto porque el "cuarto" y "quinto" lugar de memoria están vacíos, o sea que suma ceros.






