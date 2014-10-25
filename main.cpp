#include "objects.h"
#include <iostream>
using namespace std;

const int FPS  = 60;
const int GRAVITY = 2;
int   SCREEN_W   = 1280, SCREEN_H     = 960,
      error      = 0,    spawnTime    = 1,
      spawnCount = 0,    spawnID      = 100,
      coords[2] = {0,0};

int   speed      = 5,    jump         = 10;

bool  redraw     = true, JACKSAWESOME = true,
      canJump    = true,
      key[5]     = {false,false,false,false,false};

enum  KEYS{W,A,S,D,SPACE};
enum  DIMENSIONS{X,Y,width,height};

    ALLEGRO_DISPLAY *mainDisplay   = NULL;
    ALLEGRO_TIMER
        *timer                     = NULL,
        *enemyTimer                = NULL;
    ALLEGRO_EVENT_QUEUE *eventQ    = NULL;
    ALLEGRO_BITMAP
    //TERRAIN
        *Background                = NULL,
        *Dirt                      = NULL,
        *stoneWall                 = NULL,
        *treeTrunk                 = NULL,
        *treeTop                   = NULL,
        *treeBranch                = NULL,

        *playerSkin                = NULL,
    //ENEMIES
        *beetle                    = NULL;

    int
        init(),
        distance(int,int,int,int),
        Collision(int, int, int, int, int, int, int, int);


    void
         draw(),
         update(),
         gameLoop(),
         spawn();

    vector<enemy> vecEnemies(1, enemy(50,40,"char/beetle.png"));

    player player1(50,SCREEN_H - 99, 15, 47, "char/tuxGuy.png");


int main(int argc, char **argv){

    error = init();

    if(error){return error;}

    cout << "Controls:\n  W - Jump\n  A - Left\n  D - Right\n  ESC - Quit" << endl;

    al_register_event_source(eventQ,al_get_keyboard_event_source());
    al_register_event_source(eventQ,al_get_display_event_source(mainDisplay));
    al_register_event_source(eventQ,al_get_timer_event_source(timer));
    al_register_event_source(eventQ,al_get_timer_event_source(enemyTimer));

    al_start_timer(timer);
    al_start_timer(enemyTimer);

    gameLoop();

    return 0;
}

