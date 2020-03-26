#include "mesh.h"
#include <iostream>
#include <fstream>

Mesh::Mesh()
{
    //createFromData("/home/vault/Code/m2_ds/Mesh_Computational_Geometry/cube.off");
    createFromData("/home/vault/Code/m2_ds/Mesh_Computational_Geometry/quen.off");

    for(int i = 0; i < vertexTab.length(); i++) {
        vertexTab[i].setLocalCurvature(getLocalCurvature(i));
    }

    std::cout<<"Taille vertex : "<<vertexTab.length()<<std::endl;
    std::cout<<"Taille faces : "<<facesTab.length()<<std::endl;




    //Pour tester le split d'une face
    //splitFace(2);

    //Pour tester le flip de l'arête entre deux faces
    // flip(3,2);


}


// Draw a vertex
void glVertexDraw(const Point & p) {
    glVertex3f(p.x(), p.y(), p.z());
}

//Convertit les données de courbure en RDB pour la représentation graphique
void HSVtoRGB(int output[], int H, double S, double V) {
    double C = S * V;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs, Gs, Bs;
    int coef = INT_MAX;

    if(H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    }
    else if(H >= 60 && H < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    }
    else if(H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    }
    else if(H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    }
    else if(H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    }
    else {
        Rs = C;
        Gs = 0;
        Bs = X;
    }

    output[0] = (Rs + m) * coef;
    output[1] = (Gs + m) * coef;
    output[2] = (Bs + m) * coef;

}


//Face de référence (renvoie la Face)
Face Mesh::getFace(Vertex point){
    int numFace = point.getNumFace();
    return facesTab[numFace];
}

//Lire un fichier .off
void Mesh::createFromData(std::string path){

    std::fstream offFile(path, std::ios_base::in);
    int nLine = 0;
    std::fstream line;

    if (offFile.is_open())
    {
      int nbVertices, nbFaces, _;
      offFile>>nbVertices>>nbFaces>>_;
      while ( nLine < nbVertices){
          double x, y, z;
          offFile>>x>>y>>z;

          Vertex newPoint = Vertex(x,y,z, nLine);
          vertexTab.push_back(newPoint);

          nLine++;
      }
      nLine = 0;
      std::map <std::pair<int,int>, int> edgemap; // Map des arêtes qui auront été déjà rencontrées par le passé
      while ( nLine < nbFaces){
          int nVert;
          offFile>>nVert;
          if(nVert != 3){
              std::cerr<<"Can only read triangle faces";
              return;
          }
          int points[nVert];
          for (int i = 0; i < nVert; i++){
              int iPoint;
              offFile>>iPoint;
              points[i] = iPoint;
          }

          //Création de la face
          Face newFace = Face(points[0],points[1],points[2],nLine);
          //Ajout de la face comme face de référence à chaque sommet qui n'en n'a pas déjà (par défaut à -1)
          if (vertexTab[points[0]].getNumFace() < 0){vertexTab[points[0]].setNumFace(nLine);}
          if (vertexTab[points[1]].getNumFace() < 0){vertexTab[points[1]].setNumFace(nLine);}
          if (vertexTab[points[2]].getNumFace() < 0){vertexTab[points[2]].setNumFace(nLine);}
          //Ajout de la face à la liste des faces du maillage
          facesTab.append(newFace);




          //Gestion des adjacences : première adjacence, sur l'arête entre le point1 et le point2
          if ((edgemap.find(std::make_pair(points[0],points[1])) == edgemap.end()) &&
                  (edgemap.find(std::make_pair(points[1],points[0])) == edgemap.end())){ //Si on n'a pas encore rencontré cette arête
              //Alors la face n'a pas encore de face adjacente : on ajoute alors simplement l'arête aux arêtes rencontrées, avec l'indice de face correspondant
              edgemap[std::make_pair(points[0],points[1])] = nLine;
              edgemap[std::make_pair(points[1],points[0])] = nLine;
          } else {//Si on a déjà rencontré cette arête
            if (edgemap.find(std::make_pair(points[0],points[1])) != edgemap.end()){//Si on l'a recontré dans le sens point1,point2
                int autreFace = edgemap[std::make_pair(points[0],points[1])]; // On va chercher l'indice de la face
                addAdjacence(autreFace,nLine, points[0], points[1]); //On créé l'adjacence
                edgemap.erase(std::make_pair(points[0],points[1])); //L'arête ne peut concerner qu'une face : on n'a donc plus besoin de l'arête qu'on peut enlever de la map
                edgemap.erase(std::make_pair(points[1],points[0]));
            }
            else{ //Rajout du cas où point2,point1 n'existe pas, juste au cas où (normalement ça n'arrive jamais)
                int autreFace = edgemap[std::make_pair(points[1],points[0])];
                addAdjacence(autreFace,nLine, points[0], points[1]);
                edgemap.erase(std::make_pair(points[1],points[0]));
                edgemap.erase(std::make_pair(points[0],points[1]));
            }
          }



          //Gestion des adjacences : deuxième adjacence, sur l'arête entre le point1 et le point3
          //Le fonctionnement est identique
          if ((edgemap.find(std::make_pair(points[0],points[2])) == edgemap.end()) &&
                  (edgemap.find(std::make_pair(points[2],points[0])) == edgemap.end())){
              edgemap[std::make_pair(points[0],points[2])] = nLine;
              edgemap[std::make_pair(points[2],points[0])] = nLine;

          } else {
            if (edgemap.find(std::make_pair(points[0],points[2])) != edgemap.end()){
                int autreFace = edgemap[std::make_pair(points[0],points[2])];
                addAdjacence(autreFace,nLine, points[0], points[2]);
                edgemap.erase(std::make_pair(points[0],points[2]));
                edgemap.erase(std::make_pair(points[2],points[0]));
            }
            else{
                int autreFace = edgemap[std::make_pair(points[2],points[0])];
                addAdjacence(autreFace,nLine, points[0], points[2]);
                edgemap.erase(std::make_pair(points[2],points[0]));
                edgemap.erase(std::make_pair(points[0],points[2]));
            }
          }



          //Gestion des adjacences : troisième adjacence, sur l'arête entre le point3 et le point2
          //Le fonctionnement est identique
          if ((edgemap.find(std::make_pair(points[2],points[1])) == edgemap.end()) &&
                  (edgemap.find(std::make_pair(points[1],points[2])) == edgemap.end())){
              edgemap[std::make_pair(points[2],points[1])] = nLine;
              edgemap[std::make_pair(points[1],points[2])] = nLine;
          } else {
            if (edgemap.find(std::make_pair(points[2],points[1])) != edgemap.end()){
                int autreFace = edgemap[std::make_pair(points[2],points[1])];
                addAdjacence(autreFace,nLine, points[2], points[1]);
                edgemap.erase(std::make_pair(points[2],points[1]));
                edgemap.erase(std::make_pair(points[1],points[2]));
            }
            else{
                int autreFace = edgemap[std::make_pair(points[1],points[2])];
                addAdjacence(autreFace,nLine, points[2], points[1]);
                edgemap.erase(std::make_pair(points[1],points[2]));
                edgemap.erase(std::make_pair(points[2],points[1]));
            }
          }

          nLine++;
      }
      offFile.close();
    }
    else std::cerr << "Unable to open file";
}

