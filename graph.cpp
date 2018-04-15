#include "graph.h"
//#include "../graph.h"

#include <iostream>
#include <string>
#include <fstream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{

    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) ) ;
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );

}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

    /*double Edge::Getm_weight();
        {
            return m_weight;
        }*/
}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    ///Bouton Quitter

    m_tool_box.add_child(m_boutonEsc);
    m_boutonEsc.add_child(texte1);
    m_boutonEsc.set_frame(2,2,80,80);
    texte1.set_pic_name("Esc.bmp");
   // m_boutonEsc.pic_idx(999);
    m_boutonEsc.set_gravity_x(grman::GravityX::Right);

/*  if(m_interface->m_boutonEsc.clicked())
    {
        std::cout << "Vous quittez le domaine d'etude" << std::endl;
    }
*/
    ///Bouton Ajouter Arete
    m_tool_box.add_child(m_boutonAdd);
    m_boutonAdd.add_child(texte2);
    m_boutonAdd.set_frame(2,85,80,80);
    texte2.set_pic_name("Add.bmp");
    m_boutonAdd.set_gravity_x(grman::GravityX::Right);

/*
    if(m_interface->m_boutonAdd.clicked())
    {
        std::cout << "Vous venez de rajouter ... " << std::endl;
    }
*/
    /// Bouton Supprimer Arete
    m_tool_box.add_child(m_boutonRemove);
    m_boutonRemove.add_child(texte3);
    m_boutonRemove.set_frame(2,168,80,80);
    texte3.set_pic_name("Remove.bmp");
    m_boutonRemove.set_gravity_x(grman::GravityX::Right);

/*
    if(m_interface->m_boutonRemove.clicked())
    {
        std::cout << "Vous venez d'aérer le graphe" << std::endl;
    }
*/
    /// Bouton Ajout Sommet
    m_tool_box.add_child(m_boutonAddSom);
    m_boutonAddSom.add_child(texte8);
    m_boutonAddSom.set_frame(2,251,80,80);
    texte8.set_pic_name("AddSom.bmp");
    m_boutonAddSom.set_gravity_x(grman::GravityX::Right);


    /// Bouton Suppression Sommet
    m_tool_box.add_child(m_boutonRemoveSom);
    m_boutonRemoveSom.add_child(texte7);
    m_boutonRemoveSom.set_frame(2,334,80,80);
    texte7.set_pic_name("RemoveSom.bmp");
    m_boutonRemoveSom.set_gravity_x(grman::GravityX::Right);


    /// Bouton Sauvegarde
    m_tool_box.add_child(m_boutonSave);
    m_boutonSave.add_child(texte4);
    m_boutonSave.set_frame(2,417,80,80);
    texte4.set_pic_name("Save.bmp");
    m_boutonSave.set_gravity_x(grman::GravityX::Right);

/*
    if(m_interface->m_boutonSave.clicked())
    {
        std::cout << "Sauvegarde effectuée"
    }
*/
    /// Bouton Simulation
    m_tool_box.add_child(m_boutonSimul);
    m_boutonSimul.add_child(texte5);
    m_boutonSimul.set_frame(2,490,80,80);
    texte5.set_pic_name("Simul.bmp");
    m_boutonSimul.set_gravity_x(grman::GravityX::Right);

    /// Bouton Connexité
    m_tool_box.add_child(m_boutonConex);
    m_boutonConex.add_child(texte6);
    m_boutonConex.set_frame(2,573,80,80);
    texte6.set_pic_name("Conex.bmp");
    m_boutonConex.set_gravity_x(grman::GravityX::Right);


}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
int Graph::update(std::string nom_fichier, Graph g)
{
   // if (!m_interface)
   //     return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();
    for (auto &elt : m_edges)
        elt.second.pre_update();
    m_interface->m_top_box.update();
    for (auto &elt : m_vertices)
        elt.second.post_update();
    for (auto &elt : m_edges)
        elt.second.post_update();

    ///DEFINITION DES ACTIONS SI ON CLIQUE SUR UN BOUTTON

    if(m_interface->m_boutonAdd.clicked())
    {
        ///Add Arete
        ajout_arete(nom_fichier, g);
    }

    if(m_interface->m_boutonRemove.clicked())
    {
        /// Supprimer Arete
        int x;
        std::cout<<"Quelle arete voulez vous supprimer ? "<<std::endl;
        std::cin>>x;
        test_remove_edge(x);
    }

    if(m_interface->m_boutonAddSom.clicked())
    {
        /// Add Sommet
        ajout_sommet(nom_fichier, g);
    }

    if (m_interface->m_boutonRemoveSom.clicked())
    {
        /// Remove Sommet
        test_remove_sommet(nom_fichier, g);
    }

    if (m_interface->m_boutonSave.clicked())
    {
        /// Save Totale
        Sauvegarde_sommets_aretes(nom_fichier);
    }

    if(m_interface->m_boutonConex.clicked())
    {
        /// Connexité

    }

  /*  if(m_interface->m_boutonSimul.clicked())

    {
        evolution_pop();
        calcul_K();
        predation();
        facteurRepro();
        MAJarete();
    }
*/
   if(m_interface->m_boutonEsc.clicked())
    {
        if(nom_fichier == "systemep")
            return 1;
    }

    return 4;

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    /// OOOPS ! Prendre en compte l'arc ajouté dans la topologie du graphe !
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);

