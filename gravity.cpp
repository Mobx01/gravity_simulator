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
    vector<Vector2> vel_s = {{340,-140}};
    vector<Vector2> size_s = {{20,30}};
    int i =0;
    for(auto& rec : rectangles){
        rec.setposition(pos_s[i]);
        rec.setvelocity(vel_s[i]);
        rec.setsize(size_s[i]);
        i++;
    }

    // values

    const float coef_of_resistution = 0.6;// for this world 
    const float gravity = 980; // treating 100pixel as 1 meter
    const float friction_coef = 0.02;
    Vector2 external_accln = {0,gravity};
    int single_push = 5000;
    rectangle& controlled = rectangles[0];


    // initializing the window
    InitWindow(screenwidth,screenheight,"Gravity Simulator");//initialize the window 
    SetTargetFPS(60);



    // Simulation loop(rendering loop)

    while(!WindowShouldClose()){

        //LOGICs


        float dt = GetFrameTime(); //time for each frame

        //keyboard reponse(assumption that on clicking utton we are applying velocity on object)
        if(IsKeyDown(KEY_UP)) controlled.velocity.y -= single_push*dt;
        if(IsKeyDown(KEY_DOWN)) controlled.velocity.y += single_push*dt;
        if(IsKeyDown(KEY_LEFT)) controlled.velocity.x -= single_push*dt;
        if(IsKeyDown(KEY_RIGHT)) controlled.velocity.x += single_push*dt;



        // updating position , velocity and accelaration
        for(auto& rec : rectangles){
            //updating accelaration (external acc)
            rec.accelartion.x = external_accln.x;
            rec.accelartion.y = external_accln.y;


            //apply accelaration (v = u + at)
            rec.velocity.x += rec.accelartion.x*dt;
            rec.velocity.y += rec.accelartion.y*dt;


            // apply friction
            if(rec.position.y + rec.size.y >= screenheight) {
                float friction_drop = gravity * friction_coef * dt;
                
                if (rec.velocity.x > 0) {
                    rec.velocity.x -= friction_drop;
                    if (rec.velocity.x < 0) rec.velocity.x = 0; // Prevent jittering backward
                } else if (rec.velocity.x < 0) {
                    rec.velocity.x += friction_drop;
                    if (rec.velocity.x > 0) rec.velocity.x = 0; // Prevent jittering forward
                }
            }



            //apply velocity ( sf = si + v*t )
            rec.position.x += rec.velocity.x*dt;
            rec.position.y += rec.velocity.y*dt;

            
        }


        //boundary
        for(auto& rec : rectangles) {
            // Check Right Wall
            if (rec.position.x + rec.size.x >= screenwidth) {
               rec.position.x = screenwidth - rec.size.x; // Snap to right edge
               rec.velocity.x = -rec.velocity.x*coef_of_resistution;          // Bounce
            }
            // Check Left Wall
            else if (rec.position.x <= 0) {
               rec.position.x = 0;                        // Snap to left edge
               rec.velocity.x = -rec.velocity.x*coef_of_resistution;          // Bounce
            }

            // Check Bottom Wall (Floor)
            if (rec.position.y + rec.size.y >= screenheight) {
               rec.position.y = screenheight - rec.size.y; // Snap exactly to floor
               rec.velocity.y = -rec.velocity.y*coef_of_resistution;           // Bounce
            }
            // Check Top Wall (Ceiling)
            else if (rec.position.y <= 0) {
               rec.position.y = 0;                        // Snap to ceiling
               rec.velocity.y = -rec.velocity.y*coef_of_resistution;          // Bounce
            }
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