#include "graph.h"
#include<fstream>
#include<iostream>
#include<string>
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example(Graph g)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

    Charger_toutes_les_sommets_et_aretes("systemep");
    /* add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
     add_interfaced_vertex(1, 60.0, 400, 100, "moi-profP.jpg",0);
     add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
     add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
     add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
     add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
     add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
     add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);
     //add_interfaced_vertex(8, 60.0, 400, 100, "moi-profP.jpg");*/

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...

    /*add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);*/
    //ajout_arete("systemep", g);
    //ajout_sommet("systemep", g );
    std::cout<<" c'est bon!"<<std::endl;
    test_remove_sommet("systemep", g);
    //int x;
    //std::cout<<"quelle arete va etre retir� ?"<<std::endl;
    //std::cin>>x;
    //test_remove_edge(x);
    //test_remove_sommet("systemep");

    Sauvegarde_totale("systemep");
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide � l'ajout d'arcs interfac�s
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
    m_edges[idx].m_from=id_vert2;
    m_edges[idx].m_to=id_vert1;
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);

}

void Graph::Charger_toutes_les_sommets_et_aretes(std::string nom_systeme)
{
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ifstream fichier(tuvasmarcher, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier)  // si l'ouverture n'a pas r�ussi
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
void Graph::Savegarde_sommets_aretes(std::string nom_systeme)
{
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ofstream fichier(tuvasmarcher,std::ios::trunc);  //on ecrit
    if(!fichier)  // si l'ouverture n'a pas r�ussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    int fichier_taille;
    int x, y;
    std::string chiffre;
    std::string fin;
    fichier_taille=m_vertices.size();
    fichier<<m_vertices.size()<<std::endl;
    for(int i=0; i<fichier_taille; i++)
    {

        x=m_vertices[i].m_interface->m_top_box.get_posx()+2;
        y=m_vertices[i].m_interface->m_top_box.get_posy()+2;
        chiffre=std::to_string(i+1);
        fin=nom_systeme+chiffre;
        fin+=".jpg";
        fichier<<i<<" "<<m_vertices[i].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
        //fin=nom_systeme;
    }
    fichier<<m_edges.size()<<std::endl;
    fichier_taille=m_edges.size();
    for(int j=0; j<fichier_taille; j++)
    {

        fichier<<j<<" "<<m_edges[j].m_to<<" "<<m_edges[j].m_from<<" "<<m_edges[j].m_weight<<std::endl;
    }
    std::cout<<" dans save"<<std::endl;
}

void Graph::Sauvegarde_totale(std::string nom_systeme)
{
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ofstream fichier(tuvasmarcher,std::ios::trunc);  //on ecrit
    if(!fichier)  // si l'ouverture n'a pas r�ussi
    {
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    }

    int fichier_taille;
    int x, y;
    std::string chiffre;
    std::string fin;
    fichier_taille=m_vertices.size();
    fichier<<m_vertices.size()<<std::endl;
    for(int i=0; i<fichier_taille; i++)
    {

        for(int w=0; w<m_so_asupp.size();w++)
        {
            if(w!=i)
            {
                x=m_vertices[i].m_interface->m_top_box.get_posx()+2;
                y=m_vertices[i].m_interface->m_top_box.get_posy()+2;
                chiffre=std::to_string(i+1);
                fin=nom_systeme+chiffre;
                fin+=".jpg";
                fichier<<i<<" "<<m_vertices[i].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
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

}

/// eidx index of edge to remove

void Graph::test_remove_edge(int eidx)
{


    /// r�f�rence vers le Edge � enlever

    Edge &remed=m_edges.at(eidx);



    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;



    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;



    /// test : on a bien des �l�ments interfac�s

    if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :

        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */

        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */

        /* m_edges[idx] = Edge(weight, ei); */

        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr

        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)

        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)

    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

    m_edges.erase( eidx );





    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2

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
    if(!fichier)  // si l'ouverture n'a pas r�ussi
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
    std::ofstream f(tuvasmarcher, std::ios::trunc);
    f<<nombre_de_sommet<<std::endl;
    std::cout<<" c'est bon!"<<std::endl;
    for(int i =0; i<nombre_de_sommet; i++)
    {
        if((i!=sommet_choisi)&&(i<sommet_choisi))
        {
            std::cout<<"OK1"<<std::endl;
            x=m_vertices[i].m_interface->m_top_box.get_posx()+2;
            y=m_vertices[i].m_interface->m_top_box.get_posy()+2;
            chiffre=std::to_string(i+1);
            fin=nom_systeme+chiffre;
            fin+=".jpg";
            f<<i<<" "<<m_vertices[i].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
            fin=nom_systeme;
        }
        if((i!=sommet_choisi)&&(i>sommet_choisi))
        {
            std::cout<<"OK2"<<std::endl;
            x=m_vertices[i].m_interface->m_top_box.get_posx()+2;
            y=m_vertices[i].m_interface->m_top_box.get_posy()+2;
            chiffre=std::to_string(i);
            fin=nom_systeme+chiffre;
            fin+=".jpg";
            f<<i-1<<" "<<m_vertices[i].m_value<<" "<<x<<" "<<y<<" "<<fin<<std::endl;
            fin=nom_systeme;
        }
    }
    f<<m_edges.size()<<std::endl;
    int fichier_taille=m_edges.size();
    for(int j=0; j<fichier_taille; j++)
    {

        f<<j<<" "<<m_edges[j].m_to<<" "<<m_edges[j].m_from<<" "<<m_edges[j].m_weight<<std::endl;
    }
    f.close();
    Savegarde_sommets_aretes( nom_systeme);
    g.update();
    std::cout<<" c'est bon!"<<std::endl;

}

void Graph::ajout_arete(std::string nom_systeme, Graph g)
{
    int nombre_de_sommet, indice;
    int sommet_depart, sommet_arrive, poid;

    ///J'ouvre mon fichier en mode lecture
    std::string tuvasmarcher="texte/";
    tuvasmarcher+=nom_systeme;
    tuvasmarcher+=".txt";
    std::ifstream fichier(tuvasmarcher, std::ios::in);  // on ouvre le fichier en lecture
    if(!fichier)  // si l'ouverture n'a pas r�ussi
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
    std::cout<<"Quel poid voulez vous donner � cette arete ? "<<std::endl;
    std::cin>>poid;

    indice = nombre_de_sommet-1;

    std::cout<<" c'est bon!"<<std::endl;
    add_interfaced_edge(indice, sommet_depart, sommet_arrive, poid);
    Savegarde_sommets_aretes(nom_systeme);
    g.update();


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
    if(!fichier)  // si l'ouverture n'a pas r�ussi
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
    if(!fichier2)  // si l'ouverture n'a pas r�ussi
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
    m_so_asupp.push_back(nombre_de_sommet);
    add_interfaced_vertex(nombre_de_sommet,poid,x,y,nom_photo);
    Savegarde_sommets_aretes(nom_systeme);
    g.update();



}
