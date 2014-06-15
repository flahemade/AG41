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

typedef struct echeance{
	int i;			//numéro de livraison
	int cli;		//numéro de client
	double di;		//echeance
}TEcheance;
list<TEcheance> echeance;

typedef struct camion{
	list<TEcheance> aLivrer;		//numéro de livraison à livrer
	int numClient;					//numéro de client
	double	dateDepart = NULL;		//date départ initier à null
	double	coutCamion = 0;			//cout de stockage
	
}Camion;

typedef struct solution{
	list<Camion> listLivraison;
	double	coutTotal = 0;			//inclu le cout de stockage et le cout de transport
}bestSolution;
solution best_eval;

double* tabDist;

/**
 *  fonction permettant d'ajouter des valeurs à notre liste d'échancier
 */
TEcheance ajoutEcheance(int i, double cli, double di);

/**
 *  afficher la liste "echeance"
 */
void afficheEcheance();

Camion ajoutCamion(TEcheance echeance);

void afficheListCamion();

Client ajoutClient(int numClient, double coutClient, double distance);

void afficheClient();

void affiche();

string split( const string & Msg, const string & Separators, int position, int ligne);

bool lectureInstance(const char * argv[]);

void distanceClient();

/**
 *  Fonction tri par date
 *  On recupere en argument une liste d'une structure d'echeance que l'on tri en fonction de la date d'echeance
 *  Puis nous retournons la nouvelle liste
 */
void triList();

/**
 *	Groupe client par capacite camion
 */
void defBase();

void triCamionCoutStockage(solution* soluce);

double coutStockage(Camion* structCamion);

double coutStockage(Camion* structCamion, double date);

/**
 *  calcule l'heuristique de base
 */
void calculeTotal();

/**
 *  calcule le cout de stockage et de transport de cette heuristique, si meilleur que notre précédente heuristique alors on l'enregistre
 */
void calculeTotal(solution* soluce);

/**
 *  calcul l'horaire de depart de tous les camions
 */
bool calculHoraire(solution* soluce);

/**
 *  Définie un heuristique de base et ordonne les camions par ordre décroissant de cout de stockage
 */
void ordrePassage();

void calculeSolution(solution soluce, int position);

/**
 *  switch toutes les livraisons sauf la premiere
 */
void switchCamion(solution soluce, int position);

void grouperPlusTard(solution soluce, int position);

/**
 *  effectuer une livraison apres la derniere livraison si possible
 */
void avanceDepartCamion(solution soluce, int position);

/**
 *  effectuer une livraison apres la derniere livraison
 */
void departCamion(solution soluce, int position);

#endif
