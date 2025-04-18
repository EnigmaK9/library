/****************************************************************************
*  _      _ _                   _         _    _         __  __             *
* | |    (_) |                 (_)       | |  | |  /\   |  \/  |            *
* | |     _| |__  _ __ ___ _ __ _  __ _  | |  | | /  \  | \  / |            *
* | |    | | '_ \| '__/ _ \ '__| |/ _` | | |  | |/ /\ \ | |\/| |            *
* | |____| | |_) | | |  __/ |  | | (_| | | |__| / ____ \| |  | |            *
* |______|_|_.__/|_|  \___|_|  |_|\__,_|  \____/_/    \_\_|  |_|            *
*                                                                           *
*   Archivo: 02principal.cpp                                                *
*   Descripción: Sistema completo de gestión de biblioteca con usuarios,    *
*                préstamos, búsquedas y multas por retraso.                 *
*   Creación       : 2025-04-18                                             *
*   Última modificación  : 2025-04-18                                       *
*   Archivos necesarios: 02principal.cpp                                      *
*                                                                           *
*   Compilación (Probado en Debian 12):                                                *
*     g++ -std=c++11 -O2 -Wall -o libreria 02principal.cpp                *
*                                                                           *
*   Ejecución:                                                              *
*     ./biblioteca                                                          *
****************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <cctype>
#include <climits>

// ========================================
// ======== CONSTANTES DEL SISTEMA ========
// ========================================
const int MAX_LIBROS      = 200;   // Límite de libros
const int MAX_USUARIOS    = 100;   // Límite de usuarios
const double MULTA_DIARIA = 2.0;   // Multa por día de retraso

// ========================================
// ==== DEFINICIONES GLOBALES AUXILIARES ==
// ========================================
static int nextLibroId   = 1;      // Siguiente ID para libro
static int nextUsuarioId = 1;      // Siguiente ID para usuario

// ========================================
// =========== ESTRUCTURAS ================
// ========================================
struct Libro {
    int id;
    std::string titulo;
    std::string autor;
    bool disponible;
};

struct Usuario {
    int id;
    std::string nombre;
    std::string apellidoPaterno;
    std::string apellidoMaterno;
    std::vector<int> librosPrestados;
};

// Arreglos y contadores
static Libro   libros[MAX_LIBROS];
static Usuario usuarios[MAX_USUARIOS];
static int cantidadLibros   = 0;
static int cantidadUsuarios = 0;

// ========================================
// =========== AUXILIARES =================
// ========================================
bool validarTexto(const std::string& texto) {
    static const std::regex re("^[A-Za-zÁÉÍÓÚáéíóúñÑ ]+$");
    return std::regex_match(texto, re);
}

bool validarTitulo(const std::string& titulo) {
    return !titulo.empty();
}

int buscarUsuarioPorId(int id) {
    for (int i = 0; i < cantidadUsuarios; ++i)
        if (usuarios[i].id == id) return i;
    return -1;
}

int buscarLibroPorId(int id) {
    for (int i = 0; i < cantidadLibros; ++i)
        if (libros[i].id == id) return i;
    return -1;
}

std::string toLower(const std::string& s) {
    std::string r = s;
    for (char &c : r) c = std::tolower(c);
    return r;
}

/**
 * distanciaLevenshtein
 * --------------------
 * Calcula la "distancia de edición" mínima entre dos cadenas:
 * el número menor de inserciones, borrados o sustituciones
 * necesarias para transformar la cadena a en la cadena b.
 *
 * Parámetros:
 *   - a: cadena origen
 *   - b: cadena destino
 *
 * Retorna:
 *   Entero con la distancia de edición.
 */
int distanciaLevenshtein(const std::string& a, const std::string& b) {
    int n = a.size();  // longitud de la cadena a
    int m = b.size();  // longitud de la cadena b

    // Matriz de (n+1) x (m+1) para acumular costos parciales
    // d[i][j] = distancia entre primeros i caracteres de a y primeros j de b
    std::vector<std::vector<int>> d(n + 1, std::vector<int>(m + 1));

    // Caso base: transformar cadena vacía en prefijos de b
    for (int i = 0; i <= n; ++i) {
        d[i][0] = i;  // i eliminaciones necesarias
    }
    // Caso base: transformar prefijos de a en cadena vacía
    for (int j = 0; j <= m; ++j) {
        d[0][j] = j;  // j inserciones necesarias
    }

    // Rellenar matriz comparando caracteres uno a uno
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            // Costo 0 si los caracteres coinciden, 1 si difieren
            int costo = (a[i - 1] == b[j - 1] ? 0 : 1);

            // Elegir la operación que dé menor costo:
            // 1) Eliminación (d[i-1][j] + 1)
            // 2) Inserción (d[i][j-1] + 1)
            // 3) Sustitución (d[i-1][j-1] + costo)
            d[i][j] = std::min({
                d[i - 1][j] + 1,
                d[i][j - 1] + 1,
                d[i - 1][j - 1] + costo
            });
        }
    }

    // El valor en la esquina inferior derecha es la distancia total
    return d[n][m];
}


