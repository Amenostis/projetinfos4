#include "grman/grman.h"
#include <iostream>
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    Thing menu;

    int choix=0;
    std::string nom_fichier;


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
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
             /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        choix = g.update(nom_fichier,g);
        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        break;

        }

    }

    grman::fermer_allegro();


    return 0;
}
END_OF_MAIN();


