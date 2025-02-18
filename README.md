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

## Comandos para ejecutar
