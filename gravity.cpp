// this version will only have basic gravity and movements
#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


// WORLD CONTEXT STRUCT

struct WorldContext {
    int screenwidth;
    int screenheight;
    float coef_of_restitution;
    float gravity;
    float friction_coef;
    int single_push;
    Vector2 external_accln;
};

class objects {
public:
    Vector2 position;     // position vector (X+Y)
    Vector2 velocity;     // velocity vector(Vx+Vy)
    Vector2 acceleration = {0, 0}; // acceleration vector(ax+ay) initially

    void setposition(Vector2 pos) { 
        position.x = pos.x;
        position.y = pos.y;
    }
    void setvelocity(Vector2 vel) { 
        velocity.x = vel.x;
        velocity.y = vel.y;
    }
    void updateacceleration(Vector2 acc) { 
        acceleration.x += acc.x;
        acceleration.y += acc.y;
    }
};

class rectangle : public objects {
public:
    Vector2 size; // size of the rectangle (width , height)
    
    void setsize(Vector2 s) {
        size.x = s.x;
        size.y = s.y;
    }
};

// ==========================================
// CLEANED UP LOGIC FUNCTION
// ==========================================
void UpdatePhysicsLogic(vector<rectangle>& rectangles, float dt, const WorldContext& ctx) {
    
    rectangle& controlled = rectangles[0];

    // Keyboard response (applying velocity on object)
    if (IsKeyDown(KEY_UP)) controlled.velocity.y -= ctx.single_push * dt;
    if (IsKeyDown(KEY_DOWN)) controlled.velocity.y += ctx.single_push * dt;
    if (IsKeyDown(KEY_LEFT)) controlled.velocity.x -= ctx.single_push * dt;
    if (IsKeyDown(KEY_RIGHT)) controlled.velocity.x += ctx.single_push * dt;

    // Updating position, velocity, and acceleration
    for (auto& rec : rectangles) {
        // Updating acceleration (external acc)
        rec.acceleration.x = ctx.external_accln.x;
        rec.acceleration.y = ctx.external_accln.y;

        // Apply acceleration (v = u + at)
        rec.velocity.x += rec.acceleration.x * dt;
        rec.velocity.y += rec.acceleration.y * dt;

        // Apply friction
        if (rec.position.y + rec.size.y >= ctx.screenheight) {
            float friction_drop = ctx.gravity * ctx.friction_coef * dt;
            
            if (rec.velocity.x > 0) {
                rec.velocity.x -= friction_drop;
                if (rec.velocity.x < 0) rec.velocity.x = 0; // Prevent jittering backward
            } else if (rec.velocity.x < 0) {
                rec.velocity.x += friction_drop;
                if (rec.velocity.x > 0) rec.velocity.x = 0; // Prevent jittering forward
            }
        }

        // Apply velocity (sf = si + v*t)
        rec.position.x += rec.velocity.x * dt;
        rec.position.y += rec.velocity.y * dt;
    }

    // Boundary handling
    for (auto& rec : rectangles) {
        // Check Right Wall
        if (rec.position.x + rec.size.x >= ctx.screenwidth) {
           rec.position.x = ctx.screenwidth - rec.size.x; 
           rec.velocity.x = -rec.velocity.x * ctx.coef_of_restitution;          
        }
        // Check Left Wall
        else if (rec.position.x <= 0) {
           rec.position.x = 0;                        
           rec.velocity.x = -rec.velocity.x * ctx.coef_of_restitution;          
        }

        // Check Bottom Wall (Floor)
        if (rec.position.y + rec.size.y >= ctx.screenheight) {
           rec.position.y = ctx.screenheight - rec.size.y; 
           rec.velocity.y = -rec.velocity.y * ctx.coef_of_restitution;           
        }
        // Check Top Wall (Ceiling)
        else if (rec.position.y <= 0) {
           rec.position.y = 0;                        
           rec.velocity.y = -rec.velocity.y * ctx.coef_of_restitution;          
        }
    }
}


int main(int argc , char* argv[]) {

    // ==========================================
    // INITIALIZING WORLD CONTEXT
    // ==========================================
    WorldContext world = {
        970,          // screenwidth
        1000,         // screenheight
        0.6f,         // coef_of_restitution
        980.0f,       // gravity (treating 100pixel as 1 meter)
        0.02f,        // friction_coef
        5000,         // single_push
        {0, 980.0f}   // external_accln
    };
    
    // Initializing the objects
    vector<rectangle> rectangles(1);
    vector<Vector2> pos_s = {{world.screenwidth / 2.0f, world.screenheight / 2.0f}};
    vector<Vector2> vel_s = {{340, -140}};
    vector<Vector2> size_s = {{20, 30}};
    
    int i = 0;
    for (auto& rec : rectangles) {
        rec.setposition(pos_s[i]);
        rec.setvelocity(vel_s[i]);
        rec.setsize(size_s[i]);
        i++;
    }

    // Initializing the window
    InitWindow(world.screenwidth, world.screenheight, "Gravity Simulator"); 
    SetTargetFPS(60);

    // Simulation loop (rendering loop)
    while (!WindowShouldClose()) {

        float dt = GetFrameTime(); // time for each frame

        // ==========================================
        // CALLING LOGIC WITH STRUCT
        // ==========================================
        UpdatePhysicsLogic(rectangles, dt, world);

        // Rendering
        BeginDrawing();
        ClearBackground(WHITE);

        for (auto& rec : rectangles) {
            DrawRectangleV(rec.position, rec.size, BLACK);
        }
        
        // Printing current position, velocity, and acceleration for the controlled object
        rectangle& controlled = rectangles[0];
        string details = "POS X : " + to_string(controlled.position.x) + "\n" +
                         "POS Y : " + to_string(controlled.position.y) + "\n" +
                         "VEL X : " + to_string(controlled.velocity.x) + "\n" +
                         "VEL Y : " + to_string(controlled.velocity.y);

        DrawText(details.c_str(), 10, 10, 20, BLACK);

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