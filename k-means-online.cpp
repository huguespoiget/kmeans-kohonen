#pragma once
#include "Base.h"
#include <cmath>
#include <limits>

void learnProto(double alpha, Prototypes::imagette& w, const uci::Database::imagette& xi)
{
	for(int i=0 ; i < uci::Database::imagette::height ; ++i)
	{
		for(int j=0 ; j < uci::Database::imagette::width ; ++j)
		{
			w(i,j) = w(i,j) + alpha * (xi(i,j) - w(i,j));
		}
	}
}
double distanceProto(const Prototypes::imagette& w, const uci::Database::imagette& xi)
{
	//Here we evaluate the distance between the prototype and the imagette from the database
	//given by the law p
	double distancePrototype(0);
	for(int i=0 ; i < uci::Database::imagette::height ; ++i)
	{
		for(int j=0 ; j < uci::Database::imagette::width ; ++j)
		{
			//Distance bete
			//distancePrototype += std::abs(w(i,j)-xi(i,j));
			//Distance euclidienne
			distancePrototype += (w(i,j) - xi(i,j)) * (w(i,j) - xi(i,j));
		}
	}
	//return distancePrototype;
	return sqrt(distancePrototype);
}
void winnerProto(const Prototypes& protos, const uci::Database::imagette& xi, int& i, int& j)
{
	//Le passage par référence de i et j permettent de faire passer l'information
	//sur le winner sans avoir à renvoyer quoi que ce soit par la fonction
	//Il vaut mieux ne pas présupposer qu'il y a une valeur, on peut donc initialiser
	//à une valeur absurde, l'infini dans notre cas
	//double distanceMinimale(distanceProto(protos*(0,0),xi);
	double distanceMinimale(std::numeric_limits<double>::max());
	for(int u=0 ; u < HEIGHT ; ++u)
	{
		for(int v=0 ; v < WIDTH ; ++v)
		{
			if(distanceProto(protos(u,v),xi)<distanceMinimale)
			{
				distanceMinimale = distanceProto(protos(u,v), xi);
				i = u;
				j = v;
			}
		}
	} 
}