/**
 * buscarUsuarioPorNombre
 * Se solicita nombre y apellidos (posible con errores) y se devuelve
 * el usuario cuyo nombre completo es más parecido.
 */
void buscarUsuarioPorNombre() {
    if (cantidadUsuarios == 0) {
        std::cout << "No hay usuarios registrados.\n";
        return;
    }
    std::string consulta;
    std::cout << "Ingrese nombre y apellidos del usuario a buscar: ";
    std::getline(std::cin, consulta);

    std::string consultaLow = toLower(consulta);
    int mejorIdx = 0;
    int mejorDist = INT_MAX;
    for (int i = 0; i < cantidadUsuarios; ++i) {
        std::string completo = usuarios[i].nombre + " "
                             + usuarios[i].apellidoPaterno + " "
                             + usuarios[i].apellidoMaterno;
        int dist = distanciaLevenshtein(toLower(completo), consultaLow);
        if (dist < mejorDist) {
            mejorDist = dist;
            mejorIdx = i;
        }
    }
    Usuario &u = usuarios[mejorIdx];
    std::cout << "Usuario encontrado: "
              << u.nombre << " " << u.apellidoPaterno << " "
              << u.apellidoMaterno << " (ID: " << u.id << ")\n";
}

// ========================================
// =========== FUNCIONES ==================
// ========================================
void agregarUsuario() {
    if (cantidadUsuarios >= MAX_USUARIOS) {
        std::cout << "Capacidad máxima de usuarios alcanzada.\n";
        return;
    }
    Usuario u;
    u.id = nextUsuarioId++;
    do {
        std::cout << "Ingrese nombre del usuario: ";
        std::getline(std::cin, u.nombre);
        if (!validarTexto(u.nombre))
            std::cout << "Nombre inválido. Solo letras y espacios permitidos.\n";
    } while (!validarTexto(u.nombre));

    do {
        std::cout << "Ingrese apellido paterno: ";
        std::getline(std::cin, u.apellidoPaterno);
        if (!validarTexto(u.apellidoPaterno))
            std::cout << "Apellido inválido. Solo letras y espacios permitidos.\n";
    } while (!validarTexto(u.apellidoPaterno));

    do {
        std::cout << "Ingrese apellido materno: ";
        std::getline(std::cin, u.apellidoMaterno);
        if (!validarTexto(u.apellidoMaterno))
            std::cout << "Apellido inválido. Solo letras y espacios permitidos.\n";
    } while (!validarTexto(u.apellidoMaterno));

    usuarios[cantidadUsuarios++] = u;
    std::cout << "Usuario agregado con ID: " << u.id << "\n";
}

void agregarLibro() {
    if (cantidadLibros >= MAX_LIBROS) {
        std::cout << "Capacidad máxima de libros alcanzada.\n";
        return;
    }
    Libro b;
    b.id = nextLibroId++;
    do {
        std::cout << "Ingrese título del libro: ";
        std::getline(std::cin, b.titulo);
        if (!validarTitulo(b.titulo))
            std::cout << "Título inválido. No puede estar vacío.\n";
    } while (!validarTitulo(b.titulo));

    do {
        std::cout << "Ingrese autor del libro: ";
        std::getline(std::cin, b.autor);
        if (!validarTexto(b.autor))
            std::cout << "Autor inválido. Solo letras y espacios permitidos.\n";
    } while (!validarTexto(b.autor));

    b.disponible = true;
    libros[cantidadLibros++] = b;
    std::cout << "Libro agregado con ID: " << b.id << "\n";
}

void prestarLibro() {
    int idUsuario, idLibro;
    std::cout << "Ingrese ID de usuario para préstamo: ";
    std::cin >> idUsuario; std::cin.ignore();
    int idxU = buscarUsuarioPorId(idUsuario);
    if (idxU < 0) { std::cout << "Usuario no encontrado.\n"; return; }

    std::cout << "Ingrese ID de libro a prestar: ";
    std::cin >> idLibro; std::cin.ignore();
    int idxB = buscarLibroPorId(idLibro);
    if (idxB < 0) { std::cout << "Libro no encontrado.\n"; return; }
    if (!libros[idxB].disponible) {
        std::cout << "El libro no está disponible.\n"; return;
    }

    libros[idxB].disponible = false;
    usuarios[idxU].librosPrestados.push_back(idLibro);
    std::cout << "Préstamo registrado exitosamente.\n";
}

