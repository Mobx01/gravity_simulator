// this version apply mutual gravitational force between bodies.(simulating solar systme)
#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath> 

using namespace std;

// --- GLOBAL WORLD CONSTANTS ---
// Moving these to the top makes them accessible anywhere and easy to tweak!
const int SCREEN_WIDTH = 970; 
const int SCREEN_HEIGHT = 1000; 
const float G = 1000.0f; 
const float SOFTENING = 5000.0f; 
const float COEF_OF_RESTITUTION = 0.8f;
const int SINGLE_PUSH = 5000;

class objects {
    public:
        Vector2 position; 
        Vector2 velocity; 
        Vector2 accelartion = {0,0}; 
        float mass = 1.0f; 

        void setposition(Vector2 pos) { position = pos; }
        void setvelocity(Vector2 vel) { velocity = vel; }
        void setmass(float m) { mass = m; }
};

class rectangle : public objects {
    public :
        Vector2 size; 
        void setsize(Vector2 s) { size = s; }
};

// --- NEW LOGIC FUNCTIONS ---

// 1. Handles all keyboard inputs
void HandleInput(rectangle& controlled, float dt) {
    if(IsKeyDown(KEY_UP)) controlled.velocity.y -= SINGLE_PUSH * dt;
    if(IsKeyDown(KEY_DOWN)) controlled.velocity.y += SINGLE_PUSH * dt;
    if(IsKeyDown(KEY_LEFT)) controlled.velocity.x -= SINGLE_PUSH * dt;
    if(IsKeyDown(KEY_RIGHT)) controlled.velocity.x += SINGLE_PUSH * dt;
}

// 2. Handles all movement, gravity, and collisions
void UpdatePhysics(vector<rectangle>& rectangles, float dt) {
    // 1. CLEAR ACCELERATIONS
    for(auto& rec : rectangles) {
        rec.accelartion = {0, 0};
    }

    // 2. CALCULATE MUTUAL GRAVITY
    for(int i = 0; i < rectangles.size(); i++) {
        for(int j = 0; j < rectangles.size(); j++) {
            if (i == j) continue; 

            float dx = rectangles[j].position.x - rectangles[i].position.x;
            float dy = rectangles[j].position.y - rectangles[i].position.y;
            float distSqr = (dx * dx) + (dy * dy);
            float dist = sqrt(distSqr);

            float force_accel = (G * rectangles[j].mass) / (distSqr + SOFTENING);

            rectangles[i].accelartion.x += (dx / dist) * force_accel;
            rectangles[i].accelartion.y += (dy / dist) * force_accel;
        }
    }

    // 3. APPLY PHYSICS
    for(auto& rec : rectangles){
        rec.velocity.x += rec.accelartion.x * dt;
        rec.velocity.y += rec.accelartion.y * dt;
        rec.position.x += rec.velocity.x * dt;
        rec.position.y += rec.velocity.y * dt;
    }

    // 4. BOUNDARY COLLISIONS
    for(auto& rec : rectangles) {
        if (rec.position.x + rec.size.x >= SCREEN_WIDTH) {
            rec.position.x = SCREEN_WIDTH - rec.size.x; 
            rec.velocity.x = -rec.velocity.x * COEF_OF_RESTITUTION;          
        }
        else if (rec.position.x <= 0) {
            rec.position.x = 0;                        
            rec.velocity.x = -rec.velocity.x * COEF_OF_RESTITUTION;          
        }
        if (rec.position.y + rec.size.y >= SCREEN_HEIGHT) {
            rec.position.y = SCREEN_HEIGHT - rec.size.y; 
            rec.velocity.y = -rec.velocity.y * COEF_OF_RESTITUTION;           
        }
        else if (rec.position.y <= 0) {
            rec.position.y = 0;                        
            rec.velocity.y = -rec.velocity.y * COEF_OF_RESTITUTION;          
        }
    }
}


int main(int argc , char* argv[]) {
    // Setup Objects
    vector<rectangle> rectangles(3);
    
    // Object 0 (The Heavy "Sun")
    rectangles[0].setposition({SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f});
    rectangles[0].setvelocity({0, 10.0f}); 
    rectangles[0].setsize({40, 40});
    rectangles[0].setmass(5000.0f); 

    // Object 1 (A "Planet")
    rectangles[1].setposition({200.0f, SCREEN_HEIGHT/2.0f});
    rectangles[1].setvelocity({0, -250.0f}); 
    rectangles[1].setsize({15, 15});
    rectangles[1].setmass(100.0f);

    // Object 2 (Another "Planet")
    rectangles[2].setposition({SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT/2.0f});
    rectangles[2].setvelocity({0, 200.0f}); 
    rectangles[2].setsize({20, 20});
    rectangles[2].setmass(200.0f);

    rectangle& controlled = rectangles[1]; 

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mutual Gravity Simulator"); 
    SetTargetFPS(60);

    // Simulation loop(rendering loop)
    while(!WindowShouldClose()) {

        float dt = GetFrameTime(); 

        // Look how clean the logic section is now!
        HandleInput(controlled, dt);
        UpdatePhysics(rectangles, dt);

        // RENDERING
        BeginDrawing();
        ClearBackground(WHITE);

        for(auto& rec : rectangles){
            DrawRectangleV(rec.position, rec.size, BLACK);
        }
        
        string details = "CONTROLLING PLANET 1\nPOS X : "+to_string(controlled.position.x) + "\n"+
                         "POS Y : "+to_string(controlled.position.y) + "\n"+
                         "VEL X : "+to_string(controlled.velocity.x) + "\n"+
                         "VEL Y : "+to_string(controlled.velocity.y) ;

        DrawText(details.c_str(), 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*
g++ space.cpp -o engine -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./engine
*/
