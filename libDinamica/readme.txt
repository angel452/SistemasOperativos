A diferencia de las librerias estaticas, si cometemos un error en alguno de los archivos; nos obliga a volver a compilar todo, lo que provoque que tengamos copias de codigos que probablemente no lo vamos a utilizar

Concepto:
	es bastante parecida a una bilbioteca estativa
	contiene codigo objeto listo para utilizar 
	Este se ve afectado mas al SO. 
	Las librerias dinamicas son codigos ya escritos en el mismo SO. Como lib64/, so/, etc. Para que asi el ejecutable no sea tan pesado. Y estas estan disponibles en cualquier momento. Cosa que cuando se desee cambiar alguna parte del codigo de la libreria dinamica, no habra mucho problema. Pues, como sabemos, el ejecutabe se enlazara a la libreria en el MISMO MOMENTO  desde que se ejecuta, y la correccion ya estara disponible 