/*    std::map<int,Edge> Graph::Getm_edges()
        {
            return m_edges;
        }*/

}

void Graph::vider_tout_le_graph()
{
    m_interface.reset();
    m_edges.clear();
    m_vertices.clear();

}

/*
/// eidx index of edge to remove

void Graph::test_remove_edge(int eidx)

{

    /// référence vers le Edge à enlever

    Edge &remed=m_edges.at(eidx);



    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;



    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;



    /// test : on a bien des éléments interfacés

   if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :

         EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);

         m_interface->m_main_box.add_child(ei->m_top_edge);

         m_edges[idx] = Edge(weight, ei);

        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr

        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

    m_edges.erase( eidx );





    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;

}
*/
 std::map<int,Edge> Graph::Getm_edges()
        {
            return m_edges;
        }

        ///THING///
Thing::Thing()
{

    //void Thing::menu()

    m_menu.set_frame(0,0,800,600);
    m_menu.add_child(m_fond);
    m_fond.set_pic_name("menu.bmp");

    m_menu.add_child( m_bouton1 );
    m_bouton1.set_frame(100,300,150,50);
    m_bouton1.set_bg_color(BLANC);
    m_bouton1.add_child(m_bouton1_label);
    m_bouton1_label.set_message("Plateforme 1");

    m_menu.add_child( m_bouton2 );
    m_bouton2.set_frame(350,300,150,50);
    m_bouton2.set_bg_color(BLANC);
    m_bouton2.add_child(m_bouton2_label);
    m_bouton2_label.set_message("Plateforme 2");

    m_menu.add_child( m_bouton3 );
    m_bouton3.set_frame(600,300,150,50);
    m_bouton3.set_bg_color(BLANC);
    m_bouton3.add_child(m_bouton3_label);
    m_bouton3_label.set_message("Plateforme 3");

    m_menu.add_child( m_boutonRegles );
    m_boutonRegles.set_frame(300,450,250,50);
    m_boutonRegles.set_bg_color(BLANC);
    m_boutonRegles.add_child(m_boutonRegles_label);
    m_boutonRegles_label.set_message("Principes de la simulation");


    /// On ajoute en haut à droite la boite à boutons
    m_menu.add_child( m_boutonQuitter );
    m_boutonQuitter.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up );
    m_boutonQuitter.set_dim(80,80);
    m_boutonQuitter.set_bg_color(NOIR);
    m_boutonQuitter.add_child(m_Quitter);
    m_Quitter.set_pic_name("Quitter.bmp");
    //m_boite_boutons.set_bg_color(BLANC);

}


