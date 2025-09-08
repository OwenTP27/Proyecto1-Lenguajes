#include "Libro.h"

#ifndef INVENTARIO
#define INVENTARIO

// Inicializacion: Inventario* inventario = NULL;
// Para agregar al inventario se usa la funcion agregarAlInventario() usando la referencia al inventario &inventario
// Se debe construir el libro usando construirLibro() antes de intentar construir un inventario usando la funcion construirInventario
// El resto de funciones actuales solo necesitan inventario como parametro excepto construirInventario que necesita un libro y su cantidad

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

void agregarAlInventario(Inventario** inventario, Inventario* nuevo) {
	nuevo->siguiente = NULL;
	if (*inventario == NULL) {
		*inventario = nuevo;
		return;
	}

	Inventario* actual = *inventario;
	while (actual->siguiente != NULL) {
		actual = actual->siguiente;
	}
	actual->siguiente = nuevo;
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

	fclose(archivo);
}


#endif // !"INVENTARIO"
