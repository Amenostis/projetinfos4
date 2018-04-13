#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Vertex et Edge se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours à itérateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <stack>
#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box;

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value;

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value;

        // Une image de "remplissage"
        grman::WidgetImage m_img;

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx;

        // Une boite pour le label précédent
        grman::WidgetText m_box_label_idx;

        int m_idx;
        std::string m_Thing;

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp

        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
        int getidx();
    std::string getThing()
    {
        return m_Thing;
    }
};


class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :
        /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
        std::vector<int> m_in;

        /// liste des indices des arcs partant du sommet : accès aux successeurs
        std::vector<int> m_out;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_value;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr;

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne précédente est en gros équivalent à la ligne suivante :
        // VertexInterface * m_interface = nullptr;
        /// ICI les variables permettant le fonctionnement de l'algo de simulation
        int m_N_dep; /// Nombre d'individus à t de départ
        int m_N_ar; /// Nombre d'individus à t+1, à l'arrivée
        int m_K; /// Capacité de portage de la population (Coef)
        float m_ryth; /// Rythme de croissance (coef)


    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Vertex (double value=0, VertexInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }

        /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update(); /// a remplir dans le scope (int * x, int *y, bool *z)
                            /// double getvalue()
                            ///    {
                            ///        return m_value

                            ///    }

};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge;
    friend class Graph;
    //friend class Vertex;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de réglage associés
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;

    private :
        /// indice du sommet de départ de l'arc
        int m_from;

        /// indice du sommet d'arrivée de l'arc
        int m_to;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_weight;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr;

        /// Coeff pour simulation evol.pop
        int m_coeff;

    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Edge (double weight=0, EdgeInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }

        /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        /// La boite qui contient toute l'interface d'un graphe
        grman::WidgetBox m_top_box;

        /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box;

        /// Dans cette boite seront ajoutés des boutons de contrôle etc...
        grman::WidgetBox m_tool_box;

        grman::WidgetButton m_boutonEsc;
        grman::WidgetImage m_Esc;
        grman::WidgetImage texte1;

        grman::WidgetButton m_boutonRemove;
        grman::WidgetImage m_Remove;
        grman::WidgetImage texte3;

        grman::WidgetButton m_boutonAdd;
        grman::WidgetImage m_Add;
        grman::WidgetImage texte2;

        grman::WidgetButton m_boutonSave;
        grman::WidgetImage m_Save;
        grman::WidgetImage texte4;

        grman::WidgetButton m_boutonSimul;
        grman::WidgetImage m_Simul;
        grman::WidgetImage texte5;

        grman::WidgetButton m_boutonConex;
        grman::WidgetImage m_Conex;
        grman::WidgetImage texte6;

        grman::WidgetButton m_boutonRemoveSom;
        grman::WidgetImage m_RemoveSom;
        grman::WidgetImage texte7;

        grman::WidgetButton m_boutonAddSom;
        grman::WidgetImage m_AddSom;
        grman::WidgetImage texte8;




        // A compléter éventuellement par des widgets de décoration ou
        // d'édition (boutons ajouter/enlever ...)

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        GraphInterface(int x, int y, int w, int h);
};


class Graph
{
    friend class Thing;
    private :

        /// La "liste" des arêtes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;
        //C:\Users\manol\Desktop\PISCINE\graph.h
        /// Liste des sommets a ne pas sauvegarder
        std::map<int, std::string> m_so_asupp;
        ///Liste des sommets avec nom
        std::map<int, std::string> m_nom_so;


