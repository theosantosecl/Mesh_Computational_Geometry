#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <iostream>
#include <fstream>
#include <cmath>



//Iterateurs
class Iterator_on_faces;
class Iterator_on_vertices;
class Circulator_on_faces;
class Circulator_on_vertices;

class Point
{
    double _x;
    double _y;
    double _z;

public:
    Point(): _x(), _y(), _z() {}
    Point(double x_, double y_, double z_): _x(x_), _y(y_), _z(z_) {}

    double x() const {return _x; }
    double y() const {return _y; }
    double z() const {return _z; }

    //Effectue un produit scalaire
    double dot(Point point_){
        return _x*point_.x() + _x*point_.y() + _z*point_.z();
    }

    //Effectue un produit vectoriel
    Point vec(Point point_) {
        double x = _y*point_.z() - _z*point_.y();
        double y = _z*point_.x() - _x*point_.z();
        double z = _x*point_.y() - _y*point_.x();
        return Point(x, y, z);
    }

    //Norme du vecteur
    double norm() {
        return std::sqrt(_x*_x + _y*_y + _z*_z);
    }

    Point operator-(Point point_){
        return Point(_x - point_.x(), _y - point_.y(), _z - point_.z());
    }
};

class Vertice
{
    Point _point; //Point correspondant
    int _numFace; //Indice dans la liste des faces de la face de référence
    int _indice; //Indice dans la liste des vertex du Mesh

public:
    Vertice():_point(), _numFace(-1) {}
    Vertice(double x_, double y_, double z_): _point(x_, y_, z_), _numFace(-1) {}

    Vertice(float x_, float y_, float z_, int indice): _point(x_, y_, z_), _numFace(-1), _indice(indice) {}

    // get
    Point* getPoint() { return &_point; }


    int getIndice(){return _indice;}
    void setIndice(int indice){_indice = indice;}
    int getNumFace() {return _numFace;}
    void setNumFace(int numFace){_numFace = numFace;}

    Point operator-(Vertice vertice_){
        return _point - *vertice_.getPoint();
    }
};

class Face
{

    int _vertices[3];
    int _indice;

    //Indices de faces
    int _adjFaces[3];

public:
    Face(): _vertices() {}
    Face(int point1_, int point2_, int point3_): _vertices()  {
        _vertices[0] = point1_;
        _vertices[1] = point2_;
        _vertices[2] = point3_;
    }
    Face(int point1_, int point2_, int point3_, int indice): _vertices(), _indice(indice){
        _vertices[0] = point1_;
        _vertices[1] = point2_;
        _vertices[2] = point3_;
    }

