#include <iostream>
#include <fstream> //leer y escribir archivos
#include <sstream> 
#include <string>


const int MAX_PRODUCTOS = 100;
const std::string NOMBRE_ARCHIVO = "inventario.txt";
const int UMBRAL_BAJO_STOCK = 10;
int numProductos = 0; 


int codigos[MAX_PRODUCTOS];
std::string nombres[MAX_PRODUCTOS];
int stocks[MAX_PRODUCTOS];
float precios[MAX_PRODUCTOS];
std::string ubicaciones[MAX_PRODUCTOS];


void mostrarMenu();
bool cargarInventario();
void guardarInventario();
int buscarProductoPorCodigo(int codigo);
int buscarProductoPorUbicacion(std::string ubicacion); 
void registrarNuevoProducto();
void actualizarInventarioPorUbicacion();
void generarReporteBajoStock();
void encontrarProductoMasBarato();
void limpiarBuffer(); 


int main() {
   
    cargarInventario(); 

    int opcion;

    do {
        mostrarMenu();
        std::cout << "Opcion seleccionada: ";
        std::cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
            case 2:
                actualizarInventarioPorUbicacion();
                break;
            case 3:
                registrarNuevoProducto();
                break;
            case 4:
                generarReporteBajoStock();
                break;
            case 5:
                encontrarProductoMasBarato();
                break;
            case 6:
                guardarInventario();
                std::cout << "Saliendo del sistema, cambios guardados." << std::endl;
                break;
            default:
                std::cout << "Opcion no valida." << std::endl;
                break;
        }

        if (opcion != 6) {
            std::cout << "\n--- Volviendo al Menu ---\n" << std::endl;
        }

    } while (opcion != 6);

    return 0;
}


void mostrarMenu() {
    std::cout << "\n--- MENU PRINCIPAL DE INVENTARIO ---\n";
    std::cout << "2. Actualizar stock por ubicacion\n";
    std::cout << "3. Registrar nuevo producto\n";
    std::cout << "4. Reporte de bajo stock \n";
    std::cout << "5. Encontrar el producto mas barato\n";
    std::cout << "6. Guardar y Salir\n";
    std::cout << "------------------------------------\n";
}


bool cargarInventario() {
    std::cout << "Sistema de Inventario 'El Martillo'\n";
    std::cout << "Intentando cargar inventario desde " << NOMBRE_ARCHIVO << "...\n";

    std::ifstream archivoEntrada(NOMBRE_ARCHIVO);
    std::string linea;
    numProductos = 0;

    if (!archivoEntrada.is_open()) {
        std::cout << "Archivo no encontrado. Iniciando vacio.\n";
        return false;
    }

    while (std::getline(archivoEntrada, linea) && numProductos < MAX_PRODUCTOS) {
        std::stringstream ss(linea);
        std::string segmento;
        
       
        std::getline(ss, segmento, ',');
        codigos[numProductos] = std::stoi(segmento);

       
        std::getline(ss, nombres[numProductos], ',');
        
      
        std::getline(ss, segmento, ',');
        stocks[numProductos] = std::stoi(segmento);

        std::getline(ss, segmento, ',');
        precios[numProductos] = std::stof(segmento);

       
        std::getline(ss, ubicaciones[numProductos], ',');
        
        numProductos++;
    }

    archivoEntrada.close();
    std::cout << numProductos << " productos cargados con exito.\n\n";
    return true;
}


void guardarInventario() {
    std::cout << "Guardando cambios...\n";
    std::ofstream archivoSalida(NOMBRE_ARCHIVO);

    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo para guardar. No se guardo nada.\n";
        return;
    }

    for (int i = 0; i < numProductos; ++i) {
        
        archivoSalida << codigos[i] << ","
                      << nombres[i] << ","
                      << stocks[i] << ","
                      << precios[i] << ","
                      << ubicaciones[i] << "\n";
    }

    archivoSalida.close();
    std::cout << "Guardado completo.\n";
}


int buscarProductoPorCodigo(int codigo) {
    for (int i = 0; i < numProductos; ++i) {
        if (codigos[i] == codigo) {
            return i;
        }
    }
    return -1;
}


int buscarProductoPorUbicacion(std::string ubicacion) {
    for (int i = 0; i < numProductos; ++i) {
        if (ubicaciones[i] == ubicacion) {
            return i;
        }
    }
    return -1;
}


void registrarNuevoProducto() {
    if (numProductos >= MAX_PRODUCTOS) {
        std::cout << "Inventario lleno. No se puede agregar mas productos.\n";
        return;
    }

    int nuevoCodigo;
    

    do {
        std::cout << "Ingrese Codigo (entero): ";
        if (!(std::cin >> nuevoCodigo)) { 
            std::cout << "ERROR: Debe ser un numero.\n";
            limpiarBuffer();
            continue;
        }
        if (buscarProductoPorCodigo(nuevoCodigo) != -1) {
            std::cout << "ERROR: Codigo ya existe. Pruebe otro.\n";
        } else {
            break; 
        }
    } while (true);
    limpiarBuffer(); 

    std::cout << "Ingrese Nombre: ";
    std::string nuevoNombre;
    std::getline(std::cin, nuevoNombre);
    
    std::cout << "Ingrese Stock: ";
    std::cin >> stocks[numProductos];
    
    std::cout << "Ingrese Precio: $";
    std::cin >> precios[numProductos];
    limpiarBuffer();
    
    std::cout << "Ingrese Ubicacion:";
    std::getline(std::cin, ubicaciones[numProductos]);

    
    codigos[numProductos] = nuevoCodigo;
    nombres[numProductos] = nuevoNombre;
    
    
    numProductos++; 
    std::cout << "\nProducto registrado.\n";
}



void actualizarInventarioPorUbicacion() {
    std::string ubicacionBuscada;
    int cantidadCambio;

    std::cout << "Ingrese la UBICACION a buscar: ";
    std::getline(std::cin, ubicacionBuscada);

    int indice = buscarProductoPorUbicacion(ubicacionBuscada);

    if (indice != -1) {
        std::cout << "Producto: " << nombres[indice] << ", Stock actual: " << stocks[indice] << std::endl;
        std::cout << "Cuanto desea agregar o quitar? ";
        std::cin >> cantidadCambio;
        limpiarBuffer();

        int nuevoStock = stocks[indice] + cantidadCambio;
        
        if (nuevoStock >= 0) {
            stocks[indice] = nuevoStock;
            std::cout << "Stock actualizado. Nuevo stock para " << nombres[indice] << " es " << stocks[indice] << ".\n";
        } else {
            std::cout << "ERROR: El stock no puede ser negativo. Operacion cancelada.\n";
        }
    } else {
        std::cout << "ERROR: Ubicacion no encontrada.\n";
    }
}


void generarReporteBajoStock() {
    std::cout << "\n--- REPORTE BAJO STOCK ---\n";
    std::cout << "Productos con menos de " << UMBRAL_BAJO_STOCK << " unidades:\n";
    bool encontrado = false;

    for (int i = 0; i < numProductos; ++i) {
        if (stocks[i] < UMBRAL_BAJO_STOCK) {
            std::cout << "-> Nombre: " << nombres[i] << ", Stock: " << stocks[i] 
                      << ", Ubicacion: " << ubicaciones[i] << std::endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        std::cout << "Todo el inventario esta por encima del umbral.\n";
    }
    std::cout << "--------------------------\n";
}


//falto agregar funcion para encontrar el mas barato


void limpiarBuffer() {
    if (std::cin.fail()) {
        std::cin.clear();
    }
    std::cin.ignore(10000, '\n'); 
}


