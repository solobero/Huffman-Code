# Algoritmo Huffman para Compresión de Archivos Texto
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

## Uso de Llamadas al Sistema

## Comandos para ejecutar
