#include <iostream>
#include <list>					//list
#include <boost/tokenizer.hpp>	// parser
#include <iomanip>				// std::setprecision
#include <fstream>
#include <sys/time.h>

using namespace std;

/**
 *  Declaration des variables global
 */
double texec=0.;

int		nbrClient;
int		nbrTrajet;
int		capacite;
double	coutTransport = 0;		//cout de transport


typedef struct client{
	int numClient;
	double distance;
	double coutClient;
}Client;
list<Client> listClient;

typedef struct echeance{
	int i;			//numéro de livraison
	int cli;		//numéro de client
	double di;		//echeance
}TEcheance;
list<TEcheance> echeance;

typedef struct camion{
	list<TEcheance> aLivrer;		//numéro de livraison a livrer
	int numClient;					//numéro de client
	double	dateDepart = NULL;		//date départ initier à null
	double	coutCamion = 0;			//cout de stockage

}Camion;

typedef struct solution{
	list<Camion> listLivraison;
	double	coutTotal = 0;			//inclu le cout de stockage et le cout de transport
}bestSolution;
solution best_eval;

/**
 *  fonction permettant d'ajouter des valeurs à notre liste d'échancier
 */
TEcheance ajoutEcheance(int i, double cli, double di){
	TEcheance temp;
	
	temp.i = i;
	temp.cli = cli;
	temp.di = di;
	
	return temp;
}

/**
 *  afficher la liste "echeance"
 */
void afficheEcheance(){
	cout << endl << "--- TABLEAU D ECHEANCE ---" << endl << "i - cli - di" << endl;
	
	list<TEcheance>::const_iterator lit=echeance.begin() ;
	for(;lit!=echeance.end();++lit)
		cout << lit->i << " - " << lit->cli << "\t- " << lit->di << endl;
}

Camion ajoutCamion(TEcheance echeance){
	Camion camion;
	
	camion.aLivrer.push_back(echeance);
	camion.numClient = echeance.cli;
	camion.dateDepart = echeance.di;
	
	return camion;
}

void afficheListCamion(){
	if(!best_eval.listLivraison.empty()){
		cout << endl << "--- DETAILS DES CAMIONS SUR LE DEPART ---" << endl;
		list<Camion>::const_iterator cit = best_eval.listLivraison.begin() ;
		for (;cit != best_eval.listLivraison.end(); cit++) {
			cout << "Camion du client : " << cit->numClient << " à livrer avant le : " << cit->dateDepart << "\td'un coup de : " << cit->coutCamion << endl;
			list<TEcheance>::const_iterator tit = cit->aLivrer.begin();
			for (; tit != cit->aLivrer.end(); tit++)
				cout << "\tDate livraison : " << tit->di << endl ;
		}
	}
	else
		cout << "Acune livraison a effectuer"<<endl;
}

Client ajoutClient(int numClient, double coutClient, double distance){
	Client client;
	
	client.numClient = numClient;
	client.distance = distance;
	client.coutClient = coutClient;
	
	return client;
}

void afficheClient(){
	if(!listClient.empty()){
		cout << endl << "--- LISTE DES CLIENTS ---" << endl;
		list<Client>::const_iterator cit=listClient.begin() ;
		for(;cit != listClient.end();cit++)
			cout << "Client numero : " << cit->numClient << " cout : " << cit->coutClient << " distance : " << cit->distance << endl;
	}
	else
		cout << "Aucun client"<<endl;
}

void affiche(){
	cout << "--- DONNER DE DEPART ---" << endl;
	cout << "Nombre de trajet : " << nbrTrajet << endl;
	cout << "Nombre de client : " << nbrClient << endl;
	cout << "Capacité de transport : " << capacite <<endl;
	cout << "Cout transport : " << coutTransport <<endl;
	
	afficheClient();
	afficheEcheance();
	afficheListCamion();
	
	std::cout << endl << "Execution time : " << setprecision(10) << texec << " sec" ;
	
	cout << endl << "--- COUT TOTAL ---" << endl << setprecision(10) << best_eval.coutTotal << endl;
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
	
	int numCl = -1;
	double coutTrans = -1, distance = -1;
	/**
	 *  recupere toutes les informations concernant les nbrClient clients
	 */
	for( int position = 0; position<nbrClient; position++){
		if(getline(is, str))
			numCl = atoi(split(str, ":", 1, -1).c_str() );
		if(getline(is, str)){
			coutTrans = atof(split(str, ":", 1, -1).c_str() );
		}
		if(getline(is, str))
			distance = atof(split(str, ":", 1, -1).c_str() );
		listClient.push_back(ajoutClient(numCl, coutTrans, distance));
	}
	
	if (numCl == -1 || coutTrans == -1 || distance == -1) {
		cout << "Erreur lors de la récupération des informations des clients" << endl;
		EXIT_FAILURE;
	}
	
	/**
	 *  recupere dans deux chaines les infos concernant les job_customer ainsi que job_due_dates
	 */
	string chaine1, chaine2;
	if(getline(is, str))
		chaine1 = split(str, ":", 1, -1);
	if(getline(is, str))
		chaine2 = split(str, ":", 1, -1);
		
	for(int i=0; i<nbrTrajet;i++)
		echeance.push_back(ajoutEcheance(i, strtold(split(chaine1, ";",i, -1 ).c_str(), NULL),strtold(split(chaine2, ";",i, -1 ).c_str(), NULL)));
	
	return true;
}

