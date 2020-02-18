#include "mesh.h"

Mesh::Mesh()
{


    createFromData("/home/vault/Code/m2_ds/Mesh_Computational_Geometry/queen.off");

    std::cout<<"Taille vertex : "<<vertexTab.length()<<std::endl;
    std::cout<<"Taille faces : "<<facesTab.length()<<std::endl;
}



// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(const Point & p) {
    glVertex3f(p.x(), p.y(), p.z());
}


// Get the face of a point
Face Mesh::getFace(Point point){
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



    //Première ligne
    int nbVertices;
    int nbFaces;

    std::string substring = line.substr(0,line.find(delimiter));
    nbVertices =  std::atoi(substring.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());

    substring = line.substr(0,line.find(delimiter));
    nbFaces =  std::atoi(substring.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());


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

        Point newPoint = Point(x_i,y_i,z_i, i);

        vertexTab.push_back(newPoint);
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

double pScal(float u[3], float v[3]){
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

double pVecNorm(float u[3], float v[3]){
    float a = u[1]*v[2] - u[2]*v[1];
    float b = u[2]*v[0] - u[0]*v[2];
    float c = u[0]*v[1] - u[1]*v[0];
    return a*a + b*b + c*c;
}

double cotan(float u[3], float v[3]){
    return pScal(u, v)/pVecNorm(u, v);
}

float Mesh::getLocalCurvature(int point){
    float lx = 0;
    float ly = 0;
    float lz = 0;
    Point* pi = this->getPointPointeur(point);
    for (Circulator_on_faces cf = this->beginCircFaces(point); !(cf == this->endCircFaces(point)); ++cf){
        int iPi = cf->getPlacePoint(point);
        Point* pj = this->getPointPointeur(cf->point((iPi + 1) % 3));
        Face* f1 = this->getFacePointeur(cf->getAdjFaces()[(iPi + 2) % 3]);
        int iPi2 = f1->getPlacePoint(point);
        float x = pi->x() - this->getPointPointeur(cf->point((iPi + 2) % 3))->x();
        float y = pi->y() - this->getPointPointeur(cf->point((iPi + 2) % 3))->y();
        float z = pi->z() - this->getPointPointeur(cf->point((iPi + 2) % 3))->z();
        float v10[3] = { x, y, z};
        x = this->getPointPointeur(cf->point((iPi + 1) % 3))->x() - this->getPointPointeur(cf->point((iPi + 2) % 3))->x();
        y = this->getPointPointeur(cf->point((iPi + 1) % 3))->y() - this->getPointPointeur(cf->point((iPi + 2) % 3))->y();
        z = this->getPointPointeur(cf->point((iPi + 1) % 3))->z() - this->getPointPointeur(cf->point((iPi + 2) % 3))->z();
        float v11[3] = { x, y, z};
        x = pi->x() - this->getPointPointeur(f1->point((iPi2 + 1) % 3))->x();
        y = pi->y() - this->getPointPointeur(f1->point((iPi2 + 1) % 3))->y();
        z = pi->z() - this->getPointPointeur(f1->point((iPi2 + 1) % 3))->z();;
        float v20[3] = { x, y, z};
        x = this->getPointPointeur(f1->point((iPi2 + 2) % 3))->x() - this->getPointPointeur(f1->point((iPi2 + 1) % 3))->x();
        y = this->getPointPointeur(f1->point((iPi2 + 2) % 3))->y() - this->getPointPointeur(f1->point((iPi2 + 1) % 3))->y();
        z = this->getPointPointeur(f1->point((iPi2 + 2) % 3))->z() - this->getPointPointeur(f1->point((iPi2 + 1) % 3))->z();
        float v21[3] = { x, y, z};

        lx += (pj->x() - pi->x())*cotan(v10, v11)*cotan(v20, v21);
        ly += (pj->y() - pi->y())*cotan(v10, v11)*cotan(v20, v21);
        lz += (pj->z() - pi->z())*cotan(v10, v11)*cotan(v20, v21);
    }

    return (lx*lx + ly*ly + lz*lz)/2.;
}

//Example with a tetraedra
void Mesh::drawMesh() {
    for(int i = 0; i < facesTab.length(); i+=1) {

        if (i%3 == 0) glColor3d(1,0,0);
        else if (i%3 == 1) glColor3d(0,1,0);
        else if (i%3 == 2) glColor3d(0,0,1);
        else glColor3d(1,1,0);

        Face face = facesTab[i];
        glBegin(GL_TRIANGLES);
            glVertexDraw(vertexTab[face.point(0)]);
            glVertexDraw(vertexTab[face.point(1)]);
            glVertexDraw(vertexTab[face.point(2)]);
        glEnd();
    }


}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    for(int i = 0; i < facesTab.length(); i+=1) {
        Face face = facesTab[i];
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[face.point(0)]);
            glVertexDraw(vertexTab[face.point(1)]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[face.point(1)]);
            glVertexDraw(vertexTab[face.point(2)]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[face.point(2)]);
            glVertexDraw(vertexTab[face.point(0)]);
        glEnd();
    }

}