/*
    /// Le cadre d'ensemble de l'interface; x y coin sup. gauche, largeur, hauteur
    /// Le fond sera gris clair et on pourra le bouger à la souris (drag & drop)
    m_top_box.set_frame(400, 300, 200, 280);
    m_top_box.set_bg_color(GRISCLAIR);
    m_top_box.set_moveable();

    /// Les liens sont ajoutés en 1er pour que les autres éléments soient affichés au dessus
    m_top_box.add_child( m_lien1 );
    m_lien1.attach_from(m_img);
    m_lien1.attach_to(m_img_anime);

    m_top_box.add_child( m_lien2 );
    m_lien2.attach_from(m_boite_boutons);
    m_lien2.attach_to(m_img_anime);
    m_lien2.reset_arrow_with_bullet();

    m_lien2.add_child( m_lien2_label );
    m_lien2_label.set_message("RIEN");

    m_top_box.add_child( m_lien3 );
    m_lien3.attach_from(m_marche);
    m_lien3.attach_to(m_img_anime);
    m_lien3.add_item( {grman::ArrowItemType::Arrow, 0.4 }   );
    m_lien3.add_item( {grman::ArrowItemType::Arrow, 0.5, 14}   );
    m_lien3.add_item( {grman::ArrowItemType::Arrow, 0.6, 14, 1.0 }   );
    m_lien3.add_item( {grman::ArrowItemType::Triangle, 1.0 }   );

    /// On ajoute une image d'arbre calée au milieu en hauteur (par défaut) et à droite
    m_top_box.add_child( m_img );
    m_img.set_pic_name("tree0.bmp");
    m_img.set_gravity_x(grman::GravityX::Right);

    /// On ajoute une image animée au milieu en hauteur et en largeur (position par défaut)
    /// Le nom du fichier doit contenir le nombre de cases de l'animation entre xx et xx
    /// (voir le fichier crusty_xx4xx.png dans le répertoire pics du projet.
    m_top_box.add_child( m_img_anime );
    m_img_anime.set_pic_name("crusty_xx4xx.png");

    /// On ajoute une check-box en bas à gauche
    m_top_box.add_child( m_marche );
    m_marche.set_dim(40, 20);
    m_marche.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down );

    /// On ajoute un texte en bas centré
    m_top_box.add_child( m_legende );
    m_legende.set_message("<- CLICK ME");
    m_legende.set_gravity_y(grman::GravityY::Down );

    /// On ajoute en haut à gauche du cadre un sous-cadre (une boite dans la boite)
    m_top_box.add_child( m_boite_regles );
    m_boite_regles.set_dim(40, 80);
    m_boite_regles.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up );
    m_boite_regles.set_bg_color(SABLECLAIR);

    /// Dans ce sous cadre on ajoute 2 ascenseurs verticaux
    m_boite_regles.add_child( m_regle_reel );
    m_regle_reel.set_range(1, 10); // Par défaut valeurs réelles
    m_regle_reel.set_frame(4,5,15,70);

    m_boite_regles.add_child( m_regle_entier );
    m_regle_entier.set_range(1, 10, true); // 3ème param true => valeurs entières
    m_regle_entier.set_frame(20,5,15,70);
    m_regle_entier.set_value(6);


    /// On ajoute en haut à droite la boite à boutons
    m_top_box.add_child( m_boite_boutons );
    m_boite_boutons.set_dim(60,60);
    m_boite_boutons.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up );
    m_boite_boutons.set_bg_color(FUCHSIACLAIR);
    m_boite_boutons.set_moveable();

  /*  /// Puis un 1er bouton avec un texte
    m_boite_boutons.add_child( m_bouton1 );
    m_bouton1.set_frame(3,3,32,16);
    m_bouton1.set_bg_color(FUCHSIA);

    m_bouton1.add_child(m_bouton1_label);
    m_bouton1_label.set_message("NEW");

    /// Puis un 2eme bouton avec une image
    m_boite_boutons.add_child( m_bouton2 );
    m_bouton2.set_dim(40,30);
    m_bouton2.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    m_bouton2.set_bg_color(NOIR);
    m_bouton2.add_child(m_bouton2_image);
    m_bouton2_image.set_pic_name("clown_button.png");

    /// Puis la légende "DRAG ME" à droite
    m_boite_boutons.add_child( m_dragme );
    m_dragme.set_message("DRAG ME");
    m_dragme.set_vertical();
    m_dragme.set_gravity_x(grman::GravityX::Right);



}
*/
/// Une méthode update de la classe doit être appelée dans la boucle de jeu
/// et cette méthode doit propager l'appel à update sur les widgets contenus...
/// Cette méthode fait le lien entre l'interface, les événements, et les conséquences
int Thing::update(Graph& graph, std::string& nom_fichier)
{
    /// Si tous les widgets dépendants de l'objet sont dans une top box
    /// alors ce seul appel suffit (la propagation d'updates se fait ensuite automatiquement)
    /*m_menu.update();
    m_bouton1.update();
    m_bouton2.update();
    m_bouton3.update();
    m_boutonRegles.update();
    m_boutonQuitter.update();*/

    m_menu.update();

if ( m_bouton1.clicked() )
{
    std::cout << "Vous avez, en face de vous, le premier ecosysteme" << std::endl;
    graph.Charger_toutes_les_sommets_et_aretes("systemep");
    nom_fichier = "systemep";
    return 4;

}

if ( m_bouton2.clicked() )
{
    std::cout << "Vous avez, en face de vous, le second ecosysteme" << std::endl;
    /// Chargement fichier systeme NOM FONCTION

}

if ( m_bouton3.clicked() )
{
    std::cout << "Vous avez, en face de vous, le dernier ecosysteme" << std::endl;
    /// Chargement fichier systeme NOM FONCTION

}

if ( m_boutonRegles.clicked() )
{
    std::cout << "" << std::endl;
    /// Chargement fichier règles NOM FONCTION

}

if ( m_boutonQuitter.clicked() )
{
    std::cout << "Vous quittez le domaine d'études" << std::endl;
    exit(0);
}



else
    return 0;

/*
   /// Ici on montre la récupération de la valeur booléenne du check box en bas à gauche
    /// pour décider d'animer ou pas le widget d'image au centre
   m_img_anime.set_animate( m_marche.get_value() );

    /// Ici on montre la récupération de la valeur entière de l'ascenseur de droite
    /// pour décider du rythme d'animation
    m_img_anime.set_animate_tempo( m_regle_entier.get_value() );

    /// Ici on montre la récupération de la valeur réelle de l'ascenseur de gauche
    /// pour fixer l'ordonnée de l'image au centre
    m_img_anime.set_pos( m_img_anime.get_pos().x, 140-10.0*m_regle_reel.get_value() );

    /// Utilisation d'un bouton pour déclencher un événement
    /// L'accès à clicked() fait un reset : tant que le bouton n'est pas
    /// à nouveau cliqué les futurs accès à clicked seront faux
    /// ( Donc il faut appeler clicked() UNE FOIS ET UNE SEULE par update )

    if ( m_bouton1.clicked() )
    {
        std::cout << "NEW !" << std::endl;

        /// Expérimental, ajout dynamique d'éléments, ATTENTION ICI ALLOCATION
        grman::WidgetImage * clown = new grman::WidgetImage(); // On pourrait déclarer auto ...
        m_dynaclowns.push(clown);
        m_top_box.add_child( *clown );
        clown->set_pic_name("crusty_xx4xx.png");
        clown->set_pos(rand()%100+10, rand()%100+110 );
        clown->set_animate();
        clown->set_animate_tempo(rand()%5+3);

        /// Mise à jour du nombre de clowns dynamiques sur le label du lien
        m_lien2_label.set_message( std::to_string( m_dynaclowns.size() ) );
    }

    /// Utilisation de l'autre bouton (méchant clown) pour enlever un clown
    if ( m_bouton2.clicked() && !m_dynaclowns.empty() )
    {
        std::cout << "ARGHHH !" << std::endl;

        /// Expérimental, retrait dynamique d'éléments
        grman::WidgetImage * clown = m_dynaclowns.top(); // On pourrait déclarer auto ...
        m_dynaclowns.pop();

        // On est pas obligé d'enlever les éléments dans l'ordre inverse de leur ajout
        // il suffit de se souvenir de l'adresse de l'élément à supprimer
        m_top_box.remove_child( *clown );

        /// Le remove de la collection ne détruit pas l'objet
        /// C'est la responsabilité de la classe qui a fait new de faire delete
        delete clown;

        /// Mise à jour du nombre de clowns dynamiques sur le label du lien
        m_lien2_label.set_message( std::to_string( m_dynaclowns.size() ) );
    }*/
}



