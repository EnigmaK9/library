/*
 * Archivo: principal.cpp
 * Descripción: Sistema simple de gestión de biblioteca con gestión completa de usuarios, búsqueda y multas por retraso
 * Fecha de creación: 2025-04-18
 * Última modificación: 2025-04-18
 * Archivos necesarios: principal.cpp
 *
 * Compilación (Debian 12):
 *   g++ -std=c++11 -o biblioteca principal.cpp
 *
 * Ejecución:
 *   ./biblioteca
 */

#include <iostream>    // Para entrada/salida
#include <string>      // Para std::string
#include <vector>      // Para std::vector
#include <regex>       // Para validación con expresiones regulares
#include <algorithm>   // Para std::remove_if

// Constantes de configuración
const int MAX_LIBROS = 200;           // Límite de libros
const int MAX_USUARIOS = 100;         // Límite de usuarios
const double MULTA_DIARIA = 2.0;      // Multa por día de retraso (en unidades monetarias)

// Estructura que representa un libro en la biblioteca
struct Libro {
    std::string titulo;    // Título del libro
    std::string autor;     // Autor del libro
    int id;                // Identificador único del libro
    bool disponible;       // true si está disponible para préstamo
};

// Estructura que representa un usuario de la biblioteca
struct Usuario {
    std::string nombre;            // Nombre del usuario
    std::string apellidoPaterno;   // Apellido paterno
    std::string apellidoMaterno;   // Apellido materno
    int id;                        // Identificador único del usuario
    std::vector<int> librosPrestados; // IDs de libros actualmente prestados
};

// Almacenamiento en memoria (vectores estáticos + contadores)
Libro libros[MAX_LIBROS];
int cantidadLibros = 0;
Usuario usuarios[MAX_USUARIOS];
int cantidadUsuarios = 0;

/**
 * validarTexto
 * Valida que una cadena contenga solo letras y espacios (incluye acentos).
 * Devuelve true si pasa la validación.
 */
bool validarTexto(const std::string& texto) {
    std::regex re("^[A-Za-zÁÉÍÓÚáéíóúñÑ ]+$");
    return std::regex_match(texto, re);
}

/**
 * validarTitulo
 * Valida que el título no esté vacío.
 */
bool validarTitulo(const std::string& titulo) {
    return !titulo.empty();
}

/**
 * agregarUsuario
 * Solicita datos de usuario, los valida y los agrega al sistema.
 */
void agregarUsuario() {
    if (cantidadUsuarios >= MAX_USUARIOS) {
        std::cout << "Capacidad máxima de usuarios alcanzada." << std::endl;
        return;
    }
    Usuario u;
    // Nombre
    do {
        std::cout << "Ingrese nombre del usuario: ";
        std::getline(std::cin, u.nombre);
        if (!validarTexto(u.nombre)) {
            std::cout << "Nombre inválido. Solo letras y espacios permitidos." << std::endl;
        }
    } while (!validarTexto(u.nombre));
    // Apellido paterno
    do {
        std::cout << "Ingrese apellido paterno: ";
        std::getline(std::cin, u.apellidoPaterno);
        if (!validarTexto(u.apellidoPaterno)) {
            std::cout << "Apellido inválido. Solo letras y espacios permitidos." << std::endl;
        }
    } while (!validarTexto(u.apellidoPaterno));
    // Apellido materno
    do {
        std::cout << "Ingrese apellido materno: ";
        std::getline(std::cin, u.apellidoMaterno);
        if (!validarTexto(u.apellidoMaterno)) {
            std::cout << "Apellido inválido. Solo letras y espacios permitidos." << std::endl;
        }
    } while (!validarTexto(u.apellidoMaterno));
    // ID
    std::cout << "Ingrese ID numérico de usuario: ";
    std::cin >> u.id;
    std::cin.ignore();

    usuarios[cantidadUsuarios++] = u;
    std::cout << "Usuario agregado exitosamente." << std::endl;
}

/**
 * agregarLibro
 * Solicita datos de libro, los valida y los agrega al sistema.
 */
void agregarLibro() {
    if (cantidadLibros >= MAX_LIBROS) {
        std::cout << "Capacidad máxima de libros alcanzada." << std::endl;
        return;
    }
    Libro b;
    // Título
    do {
        std::cout << "Ingrese título del libro: ";
        std::getline(std::cin, b.titulo);
        if (!validarTitulo(b.titulo)) {
            std::cout << "Título inválido. No puede estar vacío." << std::endl;
        }
    } while (!validarTitulo(b.titulo));
    // Autor (solo texto)
    do {
        std::cout << "Ingrese autor del libro: ";
        std::getline(std::cin, b.autor);
        if (!validarTexto(b.autor)) {
            std::cout << "Autor inválido. Solo letras y espacios permitidos." << std::endl;
        }
    } while (!validarTexto(b.autor));
    // ID
    std::cout << "Ingrese ID numérico de libro: ";
    std::cin >> b.id;
    std::cin.ignore();
    b.disponible = true;

    libros[cantidadLibros++] = b;
    std::cout << "Libro agregado exitosamente." << std::endl;
}

