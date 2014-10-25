#include "objects.h"

using namespace std;

enum  DIMENSIONS{X,Y,width,height};

player::player(int posX, int posY, int width, int height, string imagePath){

    dim[X]      = posX;
    dim[Y]      = posY;
    dim[width]  = width;
    dim[height] = height;
    bitmap      = al_load_bitmap(imagePath.c_str());
    grounded    = true;
}

enemy::enemy(int posX, int posY, string imagePath){

    dim[X] = posX;
    dim[Y] = posY;
    bitmap = al_load_bitmap(imagePath.c_str());

}

terrain::terrain(int posX, int posY, string imagePath){

    dim[X] = posX;
    dim[Y] = posY;
    bitmap = al_load_bitmap(imagePath.c_str());


}