void devolverLibro() {
    int idUsuario, idLibro;
    std::cout << "Ingrese ID de usuario para devolución: ";
    std::cin >> idUsuario; std::cin.ignore();
    int idxU = buscarUsuarioPorId(idUsuario);
    if (idxU < 0) { std::cout << "Usuario no encontrado.\n"; return; }

    std::cout << "Ingrese ID de libro a devolver: ";
    std::cin >> idLibro; std::cin.ignore();
    auto &prestados = usuarios[idxU].librosPrestados;
    auto it = std::find(prestados.begin(), prestados.end(), idLibro);
    if (it == prestados.end()) {
        std::cout << "Este libro no está prestado a ese usuario.\n"; return;
    }

    int diasRetardo;
    std::cout << "Días de retraso (0 días si a tiempo): ";
    std::cin >> diasRetardo; std::cin.ignore();
    if (diasRetardo > 0) {
        double multa = diasRetardo * MULTA_DIARIA;
        std::cout << "Multa por retraso: " << multa << " pesotes" << "\n";
    }

    int idxB = buscarLibroPorId(idLibro);
    if (idxB >= 0) libros[idxB].disponible = true;
    prestados.erase(std::remove(prestados.begin(), prestados.end(), idLibro),
                    prestados.end());
    std::cout << "Devolución registrada exitosamente.\n";
}

int contarLibrosDisponibles() {
    int disponibles = 0;
    for (int i = 0; i < cantidadLibros; ++i)
        if (libros[i].disponible) ++disponibles;
    return disponibles;
}

void buscarPorAutor() {
    std::string autor;
    std::cout << "Ingrese nombre de autor a buscar: ";
    std::getline(std::cin, autor);
    std::cout << "\nResultados de la búsqueda:\n";
    bool encontrado = false;
    for (int i = 0; i < cantidadLibros; ++i) {
        if (libros[i].autor == autor) {
            std::cout << "- " << libros[i].titulo
                      << " (ID: " << libros[i].id << ")"
                      << (libros[i].disponible ? " [Disponible]" : " [Prestado]")
                      << "\n";
            encontrado = true;
        }
    }
    if (!encontrado)
        std::cout << "No se encontraron libros de ese autor.\n";
}

static const std::string BANNER = R"(

 _      _ _                   _         _    _         __  __
| |    (_) |                 (_)       | |  | |  /\   |  \/  |
| |     _| |__  _ __ ___ _ __ _  __ _  | |  | | /  \  | \  / |
| |    | | '_ \| '__/ _ \ '__| |/ _` | | |  | |/ /\ \ | |\/| |
| |____| | |_) | | |  __/ |  | | (_| | | |__| / ____ \| |  | |
|______|_|_.__/|_|  \___|_|  |_|\__,_|  \____/_/    \_\_|  |_|


)";



// ========================================
// ================ MAIN ==================
// ========================================
int main() {
    std::cout << BANNER << std::endl;
    int opcion = -1;

    while (opcion != 0) {
        std::cout << "1. Agregar libro\n"
                  << "2. Agregar usuario\n"
                  << "3. Prestar libro\n"
                  << "4. Devolver libro\n"
                  << "5. Mostrar cantidad de libros disponibles\n"
                  << "6. Buscar libros por autor\n"
                  << "7. Buscar usuario por nombre y apellidos\n"
                  << "0. Salir\n"
                  << "Seleccione una opción: ";
        std::cin >> opcion; std::cin.ignore();

        switch (opcion) {
            case 1: agregarLibro();              break;
            case 2: agregarUsuario();            break;
            case 3: prestarLibro();              break;
            case 4: devolverLibro();             break;
            case 5:
                std::cout << "Libros disponibles: "
                          << contarLibrosDisponibles() << "\n";
                break;
            case 6: buscarPorAutor();            break;
            case 7: buscarUsuarioPorNombre();    break;
            case 0:
                std::cout << "Saliendo del sistema. ¡Hasta luego!\n";
                break;
            default:
                std::cout << "Opción inválida. Intente de nuevo.\n";
        }
        std::cout << "\n";
    }
    return 0;
}
