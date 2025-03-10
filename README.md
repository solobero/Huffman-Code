﻿# Algoritmo Huffman para Compresión de Archivos Texto
## Explicación Algoritmo Huffman
El algoritmo de Huffman es un algoritmo para la construcción de códigos de Huffmann, desarrollado por David A. Huffmann en 1952. 
Este algoritmo toma un alfabeto de n símbolos, junto con sus frecuencias de aparición asociadas, y produce un código de Huffman para ese alfabeto y esas frecuencias.
El algoritmo consiste en la creación de un árbol binario que tiene cada uno de los símbolos por hoja, y construido de tal forma que siguiéndolo desde la raíz a cada una de sus hojas se obtiene el código Huffman asociado.

1. Se crean varios árboles, uno por cada uno de los símbolos del alfabeto, consistiendo cada uno de los árboles en un nodo sin hijos, y etiquetado cada uno con su símbolo asociado y su frecuencia de aparición.
   
2. Se toman los dos árboles de menor frecuencia, y se unen creando un nuevo árbol. La etiqueta de la raíz será la suma de las frecuencias de las raíces de los dos árboles que se unen, y cada uno de estos árboles será un hijo del nuevo árbol. También se etiquetan las dos ramas del nuevo árbol: con un 0 la de la izquierda, y con un 1 la de la derecha.
   
3. Se repite el paso 2 hasta que sólo quede un árbol. Con este árbol se puede conocer el código asociado a un símbolo, así como obtener el símbolo asociado a un determinado código.

