#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <iostream>
#include <fstream>

// Advice => You should create a new file for each module but not necessarily for each class
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

    int sommet1;
    int sommet2;
    int sommet3;

    int _indice;

    //Indices de faces
    QVector<int> _adjFaces;
    //TODO : changer en tableau à la place de QVector

public:
    Face(): sommet1(), sommet2(), sommet3() {}
    Face(int point1_, int point2_, int point3_): sommet1(point1_), sommet2(point2_), sommet3(point3_)  {}
    Face(int point1_, int point2_, int point3_, int indice): sommet1(point1_), sommet2(point2_), sommet3(point3_), _indice(indice){}

    //get
    int point(int i) const {
        if (i == 1){
            return sommet1;
        }
        if (i == 2){
            return sommet2;
        }
        if (i == 3){
            return sommet3;
        }
        else {
            return sommet1;
        }
    }

    QVector<int> getAdjFaces() {
        return _adjFaces;
    }
    void setAdjFaces(QVector<int> adjFaces){
       _adjFaces = adjFaces;
    }

    void addAdjFaces(int newFace){
        _adjFaces.append(newFace);
    }

    int getIndice(){return _indice;}
    void setIndice(int indice){_indice = indice;}

    bool hasAdjFace(int potentialAdjFace){
        bool rep = false;
        for (int i = 0; i < _adjFaces.length(); i++){
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
        QVector<int> points1 = {sommet1, sommet2, sommet3};
        QVector<int> points2 = {face2.point(1), face2.point(2), face2.point(3)};
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
};

//** TO MODIFY
class Mesh
{
    QVector<Point> vertexTab;
    QVector<Face> facesTab;
    Point underlyingPoint;
//int faces[12]={
//        0,1,2, // 1
//        1,3,2, // 2
//        3,0,2, // 3
//        0,1,3  // 4
//    }; // To be replaced by a vector of faces
public:
    Mesh();

    void drawMesh();
    void drawMeshWireFrame();
    Face getFace(Point point);
    Point getUnderlyingPoint();
    void createFromData(std::string path);
    void addAdjacence(int face1, int face2);
    void addAdjacence(Face& face1, Face& face2);
    void testAdjRandom();

};

#endif // MESH_H
