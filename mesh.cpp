#include "mesh.h"

Mesh::Mesh()
{
    createFromData("E:\\Centrale\\Pougne\\4A\\MSO - MOS\\C++ etc\\cube.off");

    std::cout<<"Taille vertex : "<<vertexTab.length()<<std::endl;
    std::cout<<"Taille faces : "<<facesTab.length()<<std::endl;




    //If you want to split a face
    //splitFace(2);

    //If you want to flip two faces
    flip(3,2);


}


// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Point & p) {
    glVertex3f(p.x(), p.y(), p.z());
}


// Get the face of a point
Face Mesh::getFace(Vertice point){
    int numFace = point.getNumFace();
    return facesTab[numFace];
}


// read a off file
void Mesh::createFromData(std::string path){
    std::string line;
    std::ifstream thefile;
    thefile.open(path.c_str());
    getline(thefile,line);
    std::string delimiter = " ";
    std::cout<<line<<std::endl;



    //Première ligne
    int nbVertices;
    int nbFaces;

    std::string substring = line.substr(0,line.find(delimiter));
    nbVertices =  std::atoi(substring.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());

    substring = line.substr(0,line.find(delimiter));
    nbFaces =  std::atoi(substring.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());

    std::cout<<nbVertices<<" "<<nbFaces<<std::endl;

    //Les points
    float x_i;
    float y_i;
    float z_i;


    for (int i = 0; i < nbVertices; i++){
        getline(thefile,line);


        substring = line.substr(0,line.find(delimiter));
        x_i =  std::stof(substring);
        line.erase(0, line.find(delimiter) + delimiter.length());

        substring = line.substr(0,line.find(delimiter));
        y_i =  std::stof(substring);
        line.erase(0, line.find(delimiter) + delimiter.length());

        substring = line.substr(0,line.find(delimiter));
        z_i =  std::stof(substring);
        line.erase(0, line.find(delimiter) + delimiter.length());

        Vertice newPoint = Vertice(x_i,y_i,z_i, i);

        vertexTab.push_back(newPoint);
        std::cout<<line<<std::endl;
    }



    // Les faces
    int idPoint1;
    int idPoint2;
    int idPoint3;

    std::map <std::pair<int,int>, int> edgemap;

    for (int i = 0; i < nbFaces; i++){
        getline(thefile,line);

        //TODO : S'adapter au nombre de sommets
        line.erase(0, line.find(delimiter) + delimiter.length());

        substring = line.substr(0,line.find(delimiter));
        idPoint1 =  std::atoi(substring.c_str());
        line.erase(0, line.find(delimiter) + delimiter.length());

        substring = line.substr(0,line.find(delimiter));
        idPoint2 =  std::atoi(substring.c_str());
        line.erase(0, line.find(delimiter) + delimiter.length());

        substring = line.substr(0,line.find(delimiter));
        idPoint3 =  std::atoi(substring.c_str());
        line.erase(0, line.find(delimiter) + delimiter.length());


        Face newFace = Face(idPoint1,idPoint2,idPoint3,i);
        if (vertexTab[idPoint1].getNumFace() < 0){vertexTab[idPoint1].setNumFace(i);}
        if (vertexTab[idPoint2].getNumFace() < 0){vertexTab[idPoint2].setNumFace(i);}
        if (vertexTab[idPoint3].getNumFace() < 0){vertexTab[idPoint3].setNumFace(i);}

        facesTab.append(newFace);


        if ((edgemap.find(std::make_pair(idPoint1,idPoint2)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint2,idPoint1)) == edgemap.end())){
            edgemap[std::make_pair(idPoint1,idPoint2)] = i;
            edgemap[std::make_pair(idPoint2,idPoint1)] = i;
        } else {
          if (edgemap.find(std::make_pair(idPoint1,idPoint2)) != edgemap.end()){
              int autreFace = edgemap[std::make_pair(idPoint1,idPoint2)];
              addAdjacence(autreFace,i, idPoint1, idPoint2);
              edgemap.erase(std::make_pair(idPoint1,idPoint2));
              edgemap.erase(std::make_pair(idPoint2,idPoint1));
          }
          else{
              int autreFace = edgemap[std::make_pair(idPoint2,idPoint1)];
              addAdjacence(autreFace,i, idPoint1, idPoint2);
              edgemap.erase(std::make_pair(idPoint2,idPoint1));
              edgemap.erase(std::make_pair(idPoint1,idPoint2));
          }
        }



        if ((edgemap.find(std::make_pair(idPoint1,idPoint3)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint3,idPoint1)) == edgemap.end())){
            edgemap[std::make_pair(idPoint1,idPoint3)] = i;
            edgemap[std::make_pair(idPoint3,idPoint1)] = i;

        } else {
          if (edgemap.find(std::make_pair(idPoint1,idPoint3)) != edgemap.end()){
              int autreFace = edgemap[std::make_pair(idPoint1,idPoint3)];
              addAdjacence(autreFace,i, idPoint1, idPoint3);
              edgemap.erase(std::make_pair(idPoint1,idPoint3));
              edgemap.erase(std::make_pair(idPoint3,idPoint1));
          }
          else{
              int autreFace = edgemap[std::make_pair(idPoint3,idPoint1)];
              addAdjacence(autreFace,i, idPoint1, idPoint3);
              edgemap.erase(std::make_pair(idPoint3,idPoint1));
              edgemap.erase(std::make_pair(idPoint1,idPoint3));
          }
        }



        if ((edgemap.find(std::make_pair(idPoint3,idPoint2)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint2,idPoint3)) == edgemap.end())){
            edgemap[std::make_pair(idPoint3,idPoint2)] = i;
            edgemap[std::make_pair(idPoint2,idPoint3)] = i;
        } else {
          if (edgemap.find(std::make_pair(idPoint3,idPoint2)) != edgemap.end()){
              int autreFace = edgemap[std::make_pair(idPoint3,idPoint2)];
              addAdjacence(autreFace,i, idPoint3, idPoint2);
              edgemap.erase(std::make_pair(idPoint3,idPoint2));
              edgemap.erase(std::make_pair(idPoint2,idPoint3));
          }
          else{
              int autreFace = edgemap[std::make_pair(idPoint2,idPoint3)];
              addAdjacence(autreFace,i, idPoint3, idPoint2);
              edgemap.erase(std::make_pair(idPoint2,idPoint3));
              edgemap.erase(std::make_pair(idPoint3,idPoint2));
          }
        }
    }

}

