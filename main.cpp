//
//  main.cpp
//  AG41
//
//  Created by djamel on 01/06/2014.
//  Copyright (c) 2014 djamel. All rights reserved.
//

#include <iostream>
using namespace std;

//Pour la structure de données, vaut-il mieux partir sur 3 listes donnant chacune une ligne du tableau ?

int main(int argc, const char * argv[])
{

	//fonction tri par date
	
	//fonction qui grouperai les clients
	
	//fonction qui calculerai les couts, qui serai enregistrer et comparer	
	
    return 0;
}

//Fonction qui calcule le coût d'une livraison en fonction de la date à laquelle elle arrive
//On passe en argument la date, et les numéros de livraisons qu'on a regroupées ensemble
int coutStockage(int date,list<int> *livraisons){
    //On récupère la date due de chaque livraison présente dans la liste.
        //Pour chaque, on fait la soustraction par la date
        //On additionne les résultats ensemble au fur et à mesure
    //Une fois toutes les différences de dâtes parcourues, on multiplie par Beta x le nombre de livraisons
    //On retourne ce résultat qui est le coût si cette livraison groupée arrive à cette date.
}

//Cependant, il reste à créer une fonction qui permette de prendre en compte le coût de livraison.
//Ainsi, le coût de livraison dépend seulement du client et de la distance qui le sépare de l'entrepôt.
//On peut aussi les résumer à un tableau de distances dont l'index donnerait la distance A/R à un client.