/**
 *  Fonction tri par date
 *  On recupere en argument une liste d'une structure d'echeance que l'on tri en fonction de la date d'echeance
 *  Puis nous retournons la nouvelle liste
 */
void triList(){
	list<TEcheance> temp;								//créer liste temporaire
	
	
	temp.push_front(ajoutEcheance(echeance.begin()->i,echeance.begin()->cli,echeance.begin()->di)) ;	//ajoute le premier element de echeance a la nouvelle liste temp
	list<TEcheance>::const_iterator lit=echeance.begin();										//initialise le curseur sur le premier element de echeance
	lit++;
	while(lit!=echeance.end()){
		int test = 0;
		for(list<TEcheance>::const_iterator it=temp.begin();it!=temp.end();it++){
			if(lit->di <= it->di && test == 0){
				//si nouvel element supérieur ou egal a element liste temporaire alors ajouter juste avant :)
				temp.insert(it, ajoutEcheance(lit->i,lit->cli,lit->di) );	//ajoute le premier element de echeance a la nouvelle liste temp
				test = 1;
			}
		}
		if(test==0)
			temp.push_back(ajoutEcheance(lit->i,lit->cli,lit->di));
		lit++;
	}
	//le but étant de créer une nouvelle liste dans laquelle nous y insereront un par un nos element en fonction de la valeur de di
	
	echeance.assign(temp.begin(), temp.end());				//remplace tous les elements de la liste echeance par la liste temporaire
}

/**
 *	Groupe client par capacite camion
 */
void defBase(){
	list<TEcheance>::const_iterator lit = echeance.begin();
	for (;lit != echeance.end();lit++){
		if (best_eval.listLivraison.empty())
			best_eval.listLivraison.push_back( ajoutCamion( lit.operator*() ));
		else {
			list<Camion>::iterator cit = best_eval.listLivraison.begin();
			bool test = false;
			while(cit != best_eval.listLivraison.end() && test == false) {
				if (cit->aLivrer.size() < capacite && cit->numClient == lit->cli) {
					cit->aLivrer.push_back(lit.operator*());
					if (cit->dateDepart > lit->di)
						cit->dateDepart = lit->di;
					test = true;
				}
				cit++;
			}
			if (!test) {
				best_eval.listLivraison.push_back( ajoutCamion( lit.operator*() ));
			}
		}
	}
}

void triCamionCoutStockage(solution* soluce) {
	list<Camion> temp;												//créer liste temporaire
	
	list<Camion>::const_iterator lit = soluce->listLivraison.begin();			//inijijiiihitialise le curseur sur le premier element de echeance
	temp.push_front( lit.operator*() ) ;							//ajoute le premier element de echeance a la nouvelle liste temp
	lit++;
	while(lit != soluce->listLivraison.end()){
		int test = 0;
		list<Camion>::const_iterator it = temp.begin();
		for(;it!=temp.end();it++){
			if(lit->coutCamion >= it->coutCamion && test == 0){
				//si nouvel element supérieur ou egal a element liste temporaire alors ajouter juste avant :)
				temp.insert(it, lit.operator*() );	//ajoute le premier element de echeance a la nouvelle liste temp
				test = 1;
			}
		}
		if(test==0)
			temp.push_back( lit.operator*() );
		lit++;
	}
	//le but étant de créer une nouvelle liste dans laquelle nous y insereront un par un nos element en fonction de la valeur de di
	
	soluce->listLivraison.assign(temp.begin(), temp.end());			//remplace tous les elements de la liste echeance par la liste temporaire
}

void calculHoraire(double* tabDist, solution* soluce){
	/**
	 *  calcul l'horaire de depart de tous les camions
	 */
	list<Camion>::iterator cit = soluce->listLivraison.begin();
	cit = soluce->listLivraison.begin();
	list<Camion>::iterator tempCamionIt = cit;
	cit++;
	
	for (; cit != soluce->listLivraison.end(); cit++) {
		cit->dateDepart = tempCamionIt->dateDepart - (tabDist[tempCamionIt->numClient] + tabDist[cit->numClient]) ;
		tempCamionIt = cit;
	}
}