/// On a des allocations dynamiques dans m_dynaclowns => à nettoyer dans le destructeur
Thing::~Thing()
{

 /*   while ( !m_dynaclowns.empty() )
    {
        delete m_dynaclowns.top();
        m_dynaclowns.pop();
    }
*/
}


            /// SIMULATION ET EVOLUTION ////////////////////////////////////////////////

void Graph::evolution_pop()
{
    int predateur=0;
    int k=1;
    int Total=0;

    for( auto &elem: m_vertices)
    {
        facteurRepro(elem.first);
        k = calcul_K(elem.first);
        predateur = predation(elem.first);
        //std::cout << " population avant : " << elem.second.m_N_t << std::endl;
        elem.second.m_K = k;
        elem.second.m_N_dep= elem.second.m_N_ar;
        int w= (1-(elem.second.m_N_dep/k));


         /// Condition obligatoire sinon opération faussée /// Peut directement être écrit en int w = 1
    if (w==0)
        {
            w=1;
        }

        int wa = elem.second.m_N_dep;
        int hui = elem.second.m_ryth*(elem.second.m_N_dep)*w;
        int at = wa + hui + predateur;
            std::cout << "at = " << at << std::endl;


    for (int j=0;j<elem.second.m_in.size(); j++)
        {
            std::map <int,Edge> :: iterator iter;
            iter = m_edges.find(elem.second.m_in[j]);
            Total = Total + iter->second.m_weight;
            std::cout<< "Pour le sommet : " <<elem.first<<", le total sera de = " << Total <<std::endl;
        }

        elem.second.m_N_ar = at;



    if(Total==0)
        {
            elem.second.m_N_ar = 0;
        }

        if(elem.second.m_N_ar < 0)
        {
            elem.second.m_N_ar = 0;
        }
    if (elem.second.m_N_ar==42) /// 21 ?
    {
        elem.second.m_N_dep=0;
    }
        std::cout << " Numero du sommet en question : " << elem.first << std::endl; /// Id sommet
        std::cout << " Apres evolution,la population est : " << elem.second.m_N_dep << std::endl;
    }
    MAJarete();

    //std::cout << "----- FIN ----" << std::endl;
}


