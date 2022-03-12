// Making car racing game


#include "raylib.h"

int main(){
    const int width = 300;
    const int height = 400;
    int v0{2};
    int velocity{4};

    InitWindow(width, height, "Game");
    // player character
    Texture2D car = LoadTexture("characters/car.png");
    Rectangle carRec;
    carRec.width = car.width;
    carRec.height = car.height;
    carRec.x = 0;
    carRec.y = 0;
    Vector2 carPos;
    carPos.x = width/2 - carRec.width/2;
    carPos.y = height - carRec.height - 10;

    // road
    Texture2D road = LoadTexture("characters/road.png");
    float roadY = 0;
    

    // enemy
    Texture2D e1 = LoadTexture("characters/enemy1.png");
    Rectangle e1Rec;
    e1Rec.x = 0; e1Rec.y = 0;
    e1Rec.width = e1.width; e1Rec.height = e1.height; 
    Vector2 e1Pos ;
    e1Pos.x = GetRandomValue(65, width - e1Rec.width - 65);
    e1Pos.y = -e1.height + 5;
    
    
    SetTargetFPS(60);
    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(WHITE);

        roadY += v0 ;
        


        if(roadY >= road.height){
            roadY = 0.0;
        }
        
        Vector2 roadPos1{0.0, roadY};
        DrawTextureEx(road, roadPos1, 0.0, 1.0 , WHITE);

        Vector2 roadPos2{0.0, roadY - road.height };
        DrawTextureEx(road, roadPos2, 0.0, 1.0, WHITE);
        
        

        DrawTextureRec(car, carRec, carPos, WHITE);
        DrawTextureRec(e1, e1Rec, e1Pos, WHITE);

        if(IsKeyDown(KEY_A) && carPos.x > 65){
            carPos.x -= 10;
        }
        if(IsKeyDown(KEY_D) && carPos.x < width - carRec.width -65){
            carPos.x += 10;
        }
        int temp = velocity;
        e1Pos.y += temp;
        if(e1Pos.y > height + GetRandomValue(30,100)){
            e1Pos.y = GetRandomValue(-100-e1.height,-e1.height);
            e1Pos.x = GetRandomValue(65, width - e1Rec.width - 65);
            velocity = GetRandomValue(3,9);
            temp = velocity;
        }

        EndDrawing();

    }
    
    UnloadTexture(car);
    UnloadTexture(road);
    UnloadTexture(e1);
    CloseWindow();
}