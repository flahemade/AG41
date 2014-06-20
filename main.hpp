#ifndef AG41_main_hpp
#define AG41_main_hpp

#include <iostream>
#include <list>					//list
#include <boost/tokenizer.hpp>	// parser
#include <iomanip>				// std::setprecision
#include <fstream>				//file buffer
#include <sys/time.h>


using namespace std;

/**
 *  Declaration des variables global
 */
double texec = 0;

int		nbrClient;
int		nbrTrajet;
int		capacite;
double	coutTransport = 0;

typedef struct client{
	int numClient;
	double distance;
	double coutClient;
}Client;
list<Client> listClient;

typedef struct produit{
	int i;			//numéro de livraison
	int cli;		//numéro de client
	double di;		//echeance
}Produit;
list<Produit> produit;

typedef struct livraison{
	list<Produit> aLivrer;		//numéro de livraison à livrer
	int numClient;					//numéro de client
	double	dateDepart = NULL;		//date départ initier à null
	double	coutLivraison = 0;			//cout de stockage
	
}Livraison;

typedef struct solution{
	list<Livraison> listLivraison;
	double	coutTotal = 0;			//inclu le cout de stockage et le cout de transport
}bestSolution;
solution best_eval;

double* tabDist;

/**
 *  fonction permettant d'ajouter des valeurs à notre liste d'échancier
 */
Produit ajoutProduit(int i, double cli, double di);

/**
 *  afficher la liste "produit"
 */
void afficheProduit();

Livraison ajoutLivraison(Produit produit);

void afficheListLivraison();

Client ajoutClient(int numClient, double coutClient, double distance);

void afficheClient();

void affiche();

string split( const string & Msg, const string & Separators, int position, int ligne);

bool lectureInstance(const char * argv[]);

void distanceClient();

/**
 *  Fonction tri par date
 *  On recupere en argument une liste d'une structure de produit que l'on tri en fonction de la date de chaque produit
 *  Puis nous retournons la nouvelle liste
 */
void triList();

/**
 *	Groupe client par capacite de livraison
 */
void defBase();

void triLivraisonCoutStockage(solution* soluce);

double coutStockage(Livraison* structLivraison);

double coutStockage(Livraison* structLivraison, double date);

/**
 *  calcule l'heuristique de base
 */
void calculeTotal();

/**
 *  calcule le cout de stockage et de transport de cette heuristique, si meilleur que notre précédente heuristique alors on l'enregistre
 */
void calculeTotal(solution* soluce);

/**
 *  calcul l'horaire de depart de toutes les livraisons
 */
bool calculHoraire(solution* soluce);

/**
 *  Définie un heuristique de base et ordonne les livraisons par ordre décroissant de cout de stockage
 */
void ordrePassage();

void calculeSolution(solution soluce, int position);

/**
 *  switch toutes les livraisons sauf la premiere
 */
void switchLivraison(solution soluce, int position);

int groupement(solution soluce);

/**
 *  effectuer une livraison apres la derniere livraison si possible
 */
void avanceDepartLivraison(solution soluce, int position);

/**
 *  effectuer une livraison apres la derniere livraison
 */
void departLivraison(solution soluce, int position);

#endif