/**
 * prestarLibro
 * Permite prestar un libro a un usuario válido.
 * Actualiza disponibilidad y registra el préstamo en el usuario.
 */
void prestarLibro() {
    int idUsuario, idLibro;
    std::cout << "Ingrese ID de usuario para préstamo: ";
    std::cin >> idUsuario;
    std::cin.ignore();
    // Buscar usuario
    int idxU = -1;
    for (int i = 0; i < cantidadUsuarios; ++i) {
        if (usuarios[i].id == idUsuario) {
            idxU = i; break;
        }
    }
    if (idxU < 0) {
        std::cout << "Usuario no encontrado." << std::endl;
        return;
    }
    std::cout << "Ingrese ID de libro a prestar: ";
    std::cin >> idLibro;
    std::cin.ignore();
    // Buscar libro
    for (int i = 0; i < cantidadLibros; ++i) {
        if (libros[i].id == idLibro) {
            if (libros[i].disponible) {
                libros[i].disponible = false;
                usuarios[idxU].librosPrestados.push_back(idLibro);
                std::cout << "Préstamo registrado exitosamente." << std::endl;
            } else {
                std::cout << "El libro no está disponible." << std::endl;
            }
            return;
        }
    }
    std::cout << "Libro con ID " << idLibro << " no encontrado." << std::endl;
}

/**
 * devolverLibro
 * Registra la devolución de un libro y calcula multa si hubo retraso.
 */
void devolverLibro() {
    int idUsuario, idLibro;
    std::cout << "Ingrese ID de usuario para devolución: ";
    std::cin >> idUsuario;
    std::cin.ignore();
    // Buscar usuario
    int idxU = -1;
    for (int i = 0; i < cantidadUsuarios; ++i) {
        if (usuarios[i].id == idUsuario) { idxU = i; break; }
    }
    if (idxU < 0) {
        std::cout << "Usuario no encontrado." << std::endl;
        return;
    }
    std::cout << "Ingrese ID de libro a devolver: ";
    std::cin >> idLibro;
    std::cin.ignore();
    // Verificar que el usuario tenga ese libro prestado
    auto &prestados = usuarios[idxU].librosPrestados;
    auto it = std::find(prestados.begin(), prestados.end(), idLibro);
    if (it == prestados.end()) {
        std::cout << "Este libro no está prestado a ese usuario." << std::endl;
        return;
    }
    // Calcular multa
    int diasRetardo;
    std::cout << "Días de retraso (0 si a tiempo): ";
    std::cin >> diasRetardo;
    std::cin.ignore();
    if (diasRetardo > 0) {
        double multa = diasRetardo * MULTA_DIARIA;
        std::cout << "Multa por retraso: " << multa << std::endl;
    }
    // Marcar como disponible y eliminar del vector de préstamos
    for (int i = 0; i < cantidadLibros; ++i) {
        if (libros[i].id == idLibro) {
            libros[i].disponible = true;
            break;
        }
    }
    prestados.erase(std::remove(prestados.begin(), prestados.end(), idLibro), prestados.end());
    std::cout << "Devolución registrada exitosamente." << std::endl;
}

/**
 * contarLibrosDisponibles
 * Retorna la cantidad de libros disponibles en el sistema.
 */
int contarLibrosDisponibles() {
    int disponibles = 0;
    for (int i = 0; i < cantidadLibros; ++i) {
        if (libros[i].disponible) {
            ++disponibles;
        }
    }
    return disponibles;
}

/**
 * buscarPorAutor
 * Permite buscar y listar libros por nombre de autor.
 */
void buscarPorAutor() {
    std::string autor;
    std::cout << "Ingrese nombre de autor a buscar: ";
    std::getline(std::cin, autor);
    std::cout << "Resultados de la búsqueda:" << std::endl;
    bool encontrado = false;
    for (int i = 0; i < cantidadLibros; ++i) {
        if (libros[i].autor == autor) {
            std::cout << "- " << libros[i].titulo << " (ID: " << libros[i].id << ")"
                      << (libros[i].disponible ? " [Disponible]" : " [Prestado]")
                      << std::endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "No se encontraron libros de ese autor." << std::endl;
    }
}

int main() {
    int opcion = -1;
    while (opcion != 0) {
        std::cout << "\n=== Sistema de Gestión de Biblioteca ===\n";
        std::cout << "1. Agregar libro\n";
        std::cout << "2. Agregar usuario\n";
        std::cout << "3. Prestar libro\n";
        std::cout << "4. Devolver libro\n";
        std::cout << "5. Mostrar cantidad de libros disponibles\n";
        std::cout << "6. Buscar libros por autor\n";
        std::cout << "0. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: agregarLibro(); break;
            case 2: agregarUsuario(); break;
            case 3: prestarLibro(); break;
            case 4: devolverLibro(); break;
            case 5:
                std::cout << "Libros disponibles: " << contarLibrosDisponibles() << std::endl;
                break;
            case 6: buscarPorAutor(); break;
            case 0:
                std::cout << "Saliendo del sistema. ¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Intente de nuevo." << std::endl;
        }
    }
    return 0;
}
