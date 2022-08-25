#include "codeFreeman.h"

void codeFreeman::startSimulation()
{
    string mChoice{};
    system("CLS");
    cout << "Voulez-vous analyser une image avec le code de freeman? Y/N" << endl;
    cin >> mChoice;
    //Boucle tant que yes est choisi
    while (mChoice == "Y" || mChoice == "y") {
        cout << "Entrez le nom du fichier texte a analyser" << endl;
        cin >> filename;
        //verifier si fichier existe
        verifFilename();
        //lire fichier et remplir Shapes
        readFile();
        //Creer les images et les remplir
        createImage();
        //Creer le fichier de sortie et le remplir
        createFile();
        //Supprimer tous les pointeurs et tableaux
        deleteAll();
        system("CLS");
        cout << "Voulez-vous recommencer? Y/N" << endl;
        cin >> mChoice;
    }
}

void codeFreeman::readFile()
{
    // ouvrir le fichier 
    file.open(filename.c_str());

    //Compter nombre de chiffre dans fichier
    while (file >> word)
    {
        switch (sizeFile)
        {
        case 0:
            //premier chiffre correspond a la taille en x
            sizeX = stoi(word);
            break;
        case 1:
            //2e chiffre correspond a la taille en y
            sizeY = stoi(word);
            break;
        case 2:
            //3e chiffre correspond au nombre de forme
            nbForme = stoi(word);
            break;
        }
        sizeFile++;
    }
    file.close();
    file.open(filename.c_str());
    string line;
    int a = 0;
    //initialiser les pointeurs et tableaux
    init();
    //allocation dynamique
    Shapes = (SShape*)malloc(sizeof(SShape) * (nbForme));
    for (int i = 0; i < nbForme + 2; i++)
    {
        //prendre la ligne suivante
        getline(file, line);
        if (i >= 2) {
            for (int j = 0; j < 3; j++) {
                size_t space_pos = line.find(" ");
                if (space_pos != std::string::npos) {
                    switch (j)
                    {
                    case 0:
                        Shapes[i - 2].CoordX = stoi(line.substr(0));
                        break;
                    case 1:
                        Shapes[i - 2].CoordY = stoi(line.substr(space_pos));
                        line = line.substr(space_pos + 1);
                        break;
                    case 2:
                        Shapes[i - 2].NbrFreeman = stoi(line.substr(space_pos));
                        line = line.substr(space_pos + 1);
                        break;
                    }
                }
            }
            //met le code freeman dans Shapes
            getData(i - 2, line);
        }
    }
    file.close();
}

void codeFreeman::getData(int i, string line)
{
    //Allocation dynamique pour le code freeman de chaque shapes
    Shapes[i].CodeFreeman = (unsigned char*)malloc(sizeof(unsigned char) * (Shapes[i].NbrFreeman));

    //Remplir le code de freeman pour la shape i jusquau nombre de freeman de la shape i
    for (int j = 0; j < Shapes[i].NbrFreeman; j++) {
        size_t space_pos = line.find(" ");
        if (space_pos != std::string::npos) {
            Shapes[i].CodeFreeman[j] = stoi(line.substr(space_pos));
            line = line.substr(space_pos + 1);
        }
    }
}

void codeFreeman::init()
{
    //allocation dynamique de shape pour le nombre de forme voulu du fichier text
    struct SShape* Shapes = (SShape*)malloc(sizeof(SShape*) * (nbForme));
    //allocation dynamique de area en fonction du nombre de forme voulu
    Area = (int*)malloc(sizeof(int) * (nbForme));
    //allocation dynamique de Image en fonction de la taille X voulu
    Image = (unsigned char**)malloc(sizeof(unsigned char*) * (sizeX));
    //Allocation dynamique de Image pour chaque valeur X en fonction de la taille Y voulu
    for (int i = 0; i < sizeX; i++)
    {
        Image[i] = (unsigned char*)malloc(sizeof(unsigned char) * (sizeY));
    }
}

