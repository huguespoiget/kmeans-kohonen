#include "Base.h"

// We place the prototypes in a grid
// On va stocker les imagettes-prototypes au sein d'une grille.
#define WIDTH 12
#define HEIGHT 12
// Need a parameter R for Kohonen
// On positionne le paramètre pour Kohonen
#define R 3
typedef uci::Map<WIDTH,HEIGHT, uci::Database::imagette::width, uci::Database::imagette::height> Prototypes;

//We include the k-means-online.cpp	which contains functions dedicated to k-means-oline algorithm
#include "k-means-online.cpp"
//We include also kohonen.cpp which contains functions dedicated to kohonen algorithm
#include "kohonen.cpp"

// For a picture (or prototype, or dataset entry), we define i,j as below:
// Pour une imagette, les indices sont definis comme suit :
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

//That function allows to assign a prototype (whom pixels are doubles
// in range [0,255]) to a picture from the dataset (whom the pixels
// are unsigned chars). & avoid usless copies.
// Cette fonction permet d'affecter un prototype (dont les pixels sont
// des double dans [0,255]) a une imagette tiree de la base (dont les
// pixels sont des unsigned char). Le & evite les copies inutiles.
void initProto(Prototypes::imagette& w, const uci::Database::imagette& xi) {
	for(int i = 0 ; i < uci::Database::imagette::height ; ++i)
		for(int j = 0 ; j < uci::Database::imagette::width ; ++j)
			w(i,j) = (double)(xi(i,j));
}

int main(int argc, char* argv[]) {
	Prototypes prototypes;

	// We can show the grid of prototypes
	// On peut afficher la grille de prototypes.
	prototypes.PPM("proto",1);

	// We'll use the dataset
	// Utilisons la base de donnees.
	uci::Database database;

	int i(0);
	int j(0);
	int cpt(0);  

	//Ici on va utiliser une initialisation plus smart
	for(int u=0 ; u < HEIGHT ; ++u) {
		for(int v=0 ; v < WIDTH ; ++v) {
			database.Next();
			uci::Database::imagette& xi = database.input; 
			std::cout << "The entry drawn from the dataset to initialize a prototype is a " << database.what << std::endl;
			std::cout << "L'imagette tiree de la base pour initiliaser les prototypes est un " << database.what << std::endl;
			initProto(prototypes(u,v),xi);
		}
	} 

	while(cpt<5000) {
		database.Next();

		uci::Database::imagette& xi = database.input; // le & fait que xi est un pointeur, on evite une copie.
		std::cout << "L'imagette tiree de la base est un " << database.what << std::endl;

		winnerProto(prototypes, xi, i, j);
		learnProtoKmean(10e-2,prototypes(i,j),xi);
		prototypes.PPM("kmeansGrid",cpt);
		
		cpt++;
	}

	for(int u=0 ; u < HEIGHT ; ++u) {
		for(int v=0 ; v < WIDTH ; ++v) {
			database.Next();
			uci::Database::imagette& xi = database.input; 
			std::cout << "The entry drawn from the dataset to initialize a prototype is a " << database.what << std::endl;
			std::cout << "L'imagette tiree de la base pour initiliaser les prototypes est un " << database.what << std::endl;
			initProto(prototypes(u,v),xi);
		}
	} 

	while(cpt<10000) {
		database.Next();

		uci::Database::imagette& xi = database.input; // le & fait que xi est un pointeur, on evite une copie.
		std::cout << "L'imagette tiree de la base est un " << database.what << std::endl;

		winnerProto(prototypes, xi, i, j);
		for(int u=0 ; u < HEIGHT ; ++u)
		{
			for(int v=0 ; v < WIDTH ; ++v)
			{
				learnProtoKohonen(10e-2,prototypes(u,v),xi,i,j,u,v);
			}
		}
		prototypes.PPM("kmeansGrid",cpt);
		
		cpt++;
	}

	return 0;
}


