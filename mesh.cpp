#include "mesh.h"

Mesh::Mesh()
{


    createFromData("E:\\Centrale\\Pougne\\4A\\MSO - MOS\\C++ etc\\queen.off");

    std::cout<<"Taille vertex : "<<vertexTab.length()<<std::endl;
    std::cout<<"Taille faces : "<<facesTab.length()<<std::endl;
    for (int i = 0; i < 100; i++){
        Face face0 = facesTab[i];
        std::cout<<"Taille    : "<<face0.getAdjFaces().length()<<std::endl;
    }
    //Face face1 = face0.getAdjFaces()[0];
    //std::cout<<"Test : "<<face0.verifyAdj(face1)<<std::endl;


    testAdjRandom();
    testAdjRandom();
    testAdjRandom();
    testAdjRandom();


    /*
    //Tetraedre
    vertexTab.push_back(Point(-0.5,-0.5,-0.5)); //0
    vertexTab.push_back(Point(0.5,-0.5,-0.5)); // 1
    vertexTab.push_back(Point(0,0.5,-0.5)); // 2
    vertexTab.push_back(Point(1,1,1)); // 3

    //Pyramide
    vertexTab.push_back(Point(1,1,1));
    vertexTab.push_back(Point(1,2,1));
    vertexTab.push_back(Point(2,1,1));
    vertexTab.push_back(Point(2,2,1));
    vertexTab.push_back(Point(1.5,1.5,3));


    //Définition du underlyingpoint du tétraedre
    for (int i = 0; i < 4; i++){
        Point point = vertexTab[i];
        point.setNumUnderlyingPoint(0);
    }

    // Celui de la pyramide
    for (int i = 4; i < 9; i++){
        Point point = vertexTab[i];
        point.setNumUnderlyingPoint(4);
    }


    // faces du tetraedre
    Face face1 = Face(vertexTab[0],vertexTab[1],vertexTab[2]);
    Face face2 = Face(vertexTab[1],vertexTab[3],vertexTab[2]);
    Face face3 = Face(vertexTab[3],vertexTab[0],vertexTab[2]);
    Face face4 = Face(vertexTab[0],vertexTab[1],vertexTab[3]);
    facesTab = {face1, face2, face3, face4};

    for (int i = 0; i < facesTab.length(); i++){
        Face face = facesTab[i];
        int taille = facesTab.length();
        QVector<Face> voisins = {facesTab[(i+1) % taille] , facesTab[(i+2) % taille],facesTab[(i+3) % taille]};
    }


    // Faces de la pyramide (triangles)
    Face face5 = Face(vertexTab[4],vertexTab[5],vertexTab[7]);
    facesTab.push_back(face5);
    Face face6 = Face(vertexTab[4],vertexTab[6],vertexTab[7]);
    facesTab.push_back(face6);
    Face face7 = Face(vertexTab[4],vertexTab[5],vertexTab[8]);
    facesTab.push_back(face7);
    Face face8 = Face(vertexTab[4],vertexTab[6],vertexTab[8]);
    facesTab.push_back(face8);
    Face face9 = Face(vertexTab[6],vertexTab[7],vertexTab[8]);
    facesTab.push_back(face9);
    Face face10 = Face(vertexTab[5],vertexTab[7],vertexTab[8]);
    facesTab.push_back(face10);

    // Définiton des faces adjacentes
    face5.setAdjFaces({face6,face7,face10});
    face6.setAdjFaces({face5,face8,face9});
    face7.setAdjFaces({face5,face8,face10});
    face8.setAdjFaces({face6,face7,face9});
    face9.setAdjFaces({face6,face8,face10});
    face10.setAdjFaces({face5,face7,face9});
*/


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

    std::cout<<line<<std::endl;
    std::string substring = line.substr(0,line.find(delimiter));
    nbVertices =  std::atoi(substring.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());

    std::cout<<line<<std::endl;
    substring = line.substr(0,line.find(delimiter));
    nbFaces =  std::atoi(substring.c_str());
    line.erase(0, line.find(delimiter) + delimiter.length());

    std::cout<<line<<std::endl;




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

        Point newPoint = Point(x_i,y_i,z_i);

        if (i % 1000 == 0){
            std::cout<<x_i<<" "<<y_i<<" "<<z_i<<std::endl;
        }
        vertexTab.push_back(newPoint);
    }






    // Les faces
    int idPoint1;
    int idPoint2;
    int idPoint3;

    std::map <std::pair<int,int>, Face*> edgemap;

    for (int i = 0; i < nbFaces; i++){
        getline(thefile,line);

        if (i % 1000 == 0 || i < 10){
            std::cout<<"Face n°"<<i<<std::endl;
            std::cout<<line<<std::endl;
        }

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


        if (i < 10 || i > 96700){
            std::cout<<"3 points : "<<idPoint1<<" "<<idPoint2<<" "<<idPoint3<<std::endl;
        }
        Face newFace = Face(vertexTab[idPoint1],vertexTab[idPoint2],vertexTab[idPoint3]);
        //Face* autreFace;

        if (i % 1000 == 0 && i > 1){
            Face facetest = facesTab[0];
            std::cout<<"Taille :"<<facesTab.length()<<std::endl;
        }



        if (i % 1000 == 0){
            std::cout<<"Taille :"<<facesTab.length()<<std::endl;
        }



        if ((edgemap.find(std::make_pair(idPoint1,idPoint2)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint2,idPoint1)) == edgemap.end())){
            //edgemap.insert((std::make_pair(idPoint1,idPoint2),newFace));
            edgemap[std::make_pair(idPoint1,idPoint2)] = &newFace;
            edgemap[std::make_pair(idPoint2,idPoint1)] = &newFace;
            if (i < 10){
                std::cout<<"Test trouver : "<<(edgemap.find(std::make_pair(idPoint1,idPoint2)) == edgemap.end())<<std::endl;
            }
        } else {
          if (edgemap.find(std::make_pair(idPoint1,idPoint2)) != edgemap.end()){
              //Face* autreFace = edgemap.find(std::make_pair(idPoint1,idPoint2))->second;
              Face* autreFace = edgemap[std::make_pair(idPoint1,idPoint2)];
              (*autreFace).addAdjFaces(newFace);
              newFace.addAdjFaces(*autreFace);
              edgemap.erase(std::make_pair(idPoint1,idPoint2));
              edgemap.erase(std::make_pair(idPoint2,idPoint1));
          }
          else{
              //Face autreFace = edgemap.find(std::make_pair(idPoint1,idPoint2))->second;
              Face* autreFace = edgemap[std::make_pair(idPoint2,idPoint1)];
              (*autreFace).addAdjFaces(newFace);
              newFace.addAdjFaces(*autreFace);
              edgemap.erase(std::make_pair(idPoint2,idPoint1));
              edgemap.erase(std::make_pair(idPoint1,idPoint2));
          }
        }



        if ((edgemap.find(std::make_pair(idPoint1,idPoint3)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint3,idPoint1)) == edgemap.end())){
            //edgemap.insert(std::make_pair(std::make_pair(idPoint1,idPoint2),newFace));
            edgemap[std::make_pair(idPoint1,idPoint3)] = &newFace;
            edgemap[std::make_pair(idPoint3,idPoint1)] = &newFace;

        } else {
          if (edgemap.find(std::make_pair(idPoint1,idPoint3)) != edgemap.end()){
              //Face autreFace = edgemap.find(std::make_pair(idPoint1,idPoint2))->second;
              Face* autreFace = edgemap[std::make_pair(idPoint1,idPoint3)];
              (*autreFace).addAdjFaces(newFace);
              newFace.addAdjFaces(*autreFace);
              edgemap.erase(std::make_pair(idPoint1,idPoint3));
              edgemap.erase(std::make_pair(idPoint3,idPoint1));
          }
          else{
              //Face autreFace = edgemap.find(std::make_pair(idPoint1,idPoint2))->second;
              Face* autreFace = edgemap[std::make_pair(idPoint3,idPoint1)];
              (*autreFace).addAdjFaces(newFace);
              newFace.addAdjFaces(*autreFace);
              edgemap.erase(std::make_pair(idPoint3,idPoint1));
              edgemap.erase(std::make_pair(idPoint1,idPoint3));
          }
        }



        if ((edgemap.find(std::make_pair(idPoint3,idPoint2)) == edgemap.end()) &&
                (edgemap.find(std::make_pair(idPoint2,idPoint3)) == edgemap.end())){
            //edgemap.insert(std::make_pair(std::make_pair(idPoint1,idPoint2),newFace));
            edgemap[std::make_pair(idPoint3,idPoint2)] = &newFace;
            edgemap[std::make_pair(idPoint2,idPoint3)] = &newFace;
        } else {
          if (edgemap.find(std::make_pair(idPoint3,idPoint2)) != edgemap.end()){
              //Face autreFace = edgemap.find(std::make_pair(idPoint1,idPoint2))->second;
              Face* autreFace = edgemap[std::make_pair(idPoint3,idPoint2)];
              (*autreFace).addAdjFaces(newFace);
              newFace.addAdjFaces(*autreFace);
              edgemap.erase(std::make_pair(idPoint3,idPoint2));
              edgemap.erase(std::make_pair(idPoint2,idPoint3));
          }
          else{
              //Face autreFace = edgemap.find(std::make_pair(idPoint1,idPoint2))->second;
              Face* autreFace = edgemap[std::make_pair(idPoint2,idPoint3)];
              (*autreFace).addAdjFaces(newFace);
              newFace.addAdjFaces(*autreFace);
              edgemap.erase(std::make_pair(idPoint2,idPoint3));
              edgemap.erase(std::make_pair(idPoint3,idPoint2));
          }
        }


        facesTab.append(newFace);


        if (i % 1000 == 0){
            std::cout<<"Taille :"<<facesTab.length()<<std::endl;
        }

        if (i >= 96700){
            std::cout<<"Taille :"<<facesTab.length()<<std::endl;
            Face facetest = facesTab[0];
        }

    }


