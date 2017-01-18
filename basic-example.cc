#include "Base.h"

// On va stocker les imagettes-prototypes au sein d'une grille.
#define WIDTH  5
#define HEIGHT 4
typedef uci::Map<WIDTH,HEIGHT,
	uci::Database::imagette::width,
	uci::Database::imagette::height> Prototypes;

#include "k-means-online.cpp"
// Pour une imagette, les indices sont definis comme suit
//
//         j
//         |
//   ......|.....
//   ......|.....
//   ......|.....
//   ......|.....
//   ......|.....
//   ......#------ i   img(i,j)
//   ............
//   ............
//   ............
//   ............
//   ............  


// Cette fonction permet d'affecter un prototype (dont les pixels sont
// des double dans [0,255]) a une imagette tiree de la base (dont les
// pixels sont des unsigned char). Le & evite les copies inutiles.
void initProto(Prototypes::imagette& w,
		const uci::Database::imagette& xi) {
	for(int i = 0 ; i < uci::Database::imagette::height ; ++i)
		for(int j = 0 ; j < uci::Database::imagette::width ; ++j)
			w(i,j) = (double)(xi(i,j));
}

int main(int argc, char* argv[]) {
	Prototypes prototypes;

	// On peut afficher la grille de prototypes.
	prototypes.PPM("proto",1);

	// Utilisons la base de donnees.
	uci::Database database;

	int i(0);
	int j(0);
	int cpt(0);  

	// Pour obtenir une nouvelle imagette...
	while(cpt<100){

		database.Next();

		uci::Database::imagette& xi = database.input; // le & fait que xi est un pointeur, on evite une copie.
		std::cout << "L'imagette tiree de la base est un " << database.what << std::endl;

		winnerProto(prototypes, xi, i, j);
		learnProto(10e-2,prototypes(i,j),xi);
		prototypes.PPM("proto",cpt + 2);

		cpt++;
	}

	//Ici on va utiliser une initialisation plus smart
	for(int u=0 ; u < HEIGHT ; ++u)
	{
		for(int v=0 ; v < WIDTH ; ++v)
		{
			database.Next();
	uci::Database::imagette& xi = database.input; // le & fait que xi est un pointeur, on evite une copie.
	std::cout << "L'imagette tiree de la base pour initiliaser les prototypes est un " << database.what << std::endl;
			initProto(prototypes(u,v),xi);
		}
	} 
	prototypes.PPM("proto",cpt +2);
	++cpt;
	//Ensuite on relanc les k-means sur cette nouvelle initialisation

	while(cpt<600){

		database.Next();

		uci::Database::imagette& xi = database.input; // le & fait que xi est un pointeur, on evite une copie.
		std::cout << "L'imagette tiree de la base est un " << database.what << std::endl;

		winnerProto(prototypes, xi, i, j);
		learnProto(10e-2,prototypes(i,j),xi);
		prototypes.PPM("proto",cpt + 2);

		cpt++;
	}
	// Utilisons notre fonction de copie pour affecter un prototype à la valeur de l'imagette.
	//initProto(prototypes(2,3),xi);

	// On affiche a nopuveau la grille de prototypes.
	//prototypes.PPM("proto",2);

	// Pour obtenir une nouvelle imagette...
	//database.Next();

	// Utilisons notre fonction de copie pour affecter un prototype à la valeur de l'imagette.
	//initProto(prototypes(1,1),xi);

	// On affiche a nopuveau la grille de prototypes.
	//prototypes.PPM("proto",3);

	return 0;
}