        // int id_graph : pour sauvegarde et chargement
    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0 );
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        std::map<int, Edge> Getm_edges();


        double Getm_weight();


        /// Méthode spéciale qui construit un graphe arbitraire (démo)
        /// Voir implémentation dans le .cpp
        /// Cette méthode est à enlever et remplacer par un système
        /// de chargement de fichiers par exemple.
        void make_example();
        void vider_tout_le_graph();



        void evolution_pop();
        void facteurRepro(int num_vertex_donne); /// Nom à changer + Nom indices
        int calcul_K(int);
        int predation(int);
        void MAJarete();
        void menu();

        ///Fonctions de jeu
        void Charger_toutes_les_sommets_et_aretes(std::string nom_systeme);
        void Sauvegarde_sommets_aretes(std::string nom_systeme);
        void Sauvegarde_totale(std::string nom_systeme);
        void test_remove_edge(int eidx);
        void test_remove_sommet(std::string nom_systeme, Graph g);
        void ajout_arete(std::string nom_systeme, Graph g);
        void ajout_sommet(std::string nom_systeme,Graph g);



        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        int update(std::string nom_fichier, Graph g);
};


class Thing {

    friend class Graph;
    private :
        /// Utiliser une top_box de type WidgetBox pour encapsuler
        /// tous les éléments de l'interface associés à votre objet

        grman::WidgetBox m_menu;
        grman::WidgetImage m_fond;
        grman::WidgetBox m_top_box;
        grman::WidgetBox m_boite_boutons;

        grman::WidgetButton m_bouton1;
        grman::WidgetText m_bouton1_label;

        grman::WidgetButton m_bouton2;
        grman::WidgetText m_bouton2_label;

        grman::WidgetButton m_bouton3;
        grman::WidgetText m_bouton3_label;

        grman::WidgetButton m_boutonRegles;
        grman::WidgetText m_boutonRegles_label;

        grman::WidgetButton m_boutonQuitter;
        grman::WidgetImage m_Quitter;



        /// Les Widgets qui constitueront l'interface de l'objet
        grman::WidgetImage m_img;            // Sera l'arbre à droite
        grman::WidgetImage m_img_anime;      // Sera le clown marcheur au milieu
        grman::WidgetCheckBox m_marche;      // Sera la boite à cocher en bas à gauche (faire marcher le clown)
        grman::WidgetText m_legende;         // Sera le texte "CLICK ME" en bas

        grman::WidgetBox m_boite_regles;     // Sera la boite à sliders en haut à gauche
        grman::WidgetVSlider m_regle_reel;   // Sera le slider de gauche (ordonnée du clow)
        grman::WidgetVSlider m_regle_entier; // Sera le slider de droite ( vitesse marche clown)

        /*grman::WidgetBox m_boite_boutons;    // Sera la boite à boutons en haut à droite
        grman::WidgetButton m_bouton1;       // Sera le bouton avec le texte NEW
        grman::WidgetText m_bouton1_label;   // Le texte NEW
        grman::WidgetButton m_bouton2;       // Sera le bouton avec l'image de clown méchant
        grman::WidgetImage m_bouton2_image;  // L'image de clown méchant
        grman::WidgetText m_dragme; */         // Le message "DRAG ME" à droite des 2 boutons

        grman::WidgetEdge m_lien1;           // Sera un lien non orienté entre l'arbre et le clown
        grman::WidgetEdge m_lien2;           // Sera un lien orienté entre la boite à boutons et le clown
        grman::WidgetText m_lien2_label;     // Sera un label sur le lien précédent
        grman::WidgetEdge m_lien3;           // Sera un lien orienté entre la check box et le clown

        std::stack<grman::WidgetImage *> m_dynaclowns;    // Expérimental : les clowns ajoutés/enlevés avec les boutons

    public :

        /// Le constructeur de la classe (pas forcément par défaut !)
        /// initialise les données des widgets, place la hiérarchie des sous-cadres etc...
        Thing();
        void Charger_toutes_les_sommets_et_aretes(std::string nom_systeme);
        void Savegarde_sommets_aretes(std::string nom_systeme);
        void test_remove_edge(int eidx);
        //void evolution_pop();
        int update(Graph& graph,std::string& nom_fichier);
        //void menu();

        /// On a des allocations dynamiques dans m_dynaclowns => à nettoyer dans le destructeur
        ~Thing();

};




#endif // GRAPH_H_INCLUDED