void Mesh::testAdjRandom(){
    int ind = rand()%facesTab.length();
    Face face = facesTab[ind];
    int * facesInd = face.getAdjFaces();
    std::cout<<"Face n°"<<ind<<std::endl<<std::endl;

    for (int i = 0; i < 3;i++){
        Face faceToCompare = facesTab[facesInd[i]];
        std::cout<<"La face : "<<facesInd[i]<<std::endl;
        std::cout<<"Adjacent            : "<<face.verifyAdj(faceToCompare)<<std::endl;
        std::cout<<"Adjacent réciproque : "<<faceToCompare.verifyAdj(face)<<std::endl;
        std::cout<<"Bien dans la liste  : "<<faceToCompare.hasAdjFace(face)<<std::endl;
    }
}

void Mesh::addAdjacence(Face& face1, Face& face2, int point1, int point2){
    face1.addAdjFace(face2.getIndice(), point1, point2);
    face2.addAdjFace(face1.getIndice(), point1, point2);
}

void Mesh::addAdjacence(int indFace1, int indFace2, int indPoint1, int indPoint2){
    facesTab[indFace1].addAdjFace(indFace2, indPoint1, indPoint2);
    facesTab[indFace2].addAdjFace(indFace1, indPoint1, indPoint2);
}

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
    Face* firstFace = this->getFacePointeur(circ.getIndFace());
    int sommetPrecedentPlace = ((firstFace->getPlacePoint(circ.getIndPointCentral()))+2)%3;
    int faceOpposee = firstFace->getAdjFaces()[sommetPrecedentPlace];
    circ.setIndFace(faceOpposee);
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

float Mesh::getLocalCurvature(int point){
    float lx = 0;
    float ly = 0;
    float lz = 0;
    double a = 0;
    Vertice* pi = this->getPointPointeur(point);
    for (Circulator_on_faces cf = this->beginCircFaces(point); !(cf == this->endCircFaces(point)); ++cf){
        int iPi = cf->getPlacePoint(point);
        double a = this->cotan(cf.getIndFace(), iPi+2 % 3);
        Vertice* pj = this->getPointPointeur(cf->point((iPi + 1) % 3));
        Face* f1 = this->getFacePointeur(cf->getAdjFaces()[(iPi + 2) % 3]);
        int iPj = f1->getPlacePoint(point);
        double b = this->cotan(f1->getIndice(), iPj-1 % 3);

        a += getSurface(cf.getIndFace())/3.;
        lx += (pj->getPoint()->x() - pi->getPoint()->x())*a*b;
        ly += (pj->getPoint()->y() - pi->getPoint()->y())*a*b;
        lz += (pj->getPoint()->z() - pi->getPoint()->z())*a*b;
    }

    return (std::sqrt(lx*lx + ly*ly + lz*lz)/(2.*a))/2.;
}

void Mesh::splitFace(int indFace, Vertice _point){
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


   //Redéfinition des adjacences + changement de sommet pour if0
    _face0->setPoint(0,indNewPoint);

    // de if0
   _face0->getAdjFaces()[1] = indFacet;
   _face0->getAdjFaces()[2] = indFacet2;

    // de if1
   facesTab[face1].getAdjFaces()[(facesTab[face1].getPlacePoint(sommet0)-1)%3] = indFacet;

    // de if2
   facesTab[face2].getAdjFaces()[(facesTab[face2].getPlacePoint(sommet0)+1)%3] = indFacet2;


   //Redéfinition des faces des points
   vertexTab[sommet0].setNumFace(indFacet2);
   std::cout<<"HEY"<<std::endl;
   vertexTab[indNewPoint].setNumFace(indFace);
   std::cout<<"YEH"<<std::endl;

}

//Example
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

//Example with a wireframe
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

//To put in evidence two faces
void Mesh::drawMeshTwoFaces(int face1, int face2) {



    for(int i = 0; i < facesTab.length(); i+=1) {



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