double coutStockage(Camion* structCamion) {
	double beta = 0;
	structCamion->coutCamion = 0;
	
	/**
	 *  récupére le cout à utiliser pour le camion en question
	 */
	list<Client>::const_iterator cit = listClient.begin();
	for (;cit != listClient.end(); cit++){
		if (cit->numClient == structCamion->numClient) {
			beta = cit->coutClient;
			break;
		}
	}
	
	/**
	 *  calcule le cout du camion
	 */
	list<TEcheance>::const_iterator lit = structCamion->aLivrer.begin();
	for (; lit != structCamion->aLivrer.end(); lit++) {
		structCamion->coutCamion = (lit->di - structCamion->dateDepart) + structCamion->coutCamion;
	}
	structCamion->coutCamion = structCamion->coutCamion * beta ;
	
	return structCamion->coutCamion;
}

double coutStockage(Camion* structCamion, double date) {
	double beta = 0;
	structCamion->coutCamion = 0;
	
	/**
	 *  récupére le cout à utiliser pour le camion en question
	 */
	list<Client>::const_iterator cit = listClient.begin();
	for (;cit != listClient.end(); cit++){
		if (cit->numClient == structCamion->numClient) {
			beta = cit->coutClient;
			break;
		}
	}
	
	/**
	 *  calcul en fonction de la date donnée en parametre
	 */
	list<TEcheance>::const_iterator lit = structCamion->aLivrer.begin();
	for (; lit != structCamion->aLivrer.end(); lit++) {
		structCamion->coutCamion = (lit->di - date) + structCamion->coutCamion;
	}
	structCamion->coutCamion = structCamion->coutCamion * beta ;
	
	return structCamion->coutCamion;
}

void ordrePassage(double* tabDist) {
	list<Camion> tempEssai;
	int date = -1;
	/**
	 *  Pour chaque choix de camion fais on vérifie quel prochain choisir
	 */
	while (!best_eval.listLivraison.empty()) {
		list<Camion>::iterator cit = best_eval.listLivraison.begin();
		list<Camion>::iterator temporaire = best_eval.listLivraison.begin();
		
		/**
		 *  lance le calcul du cout de stockage de chaque camion si il part pour arriver a l'heure
		 */
		for (; cit != best_eval.listLivraison.end(); cit++){
			if(date == -1)
				coutStockage( &cit.operator*() );
			else
				coutStockage( &cit.operator*(), date);
		}
		
		/**
		 *  Tri la liste de camion en fonction des couts de stockage
		 */
		triCamionCoutStockage(&best_eval);

		if (date == -1)
			calculHoraire(tabDist, &best_eval);
		else {
			/**
			 *  calcul l'horaire de depart de tous les camions
			 */
			cit = best_eval.listLivraison.begin();
			cit->dateDepart=date;
			list<Camion>::iterator tempCamionIt = cit;
			cit++;
			
			for (; cit != best_eval.listLivraison.end(); cit++) {
				cit->dateDepart = tempCamionIt->dateDepart - (tabDist[tempCamionIt->numClient] + tabDist[cit->numClient]) ;
				tempCamionIt = cit;
			}
		}

		/**
		 *  Calcule le nouveau cout de stockage pour chacun des camions
		 */
		Camion tempCamion = best_eval.listLivraison.front();
		cit = best_eval.listLivraison.begin();
		for (; cit != best_eval.listLivraison.end(); cit++){
			coutStockage( &cit.operator*(), temporaire->dateDepart );
		}
		best_eval.listLivraison.pop_front();
		cit = best_eval.listLivraison.begin();
		date = cit->dateDepart;
		tempEssai.push_back(tempCamion);
	}
	best_eval.listLivraison.assign(tempEssai.begin(), tempEssai.end());
}

/**
 *  calcule le nouveau cout de stockage et de transport pour chacun des camions
 */
