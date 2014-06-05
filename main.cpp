#include <iostream>
#include <list>
#include <boost/tokenizer.hpp>
#include <iomanip>      // std::setprecision
#include <fstream>

#include "Parser_frame.hpp"

using namespace std;

/**
 *  Declaration des variables global
 */

int nbrClient;
int nbrTrajet;
float coutTransport;
int capacite;

typedef struct client{
	int numClient;
	float distance;
	float cout;
}Client;
list<Client> cl;

typedef struct echeance{
	int i;
	float cli;
	float di;
}TEcheance;
list<TEcheance> echeance;

typedef struct groupement{
	int cli;
	float datDeb;
	float datFin;
	int nbrColis;
}Groupement;
list<Groupement> groupement;

/**
 *  fonction permettant d'ajouter des valeurs à notre liste d'échancier
 */
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

Client ajoutClient(int numClient, float cout, float distance){
	Client client;
	
	client.numClient = numClient;
	client.distance = distance;
	client.cout = cout;
	
	return client;
}

void afficheClient(list<Client> client){
	if(!client.empty()){
		list<Client>::const_iterator cit=client.begin() ;
		for(;cit != client.end();cit++)
			cout << "Client numero : " << cit->numClient << " cout : " << cit->cout << " distance : " << cit->distance << endl;
	}
	else
		cout << "aucun client"<<endl;
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

/**
 *  afficher la liste "echeance"
 */
void afficheEcheance(list<TEcheance> echeance){
	cout << "i - cli - di" << endl;
	
	list<TEcheance>::const_iterator lit=echeance.begin() ;
	for(;lit!=echeance.end();++lit)
		cout << lit->i << " - " << lit->cli << "\t- " << lit->di << endl;
}

void affiche(){
	cout << "nbrTrajet : " << nbrTrajet <<endl;
	cout << "nbrClient : " << nbrClient <<endl;
	cout << "transporteur capaciter : " << capacite <<endl;
	cout << "cout transport : " << coutTransport <<endl;

	afficheClient(cl);
	afficheEcheance(echeance);
}

/**
 *  Fonction tri par date
 *  On recupere en argument une liste d'une structure d'echeance que l'on tri en fonction de la date d'echeance
 *  Puis nous retournons la nouvelle liste
 */
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

/**
 *	Groupe une liste de client a partir du debut inclu et d'une fin exclu
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
	afficheEcheance(temp);
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

string split( const string & Msg, const string & Separators, int position, int ligne) {
	// typedef pour alléger l'écriture
	typedef boost::tokenizer<boost::char_separator<char> > my_tok;
	// séparateur personnalisé
	boost::char_separator<char> sep( Separators.c_str() );
	// construire le tokenizer personnalisé
	my_tok tok( Msg, sep );
	
	// itérer la séquence de tokens
	string chaine;
	int j=0;
	for ( my_tok::const_iterator i = tok.begin(); i != tok.end(); ++i ) {
		// afficher chaque token extrait
		if( j == position)
			chaine = *i;
		j++;
	}

	switch (ligne) {
		case 0:
			nbrTrajet = atoi(chaine.c_str());
			break;
		case 1:
			nbrClient = atoi(chaine.c_str());
			break;
		case 2:
			capacite = atoi(chaine.c_str());
			break;
		case 3:
			coutTransport = atof(chaine.c_str());
			break;
		default:
			return chaine;
			break;
	}
	return chaine;
}


bool lectureInstance(const char * argv[]){
	/**
	 *  initialisation du tampon du fichier contenant l'instance
	 */
	filebuf fb;
	
	if(!fb.open(argv[1], ios::in)) {
		cout << "Error while opening file : " << argv[0]<< endl;
		return EXIT_FAILURE;
	}
	
	/**
	 *  initialisation du flux d'entrée à partir du tampon
	 */
	istream is(&fb);
	string str;
	int ligne = 0;
	while(ligne < 4){
		if (getline(is, str)) {
			split(str, ":", 1, ligne);
		}
		ligne++;
	}
	
	int numCl = 0;
	float coutTrans = 0.0, distance = 0.0;
	
	for( int position = 0; position<nbrClient; position++){
		if(getline(is, str))
			numCl = atoi(split(str, ":", 1, -1).c_str() );
		if(getline(is, str))
			coutTrans = atof(split(str, ":", 1, -1).c_str() );
		if(getline(is, str))
			distance = atof(split(str, ":", 1, -1).c_str() );
		cl.push_back(ajoutClient(numCl, coutTrans, distance));
	}
	
	string chaine1, chaine2;
		if(getline(is, str))
			chaine1 = split(str, ":", 1, -1);
		if(getline(is, str))
			chaine2 = split(str, ":", 1, -1);
	
	//cout << "test " << split(chaine2, ";",3, -1 ) << " --> " << atof(split(chaine2, ";",3, -1 ).c_str()) << endl << endl;
	
	for(int i=0; i<nbrTrajet;i++)
		echeance.push_back(ajoutList(i, atof(split(chaine1, ";",i, -1 ).c_str() ), atof(split(chaine2, ";",i, -1 ).c_str()) ) );

	return true;
}


//Pour la structure de données, vaut-il mieux partir sur 3 listes donnant chacune une ligne du tableau ?

int main(int argc, const char * argv[]) {

	/*
	int client[nbrClient] = {100,100,100,100};
	list<TEcheance> echeance;
	echeance.push_back(ajoutList(1, 2, 1000));
	*/
	
	/**
	 *  on passe en argument de ligne de commande le chemin vers le fichier décrivant l'instance
	 */
	if(argc != 2) {
		cout << "Usage : " << argv[0] << " path_to_instance " << endl;
		return EXIT_FAILURE;
	}
	
	
	if(lectureInstance(argv)==false)
		EXIT_FAILURE;
	
	affiche();
	
	cout << endl << "avant tri : " << endl;
	afficheEcheance(echeance);
	
	triList(&echeance);
	
	cout << endl << "apres tri : " << endl;
	afficheEcheance(echeance);
		
	//fonction qui grouperai les clients si retourne 0 alors aucun groupe dans cette interval
	int var = groupList(echeance, 700, 1300);
	cout << endl << "nombre de groupe : " << var << endl;
	
    return 0;
}

