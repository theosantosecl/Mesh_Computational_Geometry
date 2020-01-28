#include "mesh.h"

Mesh::Mesh()
{


    createFromData("E:\\Centrale\\Pougne\\4A\\MSO - MOS\\C++ etc\\queen.off");

    std::cout<<"Taille vertex : "<<vertexTab.length()<<std::endl;
    std::cout<<"Taille faces : "<<facesTab.length()<<std::endl;


    for (int i = 0; i < 10; i++){
        std::cout<< "Nombres voisins de "<<i<<" : "<<facesTab[i].getAdjFaces().length()<<std::endl;
    }


    for (int i = 96700; i < 96713; i++){
        std::cout<<"Nombres voisins de "<<i<<" : "<< facesTab[i].getAdjFaces().length()<<std::endl;
    }

    testAdjRandom();
    testAdjRandom();
    testAdjRandom();
    testAdjRandom();

    for (Iterator_on_vertices it = this->beginVertices(); !(it == this->endVertices()); ++it){
        std::cout<<it.getIndice()<<" "<<(*it).getIndice()<<std::endl;
    }




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

        facesTab.append(newFace);


        if ((edgemap.find(std::make_pair(idPoint1,idPoint2)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint2,idPoint1)) == edgemap.end())){
            edgemap[std::make_pair(idPoint1,idPoint2)] = i;
            edgemap[std::make_pair(idPoint2,idPoint1)] = i;
        } else {
          if (edgemap.find(std::make_pair(idPoint1,idPoint2)) != edgemap.end()){
              int autreFace = edgemap[std::make_pair(idPoint1,idPoint2)];
              addAdjacence(autreFace,i);
              edgemap.erase(std::make_pair(idPoint1,idPoint2));
              edgemap.erase(std::make_pair(idPoint2,idPoint1));
          }
          else{
              int autreFace = edgemap[std::make_pair(idPoint2,idPoint1)];
              addAdjacence(autreFace,i);
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
              addAdjacence(autreFace,i);
              edgemap.erase(std::make_pair(idPoint1,idPoint3));
              edgemap.erase(std::make_pair(idPoint3,idPoint1));
          }
          else{
              int autreFace = edgemap[std::make_pair(idPoint3,idPoint1)];
              addAdjacence(autreFace,i);
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
              addAdjacence(autreFace,i);
              edgemap.erase(std::make_pair(idPoint3,idPoint2));
              edgemap.erase(std::make_pair(idPoint2,idPoint3));
          }
          else{
              int autreFace = edgemap[std::make_pair(idPoint2,idPoint3)];
              addAdjacence(autreFace,i);
              edgemap.erase(std::make_pair(idPoint2,idPoint3));
              edgemap.erase(std::make_pair(idPoint3,idPoint2));
          }
        }





    }

}



void Mesh::testAdjRandom(){
    int ind = rand()%facesTab.length();
    Face face = facesTab[ind];
    QVector<int> facesInd = face.getAdjFaces();
    std::cout<<"Face n°"<<ind<<std::endl<<std::endl;

    for (int i = 0; i < facesInd.length();i++){
        Face faceToCompare = facesTab[facesInd[i]];
        std::cout<<"Adjacent            : "<<face.verifyAdj(faceToCompare)<<std::endl;
        std::cout<<"Adjacent réciproque : "<<faceToCompare.verifyAdj(face)<<std::endl;
        std::cout<<"Bien dans la liste  : "<<faceToCompare.hasAdjFace(face)<<std::endl;
    }
}



void Mesh::addAdjacence(Face& face1, Face& face2){
    face1.addAdjFaces(face2.getIndice());
    face2.addAdjFaces(face1.getIndice());
}

void Mesh::addAdjacence(int indFace1, int indFace2){
    facesTab[indFace1].addAdjFaces(indFace2);
    facesTab[indFace2].addAdjFaces(indFace1);
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


//Example with a tetraedra
void Mesh::drawMesh() {
    for(int i = 0; i < facesTab.length(); i+=1) {

        if (i%3 == 0) glColor3d(1,0,0);
        else if (i%3 == 1) glColor3d(0,1,0);
        else if (i%3 == 2) glColor3d(0,0,1);
        else glColor3d(1,1,0);

        Face face = facesTab[i];
        glBegin(GL_TRIANGLES);
        glVertexDraw(vertexTab[face.point(1)]);
        glVertexDraw(vertexTab[face.point(2)]);
        glVertexDraw(vertexTab[face.point(3)]);
        glEnd();
    }


}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    for(int i = 0; i < facesTab.length(); i+=1) {
        Face face = facesTab[i];
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[face.point(1)]);
            glVertexDraw(vertexTab[face.point(2)]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[face.point(2)]);
            glVertexDraw(vertexTab[face.point(3)]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[face.point(3)]);
            glVertexDraw(vertexTab[face.point(1)]);
        glEnd();
    }

}



