#pragma once

#include "Base.h"

double winningRate(int i_winner, int j_winner, int i, int j)
{
	//Calcul de nu
	double nu(sqrt((i-i_winner)*(i-i_winner)+(j-j_winner)*(j-j_winner)));	
	if(nu <= R)
	{
		return(1 - nu / R);
	}
	else
	{
		return 0;
	}
}

void learnProtoKohonen(double alpha, Prototypes::imagette& w, const uci::Database::imagette& xi, const int& i, const int& j, const int k, const int l)
{
	for(int u=0 ; u < uci::Database::imagette::height ; ++u)
	{
		for(int v=0 ; v < uci::Database::imagette::width ; ++v)
		{
			w(u,v) = w(u,v) + alpha * winningRate(i, j, k, l) * (xi(u,v) - w(u,v));
		}
	}
}