int Graph::predation(int num_vertex_donne) /// Indice du sommet donné
{
    int p = 0;
    int q = 0;
    std::vector<int> Total;

    for (auto &elem : m_edges)
    {
        if (elem.second.m_from == num_vertex_donne) /// Valeur KEY
        {
            std::map<int,Vertex>::iterator iter;
            iter = m_vertices.find(elem.second.m_to);
            /// Ici, on récupère la valeur du sommet voulu
            q = elem.second.m_weight;
            p = iter->second.m_N_dep;
            /// On ajoute à la suite l'influence de la prédation
            Total.push_back(p*q);
        }
    }
    int Totaux=0;
    for (unsigned int j=0;j<Total.size();j++)
    {
        Totaux =Total[j] + Totaux;
    }

    Totaux = -Totaux;
    /// Retourne une valeur négative de prédation car Totaux<0
    return Totaux;

}

int Graph::calcul_K(int num_vertex_donne)
{

    int p = 0;
    int q = 0;
    int Totaux=0;
    std::vector<int> Total;

    for (auto &elem : m_edges)
    {

        if (elem.second.m_to == num_vertex_donne)
        {
            std::map<int,Vertex>::iterator iter;
            iter = m_vertices.find(elem.second.m_from);
            p = iter->second.m_N_dep; // On prend la value du sommet en question
            q = elem.second.m_weight;
            //std::cout << " a = " << a << "b = " << b << std::endl;
            Total.push_back(p*q);
        }
    }

    for (unsigned int i=0;i<Total.size();i++)
    {
        Totaux =Total[i] + Totaux;
    }
    return Totaux;

}

void Graph::facteurRepro(int num_vertex_donne)
{
    std::map<int,Vertex>::iterator iter;
    iter = m_vertices.find(num_vertex_donne);
    //int pop_avant = it->second.m_value;
    iter->second.m_N_dep = ceil(iter->second.m_N_ar + (iter->second.m_N_ar)*0.55/2) ;

    //std::cout << "on reprodui le sommet " << num_vertex_donne <<std::endl;
    //std::cout << "Pop_avant : " << pop_avant << " Pop_apres " <<it->second.m_value <<std::endl;
}



void Graph::MAJarete()
{
    for (auto &elem : m_edges)
    {
        std::map<int,Vertex>::iterator iter;

        /// On récupère l'indice de l'arete d'ou l'on vient
        iter = m_vertices.find(elem.second.m_from);

        /// On récupère la valeur de l'arete que l'on veut
        int a = iter->second.m_N_dep;

        std::cout << "ARETE : " << elem.first << " avec un poids de : "<< elem.second.m_weight << std::endl;
        elem.second.m_weight = a/4;
        std::cout << "ARETE : " << elem.first << " avec un poids de : "<< elem.second.m_weight << std::endl;

        /// Condition imposée car sinon arete supprimée et pb dans calculs
        if (elem.second.m_weight< 0)
        {
            elem.second.m_weight = 1;
        }


    }
}
void Graph::Charger_toutes_les_sommets_et_aretes(std::string nom_systeme)
{
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ifstream fichier(tuvasmarcher, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }
    std::string blaze_photo;
    int taille;
    int indice, poid, x, y;
    fichier>>taille;
    for(int i=0; i < taille; i++)
    {
        fichier>>indice;
        fichier>> poid;
        fichier>>x;
        fichier>>y;
        fichier>>blaze_photo;
        m_nom_so[indice]=blaze_photo;
        add_interfaced_vertex(indice,poid,x,y,blaze_photo);
    }
    fichier>>taille;
    int Psom, Dsom;
    for(int j=0; j<taille; j++)
    {
        fichier>>indice;
        fichier>>Psom;
        fichier>>Dsom;
        fichier>>poid;
        add_interfaced_edge(indice, Psom, Dsom, poid);
    }

    fichier.close();
}

