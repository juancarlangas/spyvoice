#include <cstdlib>
#include <iostream>
#include <fstream>

int32_t main()
{
	std::ifstream tabla_excel { "/home/juancarlangas/Roge/Facturación/SOLICITAR/report.csv" };

	if ( tabla_excel.fail() ) {
		std::cerr << "No se pudo abrir tu perro archivo" << std::endl;
		exit( EXIT_FAILURE );
	}

	tabla_excel.close();
}
