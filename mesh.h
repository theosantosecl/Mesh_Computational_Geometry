#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <iostream>
#include <fstream>



//Iterateurs
class Iterator_on_faces;
class Iterator_on_vertices;

class Point
{
    double _x;
    double _y;
    double _z;
    int _numFace;
    int _indice;

public:
    Point():_x(),_y(),_z() {}
    Point(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}

    Point(float x_, float y_, float z_, int indice):_x(x_),_y(y_),_z(z_), _indice(indice) {}

    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }


    int getIndice(){return _indice;}
    void setIndice(int indice){_indice = indice;}
    int getNumFace() {return _numFace;}
    void setNumFace(int numFace){_numFace = numFace;}
};

class Face
{

    int _sommets[3];
    int _indice;

    //Indices de faces
    int _adjFaces[3];
    //TODO : changer en tableau à la place de QVector

public:
    Face(): _sommets() {}
    Face(int point1_, int point2_, int point3_): _sommets()  {
        _sommets[0] = point1_;
        _sommets[1] = point2_;
        _sommets[2] = point3_;
    }
    Face(int point1_, int point2_, int point3_, int indice): _sommets(), _indice(indice){
        _sommets[0] = point1_;
        _sommets[1] = point2_;
        _sommets[2] = point3_;
    }

    //get
    int point(int i) const {
        return _sommets[i];
    }

    int * getAdjFaces() {
        return _adjFaces;
    }
    void setAdjFaces(int adjFaces[3]){
        for (int i =0; i < 3; i++){
            _adjFaces[i] = adjFaces[i];
        }
    }

    int getIndice(){return _indice;}
    void setIndice(int indice){_indice = indice;}

    bool hasAdjFace(int potentialAdjFace){
        bool rep = false;
        for (int i = 0; i < 3; i++){
            if (_adjFaces[i] == potentialAdjFace){
                rep = true;
            }
        }
        return rep;
    }


    bool hasAdjFace(Face potentialAdjFace){
        return hasAdjFace(potentialAdjFace.getIndice());
    }

    bool verifyAdj(Face face2){
        QVector<int> points1 = {_sommets[0], _sommets[1], _sommets[2]};
        QVector<int> points2 = {face2.point(0), face2.point(1), face2.point(2)};
        bool aumoinsun = false;
        bool aumoinsdeux = false;
        int i = 0;
        while ( !aumoinsdeux && i<3){
            int j = 0;
            while (!aumoinsdeux && j<3){
                if (points1[i] == points2[j]){
                    if (aumoinsun) aumoinsdeux = true;
                    else aumoinsun = true;
                }
                j++;
            }
            i++;
        }
        return aumoinsdeux;
    }


    int getDernierPoint(int ind1, int ind2){
        int indiceDernierPoint = 0;
        for (int i = 0; i < 3; i++){
            if (_sommets[i] != ind1 && _sommets[i] != ind2){
                indiceDernierPoint = _sommets[i];
            }
        }
        return indiceDernierPoint;
    }

    // Donne le placement dans le tableau des points du dernier point
    int getPlaceDernierPoint(int ind1, int ind2){
        int placeDernierPoint = 0;
        for (int i = 0; i < 3; i++){
            if (_sommets[i] != ind1 && _sommets[i] != ind2){
                placeDernierPoint = i;
            }
        }
        return placeDernierPoint;
    }

    // Donne le placement dans le tableau du points entré (identifié par l'indice dans vertexTab)
    int getPlacePoint(int indPoint){
        int placePoint = -1;
        for (int i = 0; i < 3; i++){
            if (_sommets[i] == indPoint){
                placePoint = i;
            }
        }
        return placePoint;
    }

    void addAdjFace(int adjFace, int point1, int point2){
        int place = getPlaceDernierPoint(point1, point2);
        _adjFaces[place] = adjFace;
    }

};




class Mesh
{
    QVector<Point> vertexTab;
    QVector<Face> facesTab;
    Point underlyingPoint;


public:
    Mesh();

    void drawMesh();
    void drawMeshWireFrame();
    Face getFace(Point point); // INUTILE POUR LE MOMENT
    Face getFace(int indice){return facesTab[indice];}
    Face* getFacePointeur(int indice){return &(facesTab[indice]);}
    Point* getPointPointeur(int indice){ return &(vertexTab[indice]);}
    Point getUnderlyingPoint();
    void createFromData(std::string path);
    void addAdjacence(int face1, int face2, int point1, int point2);
    void addAdjacence(Face& face1, Face& face2, int point1, int point2);
    void testAdjRandom();
    int getNbFaces(){ return facesTab.length();}
    int getNbVertices(){ return vertexTab.length();}

    Iterator_on_faces beginFaces();
    Iterator_on_faces endFaces();
    Iterator_on_vertices beginVertices();
    Iterator_on_vertices endVertices();



};

class Iterator_on_faces{
    int _indice;
    Mesh* _mesh;

public:
    Iterator_on_faces(Mesh* mesh_): _mesh(mesh_){
        _indice = 0;
    }

    void operator++(){
        _indice++;
    }

    Face& operator*(){
        return *(_mesh->getFacePointeur(_indice));
    }

    Face* operator->(){
        return _mesh->getFacePointeur(_indice);
    }

    bool operator==(Iterator_on_faces it2){
        return (_indice == it2.getIndice());
    }



    void setIndice(int ind){
        _indice = ind;
    }

    int getIndice(){
        return _indice;
    }

};


class Iterator_on_vertices{
    int _indice;
    Mesh* _mesh;

public:
    Iterator_on_vertices(Mesh* mesh_): _mesh(mesh_){
        _indice = 0;
    }

    void operator++(){
        _indice++;
    }

    Point& operator*(){
        return *(_mesh->getPointPointeur(_indice));
    }

    Point* operator->(){
        return _mesh->getPointPointeur(_indice);
    }

    bool operator==(Iterator_on_vertices it2){
        return (_indice == it2.getIndice());
    }



    void setIndice(int ind){
        _indice = ind;
    }

    int getIndice(){
        return _indice;
    }

};

#endif // MESH_H
