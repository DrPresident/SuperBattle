#ifndef OBJECTS_H
#define OBJECTS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class object{
public:
    int dim[4];
    ALLEGRO_BITMAP *bitmap;
};


class rigidbody : public object{
public:
    bool grounded;
    int dx, dy;
};

class terrain : public object{
public:
    terrain(int,int,string);
};

class player : public rigidbody{
public:
    player(int,int,int,int,string);
};

class enemy : public rigidbody{
public:
    enemy(int,int,string);
};

#endif // OBJECTS_H
