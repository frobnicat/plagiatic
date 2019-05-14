/**

Copier la liste des résultats depuis la page d'accueil du résultat de moss
( exemple http://moss.stanford.edu/results/808322131/ )

Prendre uniquement la liste :
<<
    Collec_extra/Naoki_ARNAUD/ (80%) 	Collec_extra/Yann_MESSALATI/ (80%) 	987
    Collec_extra/Alan_TERBECHE/ (99%) 	Collec_extra/Matis_LERET/ (99%) 	754
    Collec_extra/Romain_BELLIOT/ (76%) 	Collec_extra/Salome_STANGHELLINI/ (78%) 	635
...
>>

Coller dans notepad++

Filtrer (enlever) le préfixe en commun pour alléger (sur l'exemple précédent on enlève "Collec_extra"

Sauver sous moss.txt

Run ce programme => génère dot.txt (code dot/graphviz)
=> ouvrir dans notepad++ et Copier
( noter le nombre de cas dans la console )

La suite se passe sur https://dreampuf.github.io/GraphvizOnline/

    Coller le code dot/graphviz dans le panneau à gauche

    Sélectionner Engine: neato ou fdp (fdp est moins confus)

    Ajuster (tester les réglages...)

    Show raw output => copier => coller dans un nouveau document notepad++
    nettoyer dernière ligne et en-tête => puis sauver sous plagigraphe.svg

**/

/// Nombre de lignes en commun (seuil de détection)
#define MINLINES 50

/// Echelle de distance (plus grand -> plus grande séparation)
#define SCALELINES 150

/// Fichier source correspondant à la liste des plagiats
#define SOURCE "../moss.txt"

/// Fichier destination correspondant à un graphe en format dot/graphviz
#define DEST "../dot.txt"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>


int main()
{
    std::ifstream ifs{SOURCE};
    if (!ifs)
        exit(1);

    std::ofstream ofs{DEST};
    if (!ofs)
        exit(2);

    ofs << "graph g{\n";

    std::set<std::string> liste;
    for (std::string line; std::getline(ifs, line); )
    {
        std::istringstream iss{line};
        std::string vert[2];
        std::string dummy[2];
        int nblines;
        iss >> vert[0] >> dummy[0] >> vert[1] >> dummy[1] >> nblines;

        if (nblines>=MINLINES)
        {
            std::cout << vert[0] << " " << vert[1] << " " << nblines << "\n";
            liste.insert(vert[0]);
            liste.insert(vert[1]);
            std::string lab[2];
            for (size_t i=0; i<2; ++i)
            {
                bool last=true;
                for (auto c : vert[i])
                    if (c=='_' || c=='-')
                    {
                        if (!last)
                            lab[i]+="\\n";
                        last = true;
                    }
                    else
                    {
                        lab[i]+=c;
                        last = false;
                    }
            }

            ofs << "  \"" << lab[0] << "\"" << " -- " << "\"" << lab[1] << "\"";// << "\n";
            ofs << " [len=" << (float)SCALELINES/nblines << " label=\"" << nblines << "\"]" << "\n";
        }
    }

    std::cout << "\n\n\n" << liste.size() << "\n\n";
    for (auto s : liste)
        std::cout << s << "\n";

    std::cout << "\n\n" << liste.size() << "\n\n";

    ofs << "}\n";

    return 0;
}