## Ejemplo
![Image](https://github.com/user-attachments/assets/9907ee41-14e0-48c8-8d99-27e857477dfc)

Identificamos cuantas veces aparace cada letra en la palabra "programa", después de esto ordenamos esta frecuencia de menor a mayor como se indica en la imagen

![Image](https://github.com/user-attachments/assets/bb95b3cb-89e3-4d9c-96f1-658cc4cc68d4)

Transformamos la estructura en una lista y la mantenemos ordenada de menor a mayor según la frecuencia de las letras de la palabra. Después formamos grupos de dos nodos para crear un nuevo nodo padre con el símbolo NULL, pero la cantidad de apariciones de ese nodo padre es la suma de los dos nodos hijos. El nuevo nodo lo ingresamos en la lista y procedemos a agrupar de a dos nodos como se hizo anteriormente. Realizamos esto hasta que solo quede un nodo.

![Image](https://github.com/user-attachments/assets/94c4d047-2814-4559-81a4-65df1bc35448)

Aquí por ejemplo, agrupamos los dos nodos padres para crear otro nodo más

![Image](https://github.com/user-attachments/assets/fef231d9-7f53-4cb4-9403-4e5a57d75219)

![Image](https://github.com/user-attachments/assets/adfc7b47-adea-4938-ae9e-2ff749f7263b)

Al final queda un único nodo padre con varias hojas hijas, que son las que representan el árbol binario para codificar el mensaje

![Image](https://github.com/user-attachments/assets/d9b43dc4-8960-4328-92ef-0262119f3a46)

Ahora para codificar el mensaje, ponemos un "0" a los lazos que van hacía la izquierda, y un "1" a los lazos que van a la derecha.

![Image](https://github.com/user-attachments/assets/c4a2e36f-d0dd-45f8-a76d-63e495d6503d)

El código que se asocia a cada símbolo del mensaje original, equivale al camino que se recorre desde la raiz del árbol hasta la hoja que contiene ese símbolo.

![Image](https://github.com/user-attachments/assets/9354b862-a0e5-49cf-8f3a-c21409b9c5ae)

Por último, le damos los valores de Huffman a cada símbolo del mensaje original y así obtendremos el mensaje comprimido con el algoritmo de Huffman.

![Image](https://github.com/user-attachments/assets/a92459ea-837a-49b1-adaa-ba1b73c472ab)
## Uso de Llamadas al Sistema

```cpp
int fd = open(filename, O_RDONLY);

int fdHuff = open(huffFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
int fdTxt = open(txtFilename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
```

Se usa con los flags O_RDONLY para lectura o O_WRONLY | O_CREAT | O_TRUNC para escritura, creación y truncamiento si el archivo ya existe.

```cpp
read(fd, buffer, BUFFER_SIZE); // Lee el archivo txt en grandes en partes

read(fd, &marker, 1); // Se usa para saber si es un nodo hoja o interno

read(fd, &value, sizeof(char)); // Leer el carácter almacenado en el nodo hoja

read(fd, &repetitions, sizeof(int)); // Leer la frecuencia del carácter
```

Se leen los datos desde el archivo txt el cual vamos a comprimir y se almacena en variables o buffers. Cada llamada a read() cumple una función específica en la manipulación del archivo comprimido y del árbol de Huffman

```cpp
write(fd, compressedContent.c_str(), compressedContent.size()); // Guarda la versión comprimida del archivo

write(fdHuff, compressedContent.c_str(), compressedContent.size()); // Guarda la versión comprimida en .huff

write(fdTxt, compressedContent.c_str(), compressedContent.size()); // Guarda la versión comprimida en .txt

write(fd, decompressedContent.c_str(), decompressedContent.size()); // Guarda el texto original

write(fd, "1", 1); // Serialización del árbol de Huffman nodo hoja
write(fd, &root->value, sizeof(char)); // Guarda el carácter en una hoja
write(fd, &root->repetitions, sizeof(int)); // Guarda la frecuencia del carácter

write(fd, "0", 1); // Serialización del árbol de Huffman nodo interno 
```
Los usamos escribir datos en archivos. Cada una tiene un propósito específico dependiendo del archivo que se esté manipulando. 
```cpp
close(fd); // Cierra el descriptor del archivo después de leer/escribir

close(fdHuff); // Asegura que los datos comprimidos se guarden correctamente

 close(fdTxt); // Finaliza la escritura del archivo comprimido en texto
```

Los usamos para cerrar archivos una vez que ya no son necesarios. Esto libera recursos del sistema y evita problemas como fugas de descriptores de archivos.

### Serialización del Árbol de Huffman
El proceso de serialización convierte el árbol de Huffman en un formato binario para guardarlo en un archivo.

#### Funcionamiento de serializeHuffmanTree() :

**Nodos hoja (letras):**

Escribe un '1' en el archivo para indicar que es una hoja.

Luego escribe el carácter y la cantidad de repeticiones.

**Nodos internos:**

Escribe un '0' para indicar un nodo interno.

Llama recursivamente a la función para los hijos izquierdo y derecho.

### El proceso de deserialización reconstruye el árbol de Huffman a partir del archivo guardado.

#### Funcionamiento de deserializeHuffmanTree() :
**Lee el primer byte:**

Si es '1', crea un nodo hoja con su valor y frecuencia.

Si es '0', crea un nodo interno, reconstruyendo sus hijos recursivamente.

Se leen primero los hijos izquierdos, luego los derechos debido a la estructura recursiva.

## Comandos para ejecutar
Para ejecutar el código del algoritmo de Huffman podemos usar el comando "make" para ejecutar todo el código en su totalidad (Todo se debe realizar en la localización de la carpeta en la que se encuentre el proyecto):

`make`

Si se quiere ejecutar manualmente archivo por archivo se usarían:

```
g++ -c main.cpp -o main.o
g++ -c file_io.cpp -o file_io.o
g++ -c huff.cpp -o huff.o
g++ main.o huffman.o file_io.o -o huffman_programa
```

Por último para comprimir un archivo de texto y descomprimirlo se usarían los siguientes comandos respectivamente:

```
./huffman_programa -c archivo.txt 
./huffman_programa -x archivo.txt 
```

Los comandos que se pueden usar son:
* -c ó --compress para comprimir un archivo de texto
* -x ó --decompress para descomprimir un archivo .huff
* -h ó --help para mostrar los comandos que se pueden usar en el programa
* -v ó --version para ver la versión del programa
```
./huffman_programa -c archivo.txt 
./huffman_programa -x archivo.txt 
./huffman_programa -h 
./huffman_programa -v
```