/*
    for (int i = 0; i < nbFaces; i++){
        Face face = facesTab[i];
        for (int j=0; j<face.getAdjFaces().length();j++){
            Face adjFace = face.getAdjFaces()[j];
            if (! adjFace.hasAdjFace(face)){
                std::cout<<"Ca va pas pour "<<i<<", taille = "<<face.getAdjFaces().length()<<", "<<adjFace.getAdjFaces().length()<<std::endl;
                adjFace.addAdjFaces(face);
                std::cout<<"Après ajout : "<<adjFace.getAdjFaces().length()<<std::endl;
            }
        }
    }*/
std::cout<<"Taille d'un d'eux : "<<facesTab[24].getAdjFaces().length()<<std::endl;
std::cout<<"Taille finale :"<<facesTab.length()<<std::endl;
}



void Mesh::testAdjRandom(){
    int ind = rand()%facesTab.length();
    Face face = facesTab[ind];
    QVector<Face> faces = face.getAdjFaces();
    std::cout<<"Face n°"<<ind<<std::endl<<std::endl;

    for (int i = 0; i < faces.length();i++){
        Face faceToCompare = faces[i];
        std::cout<<"Adjacent            : "<<face.verifyAdj(faceToCompare)<<std::endl;
        std::cout<<"Adjacent réciproque : "<<faceToCompare.verifyAdj(face)<<std::endl;
        std::cout<<"Bien dans la liste  : "<<faceToCompare.hasAdjFace(face)<<std::endl;
    }
}



void Mesh::addAdjacence(Face& face1, Face& face2){
    face1.addAdjFaces(face2);
    face2.addAdjFaces(face1);
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
        glVertexDraw(face.point(1));
        glVertexDraw(face.point(2));
        glVertexDraw(face.point(3));
        glEnd();
    }


}

//Example with a wireframe tedraedra
void Mesh::drawMeshWireFrame() {
    for(int i = 0; i < facesTab.length(); i+=1) {
        Face face = facesTab[i];
        glBegin(GL_LINE_STRIP);
            glVertexDraw(face.point(1));
            glVertexDraw(face.point(2));
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(face.point(2));
            glVertexDraw(face.point(3));
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(face.point(3));
            glVertexDraw(face.point(1));
        glEnd();
    }

}