int init(){

    if(!al_init()){
        al_show_native_message_box(mainDisplay, "Error", "Error",
                                   "Failed to initialize Allegro",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    timer = al_create_timer(1.0/FPS);
    enemyTimer = al_create_timer(spawnTime);

    if(!timer || !enemyTimer){
        al_show_native_message_box(mainDisplay,"Error","Error",
                                   "Failed to initialize timer",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    eventQ = al_create_event_queue();

    if(!eventQ){
        al_show_native_message_box(mainDisplay,"Error","Error",
                                   "Failed to initialize event queue",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if(!al_init_image_addon()){
        al_show_native_message_box(mainDisplay,"Error","Error",
                                   "Failed to initialize image addons",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if(!al_install_keyboard()){
        al_show_native_message_box(mainDisplay,"Error","Error",
                                   "Failed to initialize keyboard input",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    mainDisplay = al_create_display(SCREEN_W,SCREEN_H);

    if(!mainDisplay){
        al_show_native_message_box(mainDisplay,"Error","Error",
                                   "Failed to initialize Display",
                                   NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    Background  = al_load_bitmap("terr/Background.png");
    Dirt        = al_load_bitmap("terr/Dirt.png");
    stoneWall   = al_load_bitmap("terr/Wall.png");
    treeTrunk   = al_load_bitmap("terr/TreeTrunk.png");
    treeTop     = al_load_bitmap("terr/TreeTop.png");
    treeBranch  = al_load_bitmap("terr/TreeBranch.png");

    playerSkin  = al_load_bitmap("char/tuxGuy.png");

    beetle      = al_load_bitmap("char/beetle.png");

    return 0;

}

void draw(){

    al_draw_bitmap(Background,0,0,0);
    al_draw_bitmap(Dirt,0,SCREEN_H - 50,0);
    al_draw_bitmap(Dirt,300,SCREEN_H - 50,0);
    al_draw_bitmap(Dirt,600,SCREEN_H - 50,0);
    al_draw_bitmap(Dirt,900,SCREEN_H - 50,0);
    al_draw_bitmap(Dirt,1200,SCREEN_H - 50,0);
    al_draw_bitmap(treeTrunk,0,SCREEN_H - 350,0);
    al_draw_bitmap(treeTrunk,0,SCREEN_H - 650,0);
    al_draw_bitmap(treeTrunk,0,SCREEN_H - 950,0);
    al_draw_bitmap(treeTrunk,SCREEN_W - 47,SCREEN_H - 350,0);
    al_draw_bitmap(treeTrunk,SCREEN_W - 47,SCREEN_H - 650,0);
    al_draw_bitmap(treeTrunk,SCREEN_W - 47,SCREEN_H - 950,0);
    al_draw_bitmap(treeTop,0,0,0);
    al_draw_bitmap(treeTop,300,0,0);
    al_draw_bitmap(treeTop,600,0,0);
    al_draw_bitmap(treeTop,900,0,0);
    al_draw_bitmap(treeTop,1200,0,0);
    al_draw_bitmap(treeBranch,47,SCREEN_H - 150,0);
    al_draw_bitmap(treeBranch,247,SCREEN_H - 150,0);
    al_draw_bitmap(treeBranch,SCREEN_W - 247,SCREEN_H - 230,0);
    al_draw_bitmap(treeBranch,SCREEN_W - 447,SCREEN_H - 230,0);
    al_draw_bitmap(treeBranch,SCREEN_W - 647,SCREEN_H - 230,0);
    al_draw_bitmap(treeBranch,SCREEN_W - 247,SCREEN_H - 330,0);
    al_draw_bitmap(treeBranch,47,SCREEN_H - 400,0);
    al_draw_bitmap(treeBranch,247,SCREEN_H - 400,0);
    al_draw_bitmap(treeBranch,447,SCREEN_H - 400,0);
    al_draw_bitmap(treeBranch,647,SCREEN_H - 400,0);

    al_draw_bitmap(playerSkin,player1.dim[X],player1.dim[Y],0);

    for(unsigned int i = 0; i < vecEnemies.size(); i++){

        al_draw_bitmap(beetle, vecEnemies[i].dim[X], vecEnemies[i].dim[Y],0);

    }

    al_flip_display();
}

void gameLoop(){

    ALLEGRO_EVENT event;

    while(!error){

        al_wait_for_event(eventQ,&event);

        if(event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer){
            //JUMPING
            if(key[W] && player1.grounded && canJump){
                player1.dy = -jump;
                player1.grounded = false;
            }
            //RUN LEFT
            if(key[A]){
                player1.dx = -speed;
            }
            //RUN RIGHT
            if(key[D]){
                player1.dx = speed;
            }
            //STOP MOTION
            if(!key[A] && !key[D]){
                player1.dx = 0;
            }
            //GRAVITY
            if(!player1.grounded){
                player1.dy = GRAVITY;
            }

            redraw = true;
        }
        //CLOSING DISPLAY
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){break;}
        //KEY PRESS
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(event.keyboard.keycode){
                case ALLEGRO_KEY_W:
                    key[W] = true;
                    break;
                case ALLEGRO_KEY_A:
                    key[A] = true;
                    break;
                case ALLEGRO_KEY_S:
                    key[S] = true;
                    break;
                case ALLEGRO_KEY_D:
                    key[D] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[SPACE] = true;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    error = 27;
                    break;
            }
        }
        //KEY RELEASE
        else if(event.type == ALLEGRO_EVENT_KEY_UP){
            switch(event.keyboard.keycode){
                case ALLEGRO_KEY_W:
                    key[W] = false;
                    break;
                case ALLEGRO_KEY_A:
                    key[A] = false;
                    break;
                case ALLEGRO_KEY_S:
                    key[S] = false;
                    break;
                case ALLEGRO_KEY_D:
                    key[D] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[SPACE] = false;
                    break;
            }
        }

        if(redraw && al_is_event_queue_empty(eventQ)){
            //PLAYER PHYSICS
            if(player1.dy > 7){player1.dy = 7;}
            if(player1.dy < -10){player1.dy = -10;}

            if(player1.grounded && player1.dy > 0){player1.dy = -1;}

            player1.dim[X] += player1.dx;
            player1.dim[Y] += player1.dy;

            switch(Collision(player1.dim[X], player1.dim[Y], player1.dim[width], player1.dim[height], 0,SCREEN_H - 100, SCREEN_W, 100)){


                case 1:     //Lands on
                    if(player1.dy > 0){player1.dy = 0;}
                    player1.grounded = true;
                    break;

                case 2:     //Hits Right
                    if(player1.dx < 0){player1.dx = 0;}
                    break;

                case 3:     //Hits Left
                    if(player1.dx > 0){player1.dx = 0;}
                    break;

                case 4:     //Hits Bottom
                    if(player1.dy < 0){player1.dy = 0;}
                    break;

            }

             //ENEMY PHYSICS
            for(unsigned int i = 0; i < vecEnemies.size(); i++){

                if(vecEnemies[i].dy < -10){vecEnemies[i].dy = -10;}
                if(vecEnemies[i].dy > 7){vecEnemies[i].dy = 7;}

                vecEnemies[i].dim[Y] += vecEnemies[i].dy;
                if(!vecEnemies[i].grounded){

                    vecEnemies[i].dy += GRAVITY;
                }
            }
        }

        cout << "grounded: " << player1.grounded << endl;
        cout << "col: " << Collision(player1.dim[X], player1.dim[Y], player1.dim[width], player1.dim[height],0,SCREEN_H - 100,SCREEN_W, 100) << endl;
        cout << "dy: " << player1.dy << endl;
        cout << "dx: " << player1.dx << endl;

        if(event.timer.source == enemyTimer){spawn();}

        draw();

    }

}

int Collision(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2){

    /*
    0 - no collision
    1 - landing on top
    2 - right side
    3 - left side
    4 - hitting Bottom
    */

    if((y1 + h1) < (y2 - 5)){
        return 1;
    }

    /*
    if(x1 < (x2 + w2) &&
       x2 < (x1 + w1) &&
       y1 < (y2 + h2) &&
       y2 < (y1 + h1))
       {

           //Top
           if(y1 + h1/2 < y2){
              cout << "top" << endl;
              return 1;}

           //Right
           if(x1 > x2 + w2 - 50){return 2;}

           //Left
           if(x1 < x2){return 3;}

           //Bottom
           if(y1 > y2){return 4;}

       }
    */
    return 0;
}

void spawn(){
    vecEnemies.push_back(enemy(50,40,"char/beetle.png"));
    vecEnemies.push_back(enemy(SCREEN_W - 80,40,"char/beetle.png"));

    spawnCount += 2;
}

int distance(int x1,int y1, int x2, int y2){
    float dist;

    dist = sqrt(pow(y1 - y2, 2) + pow(x1 - x2, 2));

    return dist;
}
