#include <iostream>
#include <list>
#define _NBRCLIENTS 4

using namespace std;

typedef struct echeance{
	int i;
	int cli;
	int di;
}TEcheance;

typedef struct groupement{
	int cli;
	int datDeb;
	int datFin;
	int nbrColis;
}Groupement;

//petite fonction permettant d'ajouter des valeurs à notre liste d'échancier
TEcheance ajoutList(int i, int cli, int di){
	TEcheance temp;
	
	temp.i = i;
	temp.cli = cli;
	temp.di = di;
	
	return temp;
}

Groupement ajoutGroupe(int cli, int datDeb, int datFin, int nbrColis){
	Groupement groupe;
	
	groupe.cli = cli;
	groupe.datDeb = datDeb;
	groupe.datFin = datFin;
	groupe.nbrColis = nbrColis;

	return groupe;
}

void afficheClient(int client[_NBRCLIENTS]){
	for(int i=0; i< _NBRCLIENTS; i++)
		cout << "Le cout de la distance entre le client" << i+1 << " et l'entrepot est de " << client[i] << endl ;
}

void afficheGroupe(list<Groupement> groupe){
	if(!groupe.empty()){
		cout << "cli - date Debut - date Fin - nbr Colis a livrer" << endl;
		list<Groupement>::const_iterator git=groupe.begin() ;
		for(;git != groupe.end();git++)
			cout << git->cli << "\t-\t  " << git->datDeb << "\t -   " << git->datFin << "\t- " << git->nbrColis << endl;
	}
	else
		cout << "groupe vide"<<endl;
}

//Permet d'afficher la liste "echeance"
void afficheList(list<TEcheance> echeance){
	cout << "i - cli - di" << endl;
	
	list<TEcheance>::const_iterator lit=echeance.begin() ;
	for(;lit!=echeance.end();++lit)
		cout << lit->i << " - " << lit->cli << "\t- " << lit->di << endl;
}

//Fonction tri par date
//On recupere en argument une liste d'une structure d'echeance que l'on tri en fonction de la date d'echeance
//Puis nous retournons la nouvelle liste
void triList(list<TEcheance>* echeance){
	list<TEcheance> temp;								//créer liste temporaire

	
	temp.push_front(ajoutList(echeance->begin()->i,echeance->begin()->cli,echeance->begin()->di)) ;	//ajoute le premier element de echeance a la nouvelle liste temp
	list<TEcheance>::const_iterator lit=echeance->begin();										//initialise le curseur sur le premier element de echeance
	lit++;
	while(lit!=echeance->end()){
		int test = 0;
		for(list<TEcheance>::const_iterator it=temp.begin();it!=temp.end();it++){
			if(lit->di <= it->di && test == 0){
				//si nouvel element supérieur ou egal a element liste temporaire alors ajouter juste avant :)
				temp.insert(it, ajoutList(lit->i,lit->cli,lit->di) );	//ajoute le premier element de echeance a la nouvelle liste temp
				test = 1;
			}
		}
		if(test==0)
			temp.push_back(ajoutList(lit->i,lit->cli,lit->di));
		lit++;
	}
	//le but étant de créer une nouvelle liste dans laquelle nous y insereront un par un nos element en fonction de la valeur de di

	echeance->assign(temp.begin(), temp.end());				//remplace tous les elements de la liste echeance par la liste temporaire
}

/*
 *
 *	Groupe une liste de client a partir du debut inclu et d'une fin exclu
 *
 */
int groupList(list<TEcheance> echeance, int deb, int fin){
	list<TEcheance> temp;
	list<TEcheance>::const_iterator it= echeance.begin();
	list<TEcheance>::const_iterator lit;
	int nbrGroupe = 0;
	
	//identifie le debut de la future liste a grouper
	int test =0;
	while(it != echeance.end() && test == 0){
		if(it->di >= deb )
			test = 1;
		else
			it++;
	}
	lit=it;		//memorise position de debut pour groupement
	
	//identifie la fin de la future liste a grouper
	test =0;
	while(it != echeance.end() && test == 0){
		if(it->di >= fin )
			test = 1;
		else
			it++;
	}
	temp.assign(lit, it);		//creer une liste restreinte sur laquel nous devons effectuer le groupement
	
	cout << endl << "groupement" << endl;
	afficheList(temp);
	cout << endl;
	
	if(temp.empty())
		return nbrGroupe;

	//Nous allons maintenant effectuer une liste de client grouper
	list<Groupement> groupe;
	it = temp.begin();					//iterateur sur la liste a grouper
		
	for(; it!= temp.end();it++){
		list<Groupement>::iterator git=groupe.begin();
		test = 0;
		while(git != groupe.end() && test == 0){		//parcours de la liste du groupe
			if(git->cli == it->cli){				//si client deja dans un groupe alors
				test = 1;
				git->nbrColis++;					//incrémenter le nomnbre de livraison
				if(it->di < git->datDeb)			//defini date debut
					git->datDeb = it->di;
				if(it->di > git->datFin)			//defini date fin
					git->datFin = it->di;
			}
			git++;
		}
		if(test == 0){			//si client n'est pas dans un groupe alors
			groupe.push_back(ajoutGroupe(it->cli, it->di, it->di, 1));
			nbrGroupe++;
		}
	}
	afficheGroupe(groupe);
	return nbrGroupe;
}

//Fonction qui calcule le coût d'une livraison en fonction de la date à laquelle elle arrive
//On passe en argument la date, et les numéros de livraisons qu'on a regroupées ensemble
int coutStockage(int date, list<int> *livraisons) {
	//On récupère la date due de chaque livraison présente dans la liste.
	//Pour chaque, on fait la soustraction par la date
	//On additionne les résultats ensemble au fur et à mesure
	//Une fois toutes les différences de dâtes parcourues, on multiplie par Beta x le nombre de livraisons
	//On retourne ce résultat qui est le coût si cette livraison groupée arrive à cette date.
	
	return 1;
}

//Cependant, il reste à créer une fonction qui permette de prendre en compte le coût de livraison.
//Ainsi, le coût de livraison dépend seulement du client et de la distance qui le sépare de l'entrepôt.
//On peut aussi les résumer à un tableau de distances dont l'index donnerait la distance A/R à un client.



//Pour la structure de données, vaut-il mieux partir sur 3 listes donnant chacune une ligne du tableau ?

int main(int argc, const char * argv[]) {

	int client[_NBRCLIENTS] = {100,100,100,100};
	list<TEcheance> echeance;

	echeance.push_back(ajoutList(1, 1, 250));
	echeance.push_back(ajoutList(2, 1, 250));
	echeance.push_back(ajoutList(3, 3, 240));
	echeance.push_back(ajoutList(4, 3, 300));
	echeance.push_back(ajoutList(5, 3, 340));

	afficheClient(client);
	
	cout << endl << "avant tri : " << endl;
	afficheList(echeance);
	
	triList(&echeance);
	
	cout << endl << "apres tri : " << endl;
	afficheList(echeance);
		
	//fonction qui grouperai les clients si retourne 0 alors aucun groupe dans cette interval
	int var = groupList(echeance, 200, 400);
	cout << endl << "nombre de groupe : " << var << endl;

    return 0;
}

