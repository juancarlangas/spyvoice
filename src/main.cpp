#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>

#define EMITIDAS 2439
#define PRIMER_ANO_XML 2013
#define PRIMER_MES_XML 6

struct Factura {
	std::string codigo;
	int32_t ano;
	int32_t mes;
};

int32_t main()
{
	// LEYENDO DE LA TABLA DE EXCEL
	std::ifstream tabla_excel { "/home/juancarlangas/Roge/Facturación/SOLICITAR/report.csv" };
	std::string linea;
	Factura *emitida = new Factura [2439] ();
	int32_t emitidas { 0 };

	if ( tabla_excel.fail() ) {
		std::cerr << "No se pudo abrir tu perro archivo" << std::endl;
		exit( EXIT_FAILURE );
	}

	for ( int32_t i = 0; i < 8; ++i )
		std::getline( tabla_excel, linea );

	while( std::getline( tabla_excel, linea ) and !linea.empty() ) {
		emitida[ emitidas ].codigo = linea.substr( 0, 10 ); // codigo
		linea = linea.substr( 36 ); // TRASH
		linea = linea.substr( linea.find_first_of( ',' ) + 1 );
		emitida[ emitidas ].ano = std::stoi( linea.substr( 0, 4 ) );
		emitida[ emitidas ].mes = std::stoi( linea.substr( 5, 2 ) );
		++emitidas;
	}

	tabla_excel.close();

	if ( EMITIDAS != emitidas ) {
		std::cerr << "La constante EMITIDAS no coincide con el conteo interno" << std::endl;
		exit( EXIT_FAILURE );
	}

	// RECIBIDAS
	Factura *recibida_PDF = new Factura [EMITIDAS] ();
	int32_t recibidas_PDF { 0 };
	std::string path { "/home/juancarlangas/Roge/Facturación" };
	std::string cadena;
	int32_t ano;
	int32_t mes;

	for ( const auto & file : std::filesystem::recursive_directory_iterator( path ) ) {
		cadena = file.path();

		if ( cadena.find( "HERBALIFE" ) != std::string::npos ) {
			ano = std::stoi( cadena.substr( 38, 4 ) );
			mes = std::stoi( cadena.substr( 43, 2 ) );
			if ( ( ano == 2012 and mes >= 9 ) or ( ano >= 2013 ) ) {
				if ( cadena.ends_with( ".pdf" ) or cadena.ends_with( ".PDF" )) {
					recibida_PDF[ recibidas_PDF ].ano = ano;
					recibida_PDF[ recibidas_PDF ].mes = mes;
					recibida_PDF[ recibidas_PDF ].codigo = cadena.substr( 56, 10 );
					++recibidas_PDF;
				}
			}
		}
	}
	// RESULTADOS
	std::cout << "\nEmitidas: " << emitidas;
	std::cout << "\nRecibidas PDF: " << recibidas_PDF;
	std::cout << std::endl;

	// ANÁLISIS
	int32_t i, j;

	Factura *completada = new Factura [EMITIDAS] ();
	int32_t completadas { 0 };
	Factura *faltante = new Factura [EMITIDAS] ();
	int32_t faltantes { 0 };
	Factura *movida = new Factura [EMITIDAS] ();
	int32_t movidas { 0 };

	for ( i = 0; i < EMITIDAS; ++i ) {
		j = 0;
		while ( j < recibidas_PDF and emitida[i].codigo != recibida_PDF[j].codigo )
			++j;

		if ( j == recibidas_PDF ) {
			faltante[ faltantes ].ano = emitida[ i ].ano;
			faltante[ faltantes ].mes = emitida[ i ].mes;
			faltante[ faltantes ].codigo = emitida[ i ].codigo;
			++faltantes;
		}
		else if (	emitida[ i ].ano == recibida_PDF[ j ].ano and
					emitida[ i ].mes == recibida_PDF[ j ].mes ) {
			completada[ completadas ].ano = emitida[ i ].ano;
			completada[ completadas ].mes = emitida[ i ].mes;
			completada[ completadas ].codigo = emitida[ i ].codigo;
			++completadas;
		}
		else {
			movida[ movidas ].ano = emitida[ i ].ano;
			movida[ movidas ].mes = emitida[ i ].mes;
			movida[ movidas ].codigo = emitida[ i ].codigo;
			++movidas;
		}
	}
	std::cout << "\nCompletadas: " << completadas;
	std::cout << "\nMovidas: " << movidas;
	std::cout << "\nFaltantes: " << faltantes;
	std::cout << '\n' << std::endl;

	// RESULTADOS
	std::ofstream completadas_file{ "/home/juancarlangas/Desktop/completadas.csv" };
	if ( completadas_file.fail() ) {
		std::cerr << "No se puede escribir el archivo de resultados mi chingón" << std::endl;
		exit( EXIT_FAILURE );
	}
	for ( i = 0; i < completadas; ++i )
		completadas_file << completada[i].ano << ',' << completada[i].mes << ',' << completada[i].codigo
			<< '\n';
	completadas_file.close();

	std::ofstream faltantes_file{ "/home/juancarlangas/Desktop/faltantes.csv" };
	if ( faltantes_file.fail() ) {
		std::cerr << "No se puede escribir el archivo de resultados mi chingón" << std::endl;
		exit( EXIT_FAILURE );
	}
	for ( i = 0; i < faltantes; ++i )
		faltantes_file << faltante[i].ano << ',' << faltante[i].mes << ',' << faltante[i].codigo
			<< '\n';
	faltantes_file.close();

	std::ofstream movidas_file{ "/home/juancarlangas/Desktop/movidas.csv" };
	if ( movidas_file.fail() ) {
		std::cerr << "No se puede escribir el archivo de resultados mi chingón" << std::endl;
		exit( EXIT_FAILURE );
	}
	for ( i = 0; i < movidas; ++i )
		movidas_file << movida[i].ano << ',' << movida[i].mes << ',' << movida[i].codigo
			<< '\n';
	movidas_file.close();

	return EXIT_SUCCESS;
}