    //get
    int point(int i) const {
        return _vertices[i];
    }
    //set
    void setPoint(int i, int indice){
        _vertices[i] = indice;
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

    //Vérification de l'appartenance à la liste d'adjacence
    bool hasAdjFace(Face potentialAdjFace){
        return hasAdjFace(potentialAdjFace.getIndice());
    }

    bool verifyAdj(Face face2){
        QVector<int> points1 = {_vertices[0], _vertices[1], _vertices[2]};
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

    //Donne le dernier sommet du
    int getDernierPoint(int ind1, int ind2){
        int indiceDernierPoint = 0;
        for (int i = 0; i < 3; i++){
            if (_vertices[i] != ind1 && _vertices[i] != ind2){
                indiceDernierPoint = _vertices[i];
            }
        }
        return indiceDernierPoint;
    }

    // Donne le placement dans le tableau des points du dernier point
    int getPlaceDernierPoint(int ind1, int ind2){
        int placeDernierPoint = 0;
        for (int i = 0; i < 3; i++){
            if (_vertices[i] != ind1 && _vertices[i] != ind2){
                placeDernierPoint = i;
            }
        }
        return placeDernierPoint;
    }

    // Donne le placement dans le tableau du points entré (identifié par l'indice dans vertexTab)
    int getPlacePoint(int indPoint){
        int placePoint = -1;
        for (int i = 0; i < 3; i++){
            if (_vertices[i] == indPoint){
                placePoint = i;
            }
        }
        return placePoint;
    }

    //Ajout d'une adjacence à la bonne place
    void addAdjFace(int adjFace, int point1, int point2){
        int place = getPlaceDernierPoint(point1, point2);
        _adjFaces[place] = adjFace;
    }

    void splitFace(int indNewPoint);
};




class Mesh
{
    QVector<Vertice> vertexTab;
    QVector<Face> facesTab;
    Vertice underlyingPoint;


public:
    Mesh();

    //Méthode draw
    void drawMesh();
    void drawMeshWireFrame();
    void drawMeshPoints();
    void drawMeshTwoFaces(int face1, int face2);

    // get
    Face getFace(Vertice point); //Récupère la face de référence du sommet
    Face getFace(int indice){return facesTab[indice];}

    Face* getFacePointeur(int indice){return &(facesTab[indice]);} //Passage de l'indice au pointeur
    Vertice* getPointPointeur(int indice){ return &(vertexTab[indice]);} //Passage de l'indice au pointeur

    Vertice getUnderlyingPoint();

    int getNbFaces(){ return facesTab.length();}
    int getNbVertices(){ return vertexTab.length();}

    //Importation à partir d'un fichier .off
    void createFromDatat(std::string path);
    void createFromData(std::string path);

    //Ajout d'adjacences entre faces
    void addAdjacence(int face1, int face2, int point1, int point2);
    void addAdjacence(Face& face1, Face& face2, int point1, int point2);

    //Test randomisé d'adjacence pour vérifier que les faces ont bien été attachées
    void testAdjRandom();



    //Méthodes pour les itérateurs
    Iterator_on_faces beginFaces();
    Iterator_on_faces endFaces();
    Iterator_on_vertices beginVertices();
    Iterator_on_vertices endVertices();
    Circulator_on_faces beginCircFaces(int point);
    Circulator_on_faces endCircFaces(int point);
    Circulator_on_vertices beginCircVertices(int point);
    Circulator_on_vertices endCircVertices(int point);


    //Méthodes de split
    void splitFace(int indFace, Vertice _point);
    //Version avec uniquement les coordonnées
    void splitFace(int indFace, double x, double y, double z){
        Vertice _vertice = Vertice(x,y,z);
        splitFace(indFace, _vertice);
    }
    //Version sans point de split : on prend comme point de split le barycentre
    void splitFace(int indFace){
        Face face = facesTab[indFace];
        double  x = (vertexTab[face.point(0)].getPoint()->x() + vertexTab[face.point(1)].getPoint()->x() + vertexTab[face.point(2)].getPoint()->x())/3;
        double  y = (vertexTab[face.point(0)].getPoint()->y() + vertexTab[face.point(1)].getPoint()->y() + vertexTab[face.point(2)].getPoint()->y())/3;
        double  z = (vertexTab[face.point(0)].getPoint()->z() + vertexTab[face.point(1)].getPoint()->z() + vertexTab[face.point(2)].getPoint()->z())/3;
        splitFace(indFace, x, y, z);
    }

    //Méthode de flip
    void flip(int indF0, int indF1);



    //Méthodes pour le calcul de la courbure
    //Cotangente
    double cotan(int face_, int point_){
        Point u = vertexTab[facesTab[face_].point((point_ + 2) % 3)] - vertexTab[facesTab[face_].point((point_) % 3)];
        Point v = vertexTab[facesTab[face_].point((point_ + 1) % 3)] - vertexTab[facesTab[face_].point((point_) % 3)];
        return u.dot(v) / (u.vec(v)).norm();
    }

    //Surface d'une face
    double getSurface(int indFace) {
        Face f = facesTab[indFace];
        double det1 = vertexTab[f.point(0)].getPoint()->x() * ( vertexTab[f.point(1)].getPoint()->y() - vertexTab[f.point(2)].getPoint()->y() ) -
                vertexTab[f.point(1)].getPoint()->x() * ( vertexTab[f.point(0)].getPoint()->y() - vertexTab[f.point(2)].getPoint()->y() ) +
                vertexTab[f.point(2)].getPoint()->x() * ( vertexTab[f.point(0)].getPoint()->y() - vertexTab[f.point(1)].getPoint()->y() );
        double det2 = vertexTab[f.point(0)].getPoint()->y() * ( vertexTab[f.point(1)].getPoint()->z() - vertexTab[f.point(2)].getPoint()->z() ) -
                vertexTab[f.point(1)].getPoint()->y() * ( vertexTab[f.point(0)].getPoint()->z() - vertexTab[f.point(2)].getPoint()->z() ) +
                vertexTab[f.point(2)].getPoint()->y() * ( vertexTab[f.point(0)].getPoint()->z() - vertexTab[f.point(1)].getPoint()->z() );
        double det3 = vertexTab[f.point(0)].getPoint()->z() * ( vertexTab[f.point(1)].getPoint()->x() - vertexTab[f.point(2)].getPoint()->x() ) -
                vertexTab[f.point(1)].getPoint()->z() * ( vertexTab[f.point(0)].getPoint()->x() - vertexTab[f.point(2)].getPoint()->x() ) +
                vertexTab[f.point(2)].getPoint()->z() * ( vertexTab[f.point(0)].getPoint()->x() - vertexTab[f.point(1)].getPoint()->x() );
        return std::sqrt(det1*det1 + det2*det2 + det3*det3)/2.;
    }

    //Calcul de la courbure autour du sommet
    double getLocalCurvature(int point);
};


//Itérateurs et circulateurs

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

    Vertice& operator*(){
        return *(_mesh->getPointPointeur(_indice));
    }

    Vertice* operator->(){
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


class Circulator_on_faces{
    int _indFace;
    int _indPointCentral;
    Mesh* _mesh;

public:

    Circulator_on_faces(Mesh* mesh, int indPoint):  _indPointCentral(indPoint), _mesh(mesh){
        _indFace = (_mesh->getPointPointeur(_indPointCentral))->getNumFace();
    }

    void operator++(){
        Face* currentFace = _mesh->getFacePointeur(_indFace);
        int sommetSuivantPlace = ((currentFace->getPlacePoint(_indPointCentral))+1)%3;
        int faceOpposee = currentFace->getAdjFaces()[sommetSuivantPlace];
        _indFace = faceOpposee;
    }


    Face& operator*(){
        return *(_mesh->getFacePointeur(_indFace));
    }

    Face* operator->(){
        return _mesh->getFacePointeur(_indFace);
    }


    bool operator==(Circulator_on_faces circ2){
        return (_indFace == circ2.getIndFace() && _indPointCentral == circ2.getIndPointCentral());
    }

    int getIndFace(){return _indFace;}
    void setIndFace(int indFace){_indFace = indFace;}
    int getIndPointCentral(){return _indPointCentral;}
    void setIndPointCentral(int indPointCentral){_indPointCentral = indPointCentral;}


};


class Circulator_on_vertices{
    Circulator_on_faces circFace;
    int _indPointCentral;
    int _indPoint;
    Mesh* _mesh;

public:
    Circulator_on_vertices(Mesh* mesh, int indPointCentral): circFace(mesh, indPointCentral),  _indPointCentral(indPointCentral), _mesh(mesh){
        _indPoint = circFace->point( (circFace->getPlacePoint(_indPointCentral)+2)%3 );
    }

    void operator++(){
        ++circFace;
        _indPoint = circFace->point( (circFace->getPlacePoint(_indPointCentral)+2)%3 );
    }


    Vertice& operator*(){
        return *(_mesh->getPointPointeur(_indPoint));
    }

    Vertice* operator->(){
        return _mesh->getPointPointeur(_indPoint);
    }



    bool operator==(Circulator_on_vertices circ2){
        return (_indPoint == circ2.getPoint());
                //&& circFace == circ2.getCircFace()); Attention, s'il faut prendre en compte ça, ça change la méthode end dans lequel il faut modifier également circFace
    }


    int getPoint(){return _indPoint;}
    void setPoint(int point){_indPoint = point;}
    int getIndPointCentral(){return _indPointCentral;}

    Circulator_on_faces getCircFace(){
        return circFace;
    }

};


#endif // MESH_H