//Test d'adjacence entre les faces
void Mesh::testAdjRandom(){
    int ind = rand()%facesTab.length();
    Face face = facesTab[ind];
    int * facesInd = face.getAdjFaces();
    std::cout<<"Face n°"<<ind<<std::endl<<std::endl; //Face de test

    for (int i = 0; i < 3;i++){
        Face faceToCompare = facesTab[facesInd[i]]; //
        std::cout<<"La face : "<<facesInd[i]<<std::endl; //Face dont l'adjacence est testée
        std::cout<<"Adjacent            : "<<face.verifyAdj(faceToCompare)<<std::endl; //On vérifie l'adjacence dans un sens
        std::cout<<"Adjacent réciproque : "<<faceToCompare.verifyAdj(face)<<std::endl; //Puis dans l'autre
        std::cout<<"Bien dans la liste  : "<<faceToCompare.hasAdjFace(face)<<std::endl;// On vérifie que la face de test est bien dans la liste d'adjacence de la liste adjacente
    }
}

//Ajout d'une adjacence dans le tableau d'adjacence
void Mesh::addAdjacence(Face& face1, Face& face2, int point1, int point2){
    face1.addAdjFace(face2.getIndice(), point1, point2);
    face2.addAdjFace(face1.getIndice(), point1, point2);
}
void Mesh::addAdjacence(int indFace1, int indFace2, int indPoint1, int indPoint2){
    facesTab[indFace1].addAdjFace(indFace2, indPoint1, indPoint2);
    facesTab[indFace2].addAdjFace(indFace1, indPoint1, indPoint2);
}



//Méthodes des itérateurs et circulateurs
Iterator_on_faces Mesh::beginFaces(){
    Iterator_on_faces it(this);
    return it;
}

Iterator_on_faces Mesh::endFaces(){
    Iterator_on_faces it(this);
    it.setIndice(this->getNbFaces());
    return it;
}

Iterator_on_vertices Mesh::beginVertices(){
    Iterator_on_vertices it(this);
    return it;
}

