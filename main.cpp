#include <iostream>
#include <raylib.h>
#include <deque>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;

double lastUpdateTime = 0;

double eventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{10, 10}, Vector2{10, 11}, Vector2{10, 12}};
    Vector2 direction = {1, 0};

    void draw()
    {
        float fCellSize = static_cast<float>(cellSize);
        for (auto &cell : body)
        {
            Rectangle segment = Rectangle{cell.x * cellSize, cell.y * cellSize, fCellSize, fCellSize};
            DrawRectangleRounded(segment, 0.15, 0, darkGreen);
        }
    }

    void update()
    {
        Vector2 newHead = {body.front().x + direction.x, body.front().y + direction.y};
        body.push_front(newHead);
        body.pop_back();
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food()
    {
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        randomizePosition();
    }

    void draw()
    {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }

    void randomizePosition()
    {
        position.x = GetRandomValue(0, cellCount - 1);
        position.y = GetRandomValue(0, cellCount - 1);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }
};

class Game
{
public:
    Snake snake;
    Food food;

    void update()
    {
        snake.update();
    }

    void draw()
    {
        food.draw();
        snake.draw();
    }
};

int main()
{
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (eventTriggered(0.2))
        {
            game.update();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
        }
        else if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
        }
        else if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
        }
        else if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
        }

        ClearBackground(green);
        game.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}