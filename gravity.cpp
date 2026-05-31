// this version will only have basic gravity and movements
#include<raylib.h>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

class objects{
    public:
        Vector2 position;//position vector (X+Y)
        Vector2 velocity;//velocity vector(Vx+Vy)
        Vector2 accelartion={0,0};//accelaration vector(ax+ay). intitially
        void setposition(Vector2 pos){ //sets position 
            position.x = pos.x;
            position.y = pos.y;
        }
        void setvelocity(Vector2 vel){ //sets velocity
            velocity.x = vel.x;
            velocity.y = vel.y;
        }
        void updateaccelartion(Vector2 acc){ //sets velocity
            accelartion.x += acc.x;
            accelartion.y += acc.y;
        }
};

class rectangle : public objects{
    public :
        Vector2 size;//size of the rectangle (width , height)
        void setsize(Vector2 s){
            size.x = s.x;
            size.y = s.y;
        }
        


};


int main(int argc , char* argv[]){

    const int screenwidth = 970;//scrren width
    const int screenheight= 1000;//scrren height
    

    //intilizing the objects
    vector<rectangle> rectangles(1);
    vector<Vector2> pos_s = {{screenwidth/2,screenheight/2}};
    vector<Vector2> vel_s = {{0.5,0.2}};
    vector<Vector2> size_s = {{20,30}};
    int i =0;
    for(auto& rec : rectangles){
        rec.setposition(pos_s[i]);
        rec.setvelocity(vel_s[i]);
        rec.setsize(size_s[i]);
        i++;
    }

    int single_push = 1;
    rectangle& controlled = rectangles[0];

    // initializing the window
    InitWindow(screenwidth,screenheight,"Gravity Simulator");//initialize the window 
    SetTargetFPS(60);


    // Simulation loop(rendering loop)

    while(!WindowShouldClose()){

        //LOGICs


        float dt = GetFrameTime(); //time for each frame

        //keyboard reponse(assumption that on clicking utton we are applying velocity on object)
        if(IsKeyDown(KEY_UP)) controlled.velocity.y -= single_push;
        if(IsKeyDown(KEY_DOWN)) controlled.velocity.y += single_push;
        if(IsKeyDown(KEY_LEFT)) controlled.velocity.x -= single_push;
        if(IsKeyDown(KEY_RIGHT)) controlled.velocity.x += single_push;

        // updating position and velocity
        for(auto& rec : rectangles){
            //apply accelaration
            rec.velocity.x += rec.accelartion.x;
            rec.velocity.y += rec.accelartion.y;
            //apply velocity
            rec.position.x += rec.velocity.x;
            rec.position.y += rec.velocity.y;
        }

        //rendering
        BeginDrawing();
        ClearBackground(WHITE);

        for(auto& rec : rectangles){
            DrawRectangleV(rec.position,rec.size,BLACK);
        }
        //printing current position , velocity and accelaration 
        string details = "POS X : "+to_string(controlled.position.x) + "\n"+
                         "POS Y : "+to_string(controlled.position.y) + "\n"+
                         "VEL X : "+to_string(controlled.velocity.x) + "\n"+
                         "VEL Y : "+to_string(controlled.velocity.y) ;

        DrawText(details.c_str(),10,10,20,BLACK);


        EndDrawing();

    }

    CloseWindow();
    return 0;
}



// for compiling and running
/*
g++ gravity.cpp -o engine -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./engine
*/