Iterator_on_vertices Mesh::endVertices(){
    Iterator_on_vertices it(this);
    it.setIndice(this->getNbVertices());
    return it;
}

Circulator_on_faces Mesh::beginCircFaces(int point){
    Circulator_on_faces circ(this, point);
    return circ;
}

Circulator_on_faces Mesh::endCircFaces(int point){
    Circulator_on_faces circ(this, point);
    circ.setIndFace(circ.getIndFace());
    return circ;
}

Circulator_on_vertices Mesh::beginCircVertices(int point){
    Circulator_on_vertices circ(this, point);
    return circ;
}

Circulator_on_vertices Mesh::endCircVertices(int point){
    Circulator_on_vertices circ(this, point);
    Face* firstFace = this->getFacePointeur(circ.getCircFace().getIndFace());
    int sommetPrecedentPlace = ((firstFace->getPlacePoint(circ.getIndPointCentral()))+1)%3;
    circ.setPoint(firstFace->point(sommetPrecedentPlace));
    return circ;
}


//Flip de deux arêtes
void  Mesh::flip(int indF0, int indF1) {
    int * af0 = facesTab[indF0].getAdjFaces();
    int i = 0;
    while (i < 3) {
        if (af0[i] == indF1) break;
        i++;
    }
    int * af1 = facesTab[indF1].getAdjFaces();
    int j = 0;
    while (j < 3) {
        if (af1[j] == indF0) break;
        j++;
    }

    // On échange les points
    Face* face0 = &(facesTab[indF0]);
    Face* face1 = &(facesTab[indF1]);
    int Pj = face1->point(j);
    int Pi = face0->point(i);

    face0->setPoint((i+2)%3, Pj);
    face1->setPoint((j+2)%3, Pi);

    // On met à jour les faces adjacentes des faces adjacentes de f0 et f1
    int ia0 = facesTab[indF0].getAdjFaces()[(i+1)%3];
    Face a0 = facesTab[ia0];
    a0.getAdjFaces()[a0.getPlaceDernierPoint(facesTab[indF1].point((j+1)%3), facesTab[indF1].point((j+2)%3))] = indF1;
    int ia1 = facesTab[indF1].getAdjFaces()[(j+1)%3];
    Face a1 = facesTab[ia1];
    a1.getAdjFaces()[a1.getPlaceDernierPoint(facesTab[indF0].point((i+1)%3), facesTab[indF0].point((i+2)%3))] = indF0;

    // On met à jour des adjacences dans f0
    af0[i] = ia1;
    af0[(i+1)%3] = indF1;

    // On met à jour des adjacences dans f1
    af0[j] = ia0;
    af0[(j+1)%3] = indF0;

    // Mise à jour des points
    vertexTab[facesTab[indF0].point((i+1)%3)].setNumFace(indF0);
    vertexTab[facesTab[indF1].point((j+1)%3)].setNumFace(indF1);

}

double Mesh::getLocalCurvature(int point){
    double lx = 0; // Laplacien selon x
    double ly = 0; // Laplacien selon y
    double lz = 0; // Laplacien selon z
    double s = 0; // Surface
    Vertex* pi = this->getPointPointeur(point);
    int i = 0;

    Circulator_on_faces cf = this->beginCircFaces(point);
    do {
        int iPi = cf->getPlacePoint(point);
        double a = this->cotan(cf.getIndFace(), iPi+2 % 3);
        Vertex* pj = this->getPointPointeur(cf->point((iPi + 1) % 3));
        Face* f1 = this->getFacePointeur(cf->getAdjFaces()[(iPi + 2) % 3]);
        int iPj = f1->getPlacePoint(point);
        double b = this->cotan(f1->getIndice(), iPj+2 % 3);

        s += this->getSurface(cf.getIndFace())/3.;
        lx += (pj->getPoint()->x() - pi->getPoint()->x())*a*b;
        ly += (pj->getPoint()->y() - pi->getPoint()->y())*a*b;
        lz += (pj->getPoint()->z() - pi->getPoint()->z())*a*b;
        i++;

        ++cf;
    } while (!(cf == this->endCircFaces(point)));

    return (std::sqrt(lx*lx + ly*ly + lz*lz)/(2.*s))/2.;
}

