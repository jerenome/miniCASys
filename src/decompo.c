/* This program is free software. It comes WITHOUT ANY WARRANTY, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://wtfpl.net for more details. */
/**
 * \file decompo.c
 * \brief Décomposition en LU (code source)
 * \author Jérémy Meyer
 */
 
#include "decompo.h"

LUM* decomposition(Matrix *m)
{
	int n = nbLignes(m);
	E somme=0.;
	E somme2=0.;
	LUM* raiponce = MALLOCN(raiponce, 2);
	
	if (raiponce != NULL)
    {
        Matrix * l = newMatrix(n,n); // L
        Matrix * u = newMatrix(n,n); // U

        setElt(u,1,1,getElt(m,1,1));


        for (int j = 2 ; j<=n ; j++)
        {
            setElt(u,1,j,getElt(m,1,j));
            setElt(l,j,1,getElt(m,j,1)/getElt(m,1,1));
        }

        for (int i = 2 ; i<n ; i++)
        {
            for (int k = 1 ; k<i ; k++)
            {
                somme += getElt(l,i,k)*getElt(u,k,i);
            }

            setElt(u,i,i,getElt(m,i,i)-somme);
            somme=0.;

            for (int j = i+1 ; j<=n ; j++)
            {
                for (int k = 1 ; k<i ; k++)
                {
                    somme += getElt(l,i,k)*getElt(u,k,j);
                    somme2 += getElt(l,j,k)*getElt(u,k,i);
                }

                setElt(u,i,j,getElt(m,i,j)-somme);
                setElt(l,j,i,1./getElt(u,i,i)*(getElt(m,j,i)-somme2));
            }

            somme=0.;
            somme2=0.;
        }

        for (int k = 1 ; k<n ; k++)
        {
            somme += getElt(l,n,k)*getElt(u,k,n);
        }

        setElt(u,n,n,getElt(m,n,n)-somme);

        for (int i = 1; i <= n; i++)
            setElt(l,i,i,1.);

        raiponce[0]= l;
        raiponce[1] = u;
    }
    else
        perror("malloc");
		
	return raiponce;
}

void afficheL(LUM* lum)
{
    printf("\tL :\n");
	displayMatrix(lum[0]);
	return;
}

void afficheU(LUM* lum)
{
    printf("\tU :\n");
	displayMatrix(lum[1]);
	return;
}

void afficheLU(LUM* lum)
{
    afficheL(lum);
    printf("\n\n");
    afficheU(lum);
}

void libererLU(LUM* lum)
{
	deleteMatrix(lum[0]);
	deleteMatrix(lum[1]);
	free(lum);
	return;
}

Matrix * inverseL (Matrix * l)
{
	int n = nbLignes(l);
	Matrix * lprime = copieMatrice(l);
	Matrix * solve = identite(n);
	
	for (int i = 2 ; i<=n ; i++)
	{
		for (int j = 1 ; j<i ; j++)
		{
			if (getElt(lprime,i,j) != 0)
			{
				int multiple = -1.*getElt(lprime,i,j);
				lprime = addMultLigne(lprime, i, j, multiple);
				solve = addMultLigne(solve,i,j,multiple);
			}
		}
	}
	
	return solve;
}
				
