#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    ///Main pour algorithme de forte connexite (casi-complet)
   /* Graph g;//crere Graph et vect de vect
    std::vector<std::vector<int>>tab;
    g.Charger_toutes_les_sommets_et_aretes("plateforme_1");// charge le fichier
    tab=g.algo_de_toute_les_composantes_connexes();//  vecteur de vect= nom.fct
    for ( unsigned int i = 0; i < tab.size(); i++ )// puis i j : conex numero i, indice de
    {
        std::cout << "composante connexe numero" <<i<< ":" << std::endl;
        for ( unsigned int j = 0; j < tab[i].size(); j++ )
        {
            std::cout << "composante : "<<tab[i][j];
        }
        std::cout << std::endl;
    }*/
    Thing menu;

    int choix=0;
    std::string nom_fichier;


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
   while ( choix!=5 )
    {
        switch(choix)
        {
        case 0 :
            choix = menu.update(g,nom_fichier);
            grman::mettre_a_jour();
            break;

        case 1 :
            g.Sauvegarde_sommets_aretes(nom_fichier);
            g.vider_tout_le_graph();
            choix = 0;
            break;

        case 2 :
            g.Sauvegarde_sommets_aretes("");
            g.vider_tout_le_graph();
            break;

        case 3 :
            g.Sauvegarde_sommets_aretes("");
            g.vider_tout_le_graph();
            break;



        case 4 :
             /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        choix = g.update(nom_fichier,g);
        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        break;

        }

    }

    grman::fermer_allegro();


    return 0;
}
END_OF_MAIN();