void Graph::Sauvegarde_sommets_aretes(std::string nom_systeme)
{
    std::string tuvasmarcher="Users/manol/Desktop/PISCINE/texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ofstream fichier(tuvasmarcher,std::ios::trunc);  //on ecrit
    if(!fichier)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    int fichier_taille;
    int x, y;
    std::string chiffre;
    std::string fin;
    fichier_taille=m_vertices.size();
    fichier<<m_vertices.size()<<std::endl;
    for(auto it= m_vertices.begin(); it!=m_vertices.end(); it++)
    {

        x=m_vertices[it->first].m_interface->m_top_box.get_posx()+2;
        y=m_vertices[it->first].m_interface->m_top_box.get_posy()+2;
        chiffre=std::to_string(it->first+1);
        fin=m_nom_so[it->first];
        //fin+=".jpg";
        std::cout<<"YEEES"<<std::endl;
        //if(!(m_vertices[it->first].m_value!=0)&&(x!=0)&&(y!=0))
        //{
        std::cout<<it->first<<" "<<m_vertices[it->first].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
        fichier<<it->first<<" "<<m_vertices[it->first].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
        //}
        //fin=nom_systeme;
    }
    fichier<<m_edges.size()<<std::endl;
    fichier_taille=m_edges.size();
    for(auto it = m_edges.begin(); it!=m_edges.end(); it++)
    {
        std::cout<<it->first<<" "<<m_edges[it->first].m_to<<" "<<m_edges[it->first].m_from<<" "<<m_edges[it->first].m_weight<<std::endl;

        fichier<<it->first<<" "<<m_edges[it->first].m_to<<" "<<m_edges[it->first].m_from<<" "<<m_edges[it->first].m_weight<<std::endl;
    }
    fichier.close();
    std::cout<<" dans save"<<std::endl;
}

void Graph::Sauvegarde_totale(std::string nom_systeme)
{
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ofstream fichier(tuvasmarcher,std::ios::trunc);  //on ecrit
    if(!fichier)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    int fichier_taille;
    int x, y;
    std::string chiffre;
    std::string fin;
    fichier_taille=m_vertices.size();
    std::cout<<fichier_taille<<std::endl;
    if(!m_so_asupp.empty())
    {
        fichier_taille=fichier_taille-m_so_asupp.size();
    }
    std::cout<<fichier_taille<<std::endl;
    std::cout<<m_so_asupp.size()<<std::endl;
    fichier<<fichier_taille<<std::endl;

    for(auto i = m_vertices.begin(); i!=m_vertices.end(); i++)
    {

        for(auto w=m_so_asupp.begin(); w!=m_so_asupp.end(); w++)
        {
            if(w->first!=i->first)
            {
                x=m_vertices[i->first].m_interface->m_top_box.get_posx()+2;
                y=m_vertices[i->first].m_interface->m_top_box.get_posy()+2;
                chiffre=std::to_string(i->first+1);
                fin=m_nom_so[i->first];
                //  if(!(m_vertices[i->first].m_value!=0)&&(x!=0)&&(y!=0))
                //{
                fichier<<i->first<<" "<<m_vertices[i->first].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
                //}
            }



        }

        //fin=nom_systeme;
    }
    fichier<<m_edges.size()<<std::endl;
    fichier_taille=m_edges.size();
    for(int j=0; j<fichier_taille; j++)
    {

        fichier<<j<<" "<<m_edges[j].m_to<<" "<<m_edges[j].m_from<<" "<<m_edges[j].m_weight<<std::endl;
    }
    std::cout<<" dans save"<<std::endl;
    fichier.close();

}

void Graph::test_remove_edge(int eidx)
{


    /// référence vers le Edge à enlever

    Edge &remed=m_edges.at(eidx);



    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;



    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;



    /// test : on a bien des éléments interfacés

    if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :

        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */

        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */

        /* m_edges[idx] = Edge(weight, ei); */

        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr

        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !


    m_edges.erase( m_edges.find(eidx) );





    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;



}

