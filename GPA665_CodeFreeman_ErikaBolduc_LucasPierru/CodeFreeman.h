#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <conio.h>
using namespace std;

class codeFreeman
{
private:
	//nombre de forme dans fichier texte, taille de limage en x et en y, 
	//le nombre de mot dans fichier entree
	int nbForme{}, sizeX{}, sizeY{}, sizeFile{}, mArea{};
	typedef struct {
		int CoordX; /* Coordonnée en X du premier pixel de la forme */
		int CoordY; /* Coordonnée en Y du premier pixel de la forme */
		int NbrFreeman; /* Nombre d’éléments dans le code de Freeman */
		unsigned char* CodeFreeman; /* Pointeur qui sert de référence
										vers le tableau de données qui contient les éléments du code de
										Freeman */
	} SShape;
	SShape* Shapes = nullptr;				/* Pointeur qui sert de référence vers le tableau de description des formes */
	fstream file;				//fichier texte dentree
	string word, filename;		//mot lu et le nom du fichier dentree
	unsigned char** Image;		//tableau dynamique 2D contenant l'image de sortie
	int* Area;					//taleau dynamique contenant les aires
	enum AsciiCode {
		star = 42				//codeAscii pour *
	};
	enum direction {			// pour simplifier les directions possible dans code freeman
		west = 0,
		southWest = 1,
		south = 2,
		southEast = 3,
		east = 4,
		northEast = 5,
		north = 6,
		northWest = 7
	};

public:
	//menu d'accueil
	void startSimulation();
	//lire le fichier text dentree et remplir coordX, coordY et nombre freeman des Shapes
	void readFile();
	//remplir code freeman des shapes
	void getData(int i, string line);
	//initialiser les tableaux dynamiques
	void init();
	//compte le nombre de deplacement diagonal et droit pour calculer le perimetre
	void perimeter(int shape, double* perimeterVal);
	// fonction recursive qui rempli la forme de * et calcul laire
	void fillShape(int x, int y);
	//cree le fichier de sorti et le rempli
	void createFile();
	//cree le tableau Image et met des * pour le contour des formes
	void createImage();
	//Libere lespace de memoire des differents tableaux et met a nul plusieurs variables
	void deleteAll();
	//verifie si le nom du fichier text dentree est bon (si le fichier existe)
	void verifFilename();
};