//Split d'une Face
void Mesh::splitFace(int indFace, Vertex _point){
    _point.setIndice(vertexTab.length());
    vertexTab.push_back(_point);

    int indNewPoint = vertexTab.length()-1;

    //Initialisation des variables qui vont changer

    Face* _face0 = &facesTab[indFace];
    int sommet0 = _face0->point(0);
    int face1 = _face0->getAdjFaces()[1];
    int face2 = _face0->getAdjFaces()[2];


    //Création des deux nouvelles faces
    int indFacet = facesTab.length();
    int indFacet2 = facesTab.length()+1;
    Face newFacet = Face(indNewPoint,_face0->point(2),sommet0, indFacet);
    Face newFacet2 = Face(indNewPoint,sommet0,_face0->point(1),indFacet2);


    //Définition des adjacences des nouvelles faces
    _face0->getAdjFaces()[1] = indFacet;
    newFacet.getAdjFaces()[0] = face1;
    newFacet.getAdjFaces()[1] = indFacet2;
    newFacet.getAdjFaces()[2] = indFace;


    _face0->getAdjFaces()[2] = indFacet2;
    newFacet.getAdjFaces()[0] = face2;
    newFacet.getAdjFaces()[1] = indFace;
    newFacet.getAdjFaces()[2] = indFacet;

    //Ajout des nouvelles faces
    facesTab.push_back(newFacet);
    facesTab.push_back(newFacet2);


    //Changement d'un sommet pour if0
    _face0->setPoint(0,indNewPoint);


    //Redéfinition des adjacences
    // de if0
   _face0->getAdjFaces()[1] = indFacet;
   _face0->getAdjFaces()[2] = indFacet2;
    // de if1
   facesTab[face1].getAdjFaces()[(facesTab[face1].getPlacePoint(sommet0)-1)%3] = indFacet;
    // de if2
   facesTab[face2].getAdjFaces()[(facesTab[face2].getPlacePoint(sommet0)+1)%3] = indFacet2;


   //Redéfinition des faces de référence des sommets pour ceux qui ont changé de faces
   vertexTab[sommet0].setNumFace(indFacet2);
   vertexTab[indNewPoint].setNumFace(indFace);

}

//Dessin des faces de couleurs différentes
void Mesh::drawMesh() {
    int col1 = 0;
    int col2 = 0;
    int col3 = 0;

    for(int i = 0; i < facesTab.length(); i+=1) {


        if (i%3 == 0) col1++;
        else if (i%3 == 1) col2++;
        else if (i%3 == 2) col3++;
        glColor3d(((float) (col1 % 5))/5,((float) (col2 % 5))/5,((float) (col3 % 5))/5);


        Face face = facesTab[i];
        glBegin(GL_TRIANGLES);
            glVertexDraw(*vertexTab[face.point(0)].getPoint());
            glVertexDraw(*vertexTab[face.point(1)].getPoint());
            glVertexDraw(*vertexTab[face.point(2)].getPoint());
        glEnd();
    }


}

//Dessin des arêtes
void Mesh::drawMeshWireFrame() {
    for(int i = 0; i < facesTab.length(); i+=1) {
        Face face = facesTab[i];
        glBegin(GL_LINE_STRIP);
            glVertexDraw(*vertexTab[face.point(0)].getPoint());
            glVertexDraw(*vertexTab[face.point(1)].getPoint());
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(*vertexTab[face.point(1)].getPoint());
            glVertexDraw(*vertexTab[face.point(2)].getPoint());
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(*vertexTab[face.point(2)].getPoint());
            glVertexDraw(*vertexTab[face.point(0)].getPoint());
        glEnd();
    }
}

//Dessin des sommets en mettant en éivdence la courbure
void Mesh::drawMeshPoints() {
    int color[3];
    for(int i = 0; i < vertexTab.length(); i++) {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;

        const double s = 0.99;
        const double l = 0.99;
        const double max = 20;
        const double min = 0;
        HSVtoRGB(color, 120. + (360. - 120.)*(std::min(vertexTab[i].getLocalCurvature(), max) - min) / (max - min), s, l);
        glColor3i(color[0], color[1], color[2]);
        glBegin(GL_POINTS);
            glVertexDraw(*vertexTab[i].getPoint());
        glEnd();
    }
}

//Dessin des faces en couleur : deux sont mises en évidence (utilisé pour tester et montrer le flip)
void Mesh::drawMeshTwoFaces(int face1, int face2) {

    for(int i = 0; i < facesTab.length(); i+=1) {


        //Toutes les faces sont en nuances de bleu sauf les deux mises en évidences, en rouge et en vert
        if (i==face1) glColor3d(1,0,0);
        else if (i==face2) glColor3d(0,1,0);
        else glColor3d(0,0,((double) (i%12))/12);

        Face face = facesTab[i];
        glBegin(GL_TRIANGLES);
            glVertexDraw(*vertexTab[face.point(0)].getPoint());
            glVertexDraw(*vertexTab[face.point(1)].getPoint());
            glVertexDraw(*vertexTab[face.point(2)].getPoint());
        glEnd();
    }

}
