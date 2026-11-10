#include "raylib.h"
#include <iostream>
#include <vector>


using namespace std;





//classes------------------------------
class Object
{
    public:
        int POSX;
        int POSY; // the x and y positions of the object
        int SIZE; // the size of the object - use the objects sprite size
        bool isActive = false; // if the object is currently active;
        //AABB collision check - since using SIZE and not WxH make sure all sprites in game are roughly square
        bool CollisionCheck(Object& other)
        {
            if (POSX < other.POSX + other.SIZE &&
                POSX + SIZE > other.POSX &&
                POSY < other.POSY + other.SIZE &&
                POSY + SIZE > other.POSY)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
};

class Bullet : public Object
{
    
            public:
            int Direction; // "-" for up and "+" for down
    
          Bullet()
          {
              SIZE = 2;
          }
          
    //bullets should call this function every frame
    void MoveForward()
    {
        POSY = POSY + 5 * Direction;
    }
};

//could very easily modify this so enemys could work too??
class Ship : public Object
{

    public:
        Bullet PlayerBullets[3];

        void SpawnShip()
        {
            POSX = 400;
            POSY = 700;
            SIZE = 9;
            isActive = true;
            //set all the players bullets to not be active so none are left on sceen, also set direction
            for (int i = 0; i < 3; i++)
            {
                PlayerBullets[i].isActive = false;
                PlayerBullets[i].Direction = -1;
            }
        }

        void SpawnBullet()
        {
            for (int i = 0; i < 3; i++)
            {
                if (!PlayerBullets[i].isActive)
                {
                    PlayerBullets[i].POSX = POSX;
                    PlayerBullets[i].POSY = POSY + 2;
                    PlayerBullets[i].isActive = true;
                    break;
                }
            }
        }
};

//global variables -------------------------
const int MAPWIDTH = 800, MAPHEIGHT = 800;
Ship PlayerShip;
Bullet EnemyBullet;



void Init()
{
    PlayerShip.SpawnShip();
}


void DrawObjects()
{
    DrawRectangle(PlayerShip.POSX, PlayerShip.POSY, PlayerShip.SIZE, PlayerShip.SIZE, RED);
    
    //put all active objects in a vector and draw them;

    for (int i = 0; i < 3; i++)
    {
        if (PlayerShip.PlayerBullets[i].isActive)
        {
            PlayerShip.PlayerBullets[i].MoveForward();
            if (PlayerShip.PlayerBullets[i].POSY < 0)
            {
                PlayerShip.PlayerBullets[i].isActive = false;
            }
            DrawRectangle(PlayerShip.PlayerBullets[i].POSX, PlayerShip.PlayerBullets[i].POSY, 2, 2, WHITE);
        }
    }

    //here for testing
    if (EnemyBullet.isActive)
    {
        DrawRectangle(EnemyBullet.POSX, EnemyBullet.POSY, 2, 2, WHITE);
        EnemyBullet.MoveForward();
        if (EnemyBullet.CollisionCheck(PlayerShip))
        {
            cout << "SHIP WAS HIT!!" << endl;
        }
    }
}


int main()
{
    Init();

    InitWindow(MAPWIDTH, MAPHEIGHT, "TestWindow");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        



        if (IsKeyDown(KEY_LEFT))
        {
            PlayerShip.POSX--;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            PlayerShip.POSX++;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            PlayerShip.SpawnBullet();
        }

        if (IsKeyPressed(KEY_B))
        {
            EnemyBullet.isActive = true;
            EnemyBullet.POSX = PlayerShip.POSX;
            EnemyBullet.POSY = 32;
            EnemyBullet.Direction = 1;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawObjects();
        EndDrawing();


    }

    CloseWindow();

    return 0;
}