void calculeTotal(double* tabDist, solution* soluce){
	list<Camion>::iterator cit = soluce->listLivraison.begin();
	double calCoutTransport = 0;
	soluce->coutTotal = 0;
	
	for (; cit != soluce->listLivraison.end(); cit++){
		soluce->coutTotal += coutStockage( &cit.operator*() );
		calCoutTransport += (tabDist[cit->numClient]*2);
	}
	calCoutTransport = coutTransport * calCoutTransport;
	soluce->coutTotal += calCoutTransport;
	
	
	cout << "resultat calculer : " << endl;
	list<Camion>::const_iterator scit = soluce->listLivraison.begin() ;
	for (;scit != soluce->listLivraison.end(); scit++) {
		cout << "Camion du client : " << scit->numClient << " à livrer avant le : " << scit->dateDepart << "\td'un coup de : " << scit->coutCamion << endl;
		list<TEcheance>::const_iterator tit = scit->aLivrer.begin();
		for (; tit != scit->aLivrer.end(); tit++)
			cout << "\tDate livraison : " << tit->di << endl ;
	}
	cout << endl << "--- COUT TOTAL ---" << endl << setprecision(10) << soluce->coutTotal << endl;
	cout << endl << endl ;
	
	if (soluce->coutTotal <= best_eval.coutTotal) {
		best_eval.listLivraison = soluce->listLivraison ;
		coutTransport = coutTransport;
		best_eval.coutTotal = soluce->coutTotal;
	}
}

/**
 *  calcule la premiere heuristique de base
 */
void calculeTotal(double* tabDist){
	list<Camion>::iterator cit = best_eval.listLivraison.begin();
	double calCoutTransport = 0;
	
	for (; cit != best_eval.listLivraison.end(); cit++){
		best_eval.coutTotal += coutStockage( &cit.operator*() );
		calCoutTransport += (tabDist[cit->numClient]*2);
	}
	calCoutTransport = coutTransport * calCoutTransport;
	best_eval.coutTotal += calCoutTransport;
}

void testListCamion(double* tabDist, solution soluce){
	
	list<Camion>::iterator cit = soluce.listLivraison.begin();
	int dateFirst = cit->dateDepart;
	int numClientFirst = cit->numClient;
	int i = 0, j = 0;
	
	for (; cit != soluce.listLivraison.end(); cit++) {
		list<TEcheance>::const_iterator lit = cit->aLivrer.begin();
		j = 0;
		for (; lit != cit->aLivrer.end(); lit++) {
			if (lit->di > ( dateFirst + tabDist[numClientFirst] + tabDist[lit->cli] ) ){
				solution copySoluce = soluce;
				copySoluce.listLivraison.assign(soluce.listLivraison.begin(), soluce.listLivraison.end());

				list<Camion>::iterator ccit = copySoluce.listLivraison.begin();
				for (int x=0; x<i; x++) {
					ccit++;
				}
				list<TEcheance>::const_iterator clit = ccit->aLivrer.begin();
				for	(int y=0; y<j; y++) {
					clit++;
				}

				copySoluce.listLivraison.push_front( ajoutCamion(clit.operator*() ));
				ccit->aLivrer.erase( clit );
				
				/**
				 *  Calcule le cout total de notre solution
				 */
				calculeTotal(tabDist, &copySoluce);

				testListCamion(tabDist, copySoluce);
			}
			j++;
		}
		i++;
	}
}

int main(int argc, const char * argv[]) {
	solution soluce;
	/**
	 *  on passe en argument de ligne de commande le chemin vers le fichier décrivant l'instance
	 */
	if(argc != 2) {
		cout << "Usage : " << argv[0] << " path_to_instance " << endl;
		return EXIT_FAILURE;
	}
	
	if(lectureInstance(argv)==false)
		EXIT_FAILURE;
	
	/**
	 *  Mise en place d'un compteur de temps
	 */
    struct timeval tbegin,tend;
	
    // Lancement du chronométre
    gettimeofday(&tbegin,NULL);
	
	/**
	 *  ordonne la liste par date d'echeance croissante
	 */
	triList();
	
	/**
	 *  rassemble les clients dans un camion, jusqu'a ce que celui ci soit plein
	 */
	defBase();
		
	/**
	 *  Recupere la distance de chaque client et la stock dans un tableau
	 */
	list<Client>::iterator tempClient = listClient.begin();
	int i=1;
	double tabDist[nbrClient];
	
	for (; tempClient != listClient.end(); tempClient++){
		tabDist[i] = tempClient->distance;
		i++;
	}
	
	/**
	 *  définie un premier ordre de passage en fonction du groupement des camions
	 */
	ordrePassage(tabDist);
	
	
	/**
	 *  calcul le total de notre heuristique de base
	 */
	calculeTotal(tabDist);
	soluce = best_eval;
	
	/**
	 *  si nous avons une livraison à faire en dehors des camions déjà existant alors créer un camion pour cette livraison
	 */
	testListCamion(tabDist, soluce);
	
	
	// Arret du chronométre
    gettimeofday(&tend,NULL);
    texec=((double)(1000*(tend.tv_sec-tbegin.tv_sec)+((tend.tv_usec-tbegin.tv_usec)/1000)))/1000.;

	
	/**
	 *  affiche le resutlat
	 */
	affiche();
	
    return 0;
}