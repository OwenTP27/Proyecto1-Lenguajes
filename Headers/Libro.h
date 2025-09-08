#ifndef LIBRO
#define	LIBRO

typedef struct {
	char *codigo;
	char *nombre;
	char *autor;
	float precio;
}Libro;


Libro* construirLibro(char* codigoLibro, char* nombreLibro, char* autorLibro, float precioLibro ) {
	Libro *nuevoLibro = malloc(sizeof(Libro));
	nuevoLibro->codigo = codigoLibro;
	nuevoLibro->nombre = nombreLibro;
	nuevoLibro->autor = autorLibro;
	nuevoLibro->precio = precioLibro;
	return nuevoLibro;
}

#endif // !LIBRO