void Graph::test_remove_sommet(std::string nom_systeme, Graph g)
{
    ///cherche le nb de sommet existant
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ifstream fichier(tuvasmarcher, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }
    int nombre_de_sommet;
    int sommet_choisi;
    std::string chiffre, fin;
    int x, y;
    fichier>>nombre_de_sommet;
    fichier.close();

    ///Choix du sommet a retirer
    std::cout<<"Vous souhaitez retirer un sommet ? :"<<std::endl;
    std::cin>>sommet_choisi;
    while((sommet_choisi<0)||(sommet_choisi>(nombre_de_sommet-1)))//blindage
    {
        std::cout<<"resaisir svp entre 0 et "<<(nombre_de_sommet-1)<<" !"<<std::endl;
        std::cin>>sommet_choisi;
    }

    Vertex &remed=m_vertices.at(sommet_choisi);
    std::vector<int> yessai;
    std::map<int, Edge>::iterator it;
    if (m_interface && remed.m_interface)
    {
        m_interface->m_main_box.remove_child(remed.m_interface->m_top_box);
        m_vertices.erase(sommet_choisi);

        if(remed.m_in.empty() == false || remed.m_out.empty() == false)
        {
            for(auto it=m_edges.begin(); it!=m_edges.end(); ++it)
            {
                if((it->second.m_from == sommet_choisi) || (it->second.m_to ==sommet_choisi))
                {
                    Edge &remed=m_edges.at(it->first);
                    m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
                    yessai.push_back(it->first);

                }

            }
            for (auto j : yessai)
            {
                it= m_edges.find(j);
                m_edges.erase(it);
            }
        }

        Sauvegarde_sommets_aretes(nom_systeme);
        //g.update();
        std::cout<<" c'est bon!"<<std::endl;
    }
}
void Graph::ajout_arete(std::string nom_systeme, Graph g)
{
    int nombre_de_sommet, indice;
    int sommet_depart, sommet_arrive, poid;

    ///J'ouvre mon fichier en mode lecture
    std::map<int, Edge >::iterator it;
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ifstream fichier(tuvasmarcher, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    //je recupere le nb de sommet present
    fichier>>nombre_de_sommet;
    fichier.close();

    ///Debut des questions
    std::cout<<"VOUS VOULEZ AJOUTER UNE ARETE ! "<<std::endl;
    std::cout<<"De quel sommet part l'arete ? : "<<std::endl;
    std::cin>>sommet_depart;
    while((sommet_depart<0)||(sommet_depart>(nombre_de_sommet-1)))//blindage
    {
        std::cout<<"resaisir svp entre 0 et "<<(nombre_de_sommet-1)<<" !"<<std::endl;
        std::cin>>sommet_depart;
    }
    std::cout<<"De quel sommet part l'arete ? : "<<std::endl;
    std::cin>>sommet_arrive;
    while((sommet_arrive<0)||(sommet_arrive>(nombre_de_sommet-1)))//blindage
    {
        std::cout<<"resaisir svp entre 0 et "<<(nombre_de_sommet-1)<<" !"<<std::endl;
        std::cin>>sommet_arrive;
    }
    std::cout<<"Quel poid voulez vous donner à cette arete ? "<<std::endl;
    std::cin>>poid;

    indice = -1;
    do
    {
        indice++;
        it=m_edges.find(indice);

    }
    while(it!=m_edges.end());


    std::cout<<" c'est bon!"<<std::endl;
    std::cout<<indice<<std::endl;
    add_interfaced_edge(indice, sommet_depart, sommet_arrive, poid);

    Sauvegarde_sommets_aretes(nom_systeme);
    //g.update();


}

void Graph::ajout_sommet(std::string nom_systeme,Graph g)
{
    int nombre_de_sommet, indice;
    int x, y, poid;
    std::string nom_photo;

    ///J'ouvre mon fichier en mode lecture
    std::string tuvasmarcher="texte/";
    tuvasmarcher+="ajout_so";
    tuvasmarcher+=".txt";
    std::ifstream fichier(tuvasmarcher, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    //je recupere le nb de sommet present

    fichier>>nom_photo;
    fichier.close();
    std::string tuvasmarcher2="texte/";
    tuvasmarcher2+=nom_systeme;
    tuvasmarcher2+=".txt";
    std::ifstream fichier2(tuvasmarcher2, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier2)  // si l'ouverture n'a pas réussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    //je recupere le nb de sommet present
    fichier2>>nombre_de_sommet;
    fichier2.close();

    std::cout<<"quel est le poid de votre sommet ?"<<std::endl;
    std::cin>>poid;
    std::cout<<"quel est son abscisse ? "<<std::endl;
    std::cin>>x;
    std::cout<<"quel est son ordonne ?"<<std::endl;
    std::cin>>y;
    std::map<int, Vertex>::iterator it;
    int indi =-1;
    do
    {
        indi++;
        it=m_vertices.find(indi);

    }
    while(it!=m_vertices.end());

    m_so_asupp[indi]="systemep0";
    std::cout<<indi<<std::endl;

    add_interfaced_vertex(indi,poid,x,y,nom_photo);
    Sauvegarde_sommets_aretes(nom_systeme);
    //g.update();
}

void Graph::matrice_adj() //complete la matrice adjacence
 {
     adj.resize( m_vertices.size(), std::vector<int>( m_vertices.size(), 0 ) );
     for ( auto &elem : m_edges ) {
         adj[elem.second.m_from][elem.second.m_to] = 1;
     }
     for ( unsigned int i = 0; i < m_vertices.size(); i++ ) {
         for ( unsigned int j = 0; j < m_vertices.size(); j++ ) {
             std::cout << adj[i][j] << " ";
         }
         std::cout << std::endl;
     }
 }

std::vector<std::vector<int>> Graph::algo_de_toute_les_composantes_connexes()
{
    matrice_adj();//reactualise la matrice d'adjacence
    std::vector<std::vector<int>> tab_cfc; // tab des composantes fortements connexes
    std::vector<int> marquage;           // tab de marquages
    int a, b;                           // sommets intermediaires
    ///allocation memoire de tab_cfc et marques
    tab_cfc.resize( m_vertices.size() ); //alloue la place disponible que donne la liste de sommets
    for ( int i = 0; i < m_vertices.size(); i++ )
    {
        tab_cfc.at( i ).resize( m_vertices.size() );//complète les y
    }
    marquage.resize( m_vertices.size() );//alloue de la place pour le tableau de marquage

    for ( int i = 0; i < m_vertices.size(); i++ ) // initialise les deux tableau à 0
    {
        marquage.at( i ) = 0;
        for ( int j = 0; j < m_vertices.size(); j++ )
        {
            tab_cfc.at( i ).at( j ) = 0;
        }
    }
    int k = 0;
    for ( a = 0; a < m_vertices.size(); a++ )
    {
        if ( !marquage[a] )//si different de la valeur actuelle présente dans le tableau de marquage
        {
            tab_cfc[a] = trouver_une_composante_connexe(a);
            marquage[a] = 1;
        }
    }
    std::cout << "K :" << k;
    std::ofstream fic(
        "test.txt", std::ios::out | std::ios::trunc ); // droit d ecriture
    // avec effacement du
    // fichier ouvert
    if ( fic )
    {
        for ( int m = 0; m < m_vertices.size(); m++ )
        {
            for ( int n = 0; n < m_vertices.size(); n++ )
            {
                fic<< tab_cfc.at( m ).at( n ) << " ";
            }
            fic << std::endl;
        }
        fic.close();
    }
    else
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
    return tab_cfc ;
}

std::vector<int> Graph::trouver_une_composante_connexe(int S)
{
    std::vector<int> c1, c, c2, marquage;
    int a, b;
    int add = 1;
    // allocation memoire
    c1.resize( m_vertices.size() );
    c2.resize( m_vertices.size() );
    c.resize( m_vertices.size() );
    marquage.resize( m_vertices.size() );
    // init valeur a 0
    for ( int i = 0; i < m_vertices.size(); i++ )
    {
        c1.at( i ) = 0;
        c2.at( i ) = 0;
        c.at( i ) = 0;
        marquage.at( i ) = 0;
    }
    // on rend le sommet s connexe
    c1[S] = 1;
    c2[S] = 1;
    // recherche des composantes connexes partant de s a ajouter a c1
    while ( add )
    {
        add = 0;
        for ( a = 0; a < m_vertices.size(); a++ )
        {
            if ( !marquage[a] && c1[a] )//si c'est pas marqué
            {
                marquage[a] = 1;//marquage
                for ( b = 0; b < m_vertices.size(); b++ )
                {
                    // std::cout <<" val :"<<adjacence.at(x).at(y);
                    if ( adj[a][b] && !marquage[b] )   // ERROR HERE
                    {
                        c1[b] = 1;
                        add = 1;
                    }
                }
            }
        }
    }
    for ( int i = 0; i < m_vertices.size(); i++ )
    {
        marquage.at( i ) = 0;
    }
    add = 1;
    std::vector<std::vector<int>> Tab_adj = intervertir();
    // recherche des composantes connexes arrivant a S a ajouter dans c2
    while ( add )
    {
        add = 0;
        if ( !marquage[a] && c2[a] )
        {
            marquage[a] = 1;
            for ( b = 0; b < m_vertices.size(); b++ )
            {
                // std::cout <<" val :"<<adjacence.at(x).at(y);
                if ( adj[b][a] && !marquage[b] )   // ERROR HERE
                {
                    c2[b] = 1;
                    add = 1;
                }
            }
        }
    }

// composante fortement connexe c =intersection de c1 et c2
    for ( int a = 0; a < m_vertices.size(); a++ )
{
    c[a] = c1[a] & c2[a];
    //        std::cout << c1[x] << " " ;
}
//    std::cout << std::endl;
return c;
}

std::vector<std::vector<int>> Graph::intervertir() //interverti la matrice et la renvoie
{
     std::vector<std::vector<int>> tab;
     tab.resize( m_vertices.size(),
               std::vector<int>( m_vertices.size(), 0 ) ); // resize double vector
     for ( int i = 0; i < m_vertices.size(); i++ ) {
         for ( int j = 0; j < m_vertices.size(); j++ ) {
             tab[i][j] = adj[j][i]; // on inverse
         }

     }
     return tab;
 }