void codeFreeman::perimeter(int shape, double* perimeterVal)
{
    for (int j = 0; j < Shapes[shape].NbrFreeman; j++) {
        if (Shapes[shape].CodeFreeman[j] % 2 == 0) {
            //+1 si ligne droite
            (*perimeterVal)++;
        }
        else
        {
            //+ racine(2) si en diagonal
            *perimeterVal += sqrt(2);
        }
    }
}

void codeFreeman::fillShape(int x, int y)
{
    //sil ny a pas deja un * alors en mettre un dans Image et +1 aire
    if (Image[x][y] != star) {
        Image[x][y] = star;
        mArea++;
    }
    if (Image[x][y] == star) {
        //si deplacement a droite nest pas une *
        if (Image[x + 1][y] != star) {
            //appelle fonction recusrive et mettre une *
            fillShape(x + 1, y);
        }
        //si deplacement a gauche pas deja une *
        if (Image[x - 1][y] != star) {
            //appelle fonction recusrive et mettre une *
            fillShape(x - 1, y);
        }
        //si deplacement en bas pas deja une *
        if (Image[x][y + 1] != star) {
            //appelle fonction recusrive et mettre une *
            fillShape(x, y + 1);
        }
    }
}

void codeFreeman::createFile()
{
    double perimeterVal{};
    ofstream outfile("test.txt");

    outfile << filename << "\n" << endl;

    //ajout des shapes dans fichier sortie
    for (int i = -1; i < sizeX + 1; i++) {
        for (int j = -1; j < sizeY + 1; j++) {
            if (i < sizeX && i >= 0 && j < sizeY && j >= 0) {
                if (Image[j][i] == star)
                {
                    outfile << "*";
                }
                else {
                    outfile << " ";
                }
            }
            else {
                if (j == -1 || j == sizeY || i == -1 || i == sizeX) {
                    outfile << "%";
                }
            }
        }
        outfile << endl;
    }
    //ajout des perimetres et aires dans fichier sortie
    outfile << "\nIl y a " << nbForme << " sur l'image\n";
    for (int i = 0; i < nbForme; i++) {
        mArea = 0;
        perimeterVal = 0;
        perimeter(i, &perimeterVal);
        outfile << "Objet no. " << i + 1 << " : Perimetre = " << setprecision(4) << perimeterVal << "   Aire = " << Area[i] << endl;
    }

    outfile.close();
}

void codeFreeman::createImage()
{
    int x = 0, y = 0;
    for (int i = 0; i < nbForme; i++) {
        mArea = 0;
        if (Shapes != nullptr) {
            //mettre * a premiere coordonnee
            Image[Shapes[i].CoordX][Shapes[i].CoordY] = star;
            x = Shapes[i].CoordX;
            y = Shapes[i].CoordY;
            //deplacement en fonction du code de freeman
            for (int j = 0; j < Shapes[i].NbrFreeman; j++) {
                switch (Shapes[i].CodeFreeman[j])
                {
                case west:
                    x -= 1;
                    break;
                case southWest:
                    x -= 1;
                    y += 1;
                    break;
                case south:
                    y += 1;
                    break;
                case southEast:
                    x += 1;
                    y += 1;
                    break;
                case east:
                    x += 1;
                    break;
                case northEast:
                    x += 1;
                    y -= 1;
                    break;
                case north:
                    y -= 1;
                    break;
                case northWest:
                    x -= 1;
                    y -= 1;
                    break;
                }
                Image[x][y] = star;
            }
        }
        //remplir les shapes
        fillShape(Shapes[i].CoordX, Shapes[i].CoordY + 1);
        Area[i] = mArea + Shapes[i].NbrFreeman;
    }
}

void codeFreeman::deleteAll()
{
    for (int i = 0; i < sizeX; i++)
    {
        free(Image[i]);
    }
    free(Image);

    free(Area);
    free(Shapes);
    nbForme = 0;
    sizeX = 0;
    sizeY = 0;
    sizeFile = 0;
}

void codeFreeman::verifFilename()
{
    bool verif;
    ifstream file(filename);
    verif = bool(file);
    while (!verif) {
        cout << "Le fichier n'est pas trouve. Entrez le nom du fichier texte" << endl;
        cin >> filename;
        ifstream file(filename);
        verif = bool(file);
    }
}