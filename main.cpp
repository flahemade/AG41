#include <iostream>
#include <list>		//list
#include <boost/tokenizer.hpp>	// parser
#include <iomanip>      // std::setprecision
#include <fstream>

using namespace std;

/**
 *  Declaration des variables global
 */

int nbrClient;
int nbrTrajet;
float coutTransport;
int capacite;
float coutTotal;

typedef struct client{
	int numClient;
	float distance;
	float cout;
}Client;
list<Client> cl;

typedef struct echeance{
	int i;			//numéro de livraison
	float cli;		//numéro de client
	float di;		//echeance
}TEcheance;
list<TEcheance> echeance;

typedef struct camion{
	list<TEcheance> aLivrer;		//numéro de livraison a livrer
	int numClient;					//numéro de client
	float coutCamion = 0;				//coutCamion
	float dateCourte = NULL;				//cout de livraison minimal
}Camion;
list<Camion> listCamion;

/**
 *  fonction permettant d'ajouter des valeurs à notre liste d'échancier
 */
TEcheance ajoutEcheance(int i, int cli, int di){
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
	cout << "i - cli - di" << endl;
	
	list<TEcheance>::const_iterator lit=echeance.begin() ;
	for(;lit!=echeance.end();++lit)
		cout << lit->i << " - " << lit->cli << "\t- " << lit->di << endl;
}

Camion ajoutCamion(TEcheance echeance){
	Camion camion;
	
	camion.aLivrer.push_back(echeance);
	camion.numClient = echeance.cli;
	camion.dateCourte = echeance.di;

	return camion;
}

void afficheListCamion(){
	if(!listCamion.empty()){
		list<Camion>::const_iterator cit = listCamion.begin() ;
		for (;cit != listCamion.end(); cit++) {
			cout << "Camion du client : " << cit->numClient << " à livrer avant le : " << cit->dateCourte << endl;
			list<TEcheance>::const_iterator tit = cit->aLivrer.begin();
			for (; tit != cit->aLivrer.end(); tit++)
				cout << "\tdate livraison : " << tit->di << endl ;
		}
	}
	else
		cout << "Acune livraison a effectuer"<<endl;
}

Client ajoutClient(int numClient, float cout, float distance){
	Client client;
	
	client.numClient = numClient;
	client.distance = distance;
	client.cout = cout;
	
	return client;
}

void afficheClient(){
	if(!cl.empty()){
		list<Client>::const_iterator cit=cl.begin() ;
		for(;cit != cl.end();cit++)
			cout << "Client numero : " << cit->numClient << " cout : " << cit->cout << " distance : " << cit->distance << endl;
	}
	else
		cout << "aucun client"<<endl;
}

void affiche(){
	
	cout << "nbrTrajet : " << nbrTrajet <<endl;
	cout << "nbrClient : " << nbrClient <<endl;
	cout << "transporteur capaciter : " << capacite <<endl;
	cout << "cout transport : " << coutTransport <<endl;
	
	afficheClient();
	afficheEcheance();
	afficheListCamion();
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
		if(getline(is, str)){
			coutTrans = atof(split(str, ":", 1, -1).c_str() );
		}
		if(getline(is, str))
			distance = atof(split(str, ":", 1, -1).c_str() );
		cl.push_back(ajoutClient(numCl, coutTrans, distance));
	}
	
	string chaine1, chaine2;
	if(getline(is, str))
		chaine1 = split(str, ":", 1, -1);
	if(getline(is, str))
		chaine2 = split(str, ":", 1, -1);
	
	//cout << "test " << split(chaine2, ";",3, -1 ) << " --> " << setprecision(17) << atof(split(chaine2, ";",3, -1 ).c_str()) << endl << endl;
	
	for(int i=0; i<nbrTrajet;i++)
		echeance.push_back(ajoutEcheance(i, atof(split(chaine1, ";",i, -1 ).c_str() ), atof(split(chaine2, ";",i, -1 ).c_str()) ) );
	
	return true;
}

/**
 *  Fonction tri par date
 *  On recupere en argument une liste d'une structure d'echeance que l'on tri en fonction de la date d'echeance
 *  Puis nous retournons la nouvelle liste
 */
void triList(list<TEcheance>* echeance){
	list<TEcheance> temp;								//créer liste temporaire

	
	temp.push_front(ajoutEcheance(echeance->begin()->i,echeance->begin()->cli,echeance->begin()->di)) ;	//ajoute le premier element de echeance a la nouvelle liste temp
	list<TEcheance>::const_iterator lit=echeance->begin();										//initialise le curseur sur le premier element de echeance
	lit++;
	while(lit!=echeance->end()){
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

	echeance->assign(temp.begin(), temp.end());				//remplace tous les elements de la liste echeance par la liste temporaire
}

/**
 *	Groupe client par capacite camion
 */
void defBase(){
	list<TEcheance>::const_iterator lit = echeance.begin();
	for (;lit != echeance.end();lit++){
		if (listCamion.empty())
			listCamion.push_back( ajoutCamion( lit.operator*() ));
		else {
			list<Camion>::iterator cit = listCamion.begin();
			bool test = false;
			while(cit != listCamion.end() && test == false) {
				if (cit->aLivrer.size() < capacite && cit->numClient == lit->cli) {
					cit->aLivrer.push_back(lit.operator*());
					if (cit->dateCourte > lit->di)
						cit->dateCourte = lit->di;
					test = true;
				}
				cit++;
			}
			if (!test) {
				listCamion.push_back( ajoutCamion( lit.operator*() ));
			}
		}
	}
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

//Pour la structure de données, vaut-il mieux partir sur 3 listes donnant chacune une ligne du tableau ?

int main(int argc, const char * argv[]) {
	
	/**
	 *  on passe en argument de ligne de commande le chemin vers le fichier décrivant l'instance
	 */
	if(argc != 2) {
		cout << "Usage : " << argv[0] << " path_to_instance " << endl;
		return EXIT_FAILURE;
	}
	
	if(lectureInstance(argv)==false)
		EXIT_FAILURE;
	
	cout << endl << "avant tri : " << endl;
	afficheEcheance();
	
	triList(&echeance);
	
	cout << endl << "apres tri : " << endl;
	afficheEcheance();
	
	defBase();
	
	affiche();
		
    return 0;
}

