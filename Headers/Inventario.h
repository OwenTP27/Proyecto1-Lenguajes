#include "Libro.h"

#ifndef INVENTARIO
#define INVENTARIO


typedef struct Inventario {
	Libro libro;
	int cantidad;
	struct Inventario *siguiente;
} Inventario;

Inventario* construirInventario(Libro* nuevoLibro, int cantidadLibro) {
	Inventario *nuevoInventario = malloc(sizeof(Inventario));
	nuevoInventario->libro = *nuevoLibro;
	nuevoInventario->cantidad = cantidadLibro;
	nuevoInventario->siguiente = NULL;
	return nuevoInventario;

}

void agregarLibro(Inventario* inventario, Inventario* libro) {
	inventario->siguiente = libro;
}

void mostrarInventario(Inventario* inventario) {
	while (inventario != NULL) {
		printf("\nTitulo: %s\nAutor: %s\nPrecio: %.2f\nCantidad: %d\nCodigo: %s\n",
			inventario->libro.nombre,
			inventario->libro.autor,
			inventario->libro.precio,
			inventario->cantidad,
			inventario->libro.codigo);

		inventario = inventario->siguiente;
	}
}

void guardarInventarioEnArchivo(Inventario* inventario) {
	FILE* archivo = fopen("Proyecto1-Lenguajes\\Data\\Inventario.txt", "w");
	if (archivo == NULL) {
		perror("Error al abrir el archivo");
		return;
	}

	while (inventario != NULL) {
		fprintf(archivo,
			"Titulo: %s\nAutor: %s\nPrecio: %.2f\nCantidad: %d\nCodigo: %s\n\n",
			inventario->libro.nombre,
			inventario->libro.autor,
			inventario->libro.precio,
			inventario->cantidad,
			inventario->libro.codigo);

		inventario = inventario->siguiente;
	}

	fclose(archivo); // cerrar archivo al terminar
}


#endif // !"INVENTARIO"
