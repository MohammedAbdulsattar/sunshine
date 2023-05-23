#include "rlImGui.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color); // draw a line
void DrawCircle(int centerX, int centerY, float radius, Color color); // draw a circle
void DrawRectangle(int posX, int posY, int width, int height, Color color); // draw a rectangle

bool IsKeyPressed(int key); // Check if a key has been pressed once
bool IsKeyDown(int key); // Check if a key is being pressed
bool IsKeyReleased(int key); // Check if a key has been released once
bool IsKeyUp(int key); // Check if a key is NOT being pressed

Texture2D LoadTexture(const char* fileName); // Load texture from file into GPU memory (VRAM)
Sound LoadSound(const char* fileName); // Load sound from file

void SetMusicVolume(Music music, float volume); // Set volume for music (1.0 is max level)
void SetSoundVolume(Sound sound, float volume); // Set volume for a sound (1.0 is max level)

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    SetTargetFPS(60);

	// Testing Lab 1 Change. This is the main code and it renders "Hello World!" while game application is still running

	Vector2 ballPosition = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 }; // Create a circular ball positioned at half the screen width and height
    Texture2D titleScreen = LoadTexture("../game/assets/textures/TitleScreen.png"); // Load Texture from File

    rlImGuiSetup(true);
    InitAudioDevice();

    bool musicPaused = false; // bool check to see if music is paused
    bool useGUI = false; // bool check to see if GUI is being used
    // kindly stolen from youtube. Probably piracy. https://www.youtube.com/watch?v=k8z07I1fzbQ&ab_channel=Lyn-Topic
    Music mainTheme = LoadMusicStream("../game/assets/audio/Throw_Away_Your_Mask.mp3"); // Load Music from File
    // kindly ripped from youtube. Royalty free. https://www.youtube.com/watch?v=Wi23bXE-h54&list=PLv4nWDim3p_orIE0b4kNcqx1P5FoyeG1-&index=10&ab_channel=SoundEffects%E3%80%90FREE%E3%80%91
    Sound villainLaugh = LoadSound("../game/assets/audio/VILLAIN_LAUGH.wav"); // Load Sound from File



	while (!WindowShouldClose()) // While the window is still running and escape key or window 'x' button isn't pressed
    {
        BeginDrawing(); // start drawing
        ClearBackground(RAYWHITE);

        DrawFPS(10, 10);

        DrawText("Hello World!", 500, 9, 20, RED); // basic output

    	DrawLine(50, 50, 150, 150, { 128,0,128,255 }); // draw a basic line
        DrawCircle(250, 250, 50, { 128,128,0,255 }); // draw a basic circle
        DrawRectangle(350, 350, 200, 200, { 0,128,128,255 }); // draw a basic rectangle

        DrawText("move the ball with arrow keys", 500, 36, 20, DARKGRAY); // instructions for moving ball 
        DrawCircleV(ballPosition, 50, PURPLE); // Draw a vector circle with a center, set radius and color which can be moved with key input
        if (IsKeyDown(KEY_W))
        {
            ballPosition.y -= 2.0f; // move up, pixel precision
        }
        if (IsKeyDown(KEY_S))
        {
            ballPosition.y += 2.0f; // move down, pixel precision
        }
        if (IsKeyDown(KEY_A))
        {
            ballPosition.x -= 2.0f; // move left, pixel precision
        }
        if (IsKeyDown(KEY_D))
        {
            ballPosition.x += 2.0f; // move right, pixel precision
        }
		DrawTexture(titleScreen,700, 200, { 255,255,255,200 }); // Draw the Loaded Texture

        SetMusicVolume(mainTheme, 0.1f); // Set volume for music (1.0 is max level)
        SetSoundVolume(villainLaugh, 0.1f); // Set volume for a sound (1.0 is max level)

        UpdateMusicStream(mainTheme); // Update music and continue playing through the music track

        if (musicPaused)
        {
            PauseMusicStream(mainTheme); // Pause music when paused
        }
        else
        {
            PlayMusicStream(mainTheme); // Continue playing music
        }

        if (IsKeyPressed(KEY_V))
        {
            PlaySound(villainLaugh); // Play the sound effect when the Key is pressed
        }

        DrawRectangle(970, 10, 300, 30, { 200, 150, 200, 255 });
        DrawText("Press ~ to open/close GUI", 980, 15, 20, WHITE);

        if (IsKeyPressed(KEY_GRAVE))
        {
            useGUI = !useGUI; // Flip trigger for GUI Render when Tilde (~) Key is pressed
        }

        if (useGUI)
        {
            rlImGuiBegin(); // Render GUI

            if (ImGui::Button("Play Villain Laugh"))
                PlaySound(villainLaugh); // Play the sound Effect

            if (musicPaused)
            {
                if (ImGui::Button("Play Main Menu Music"))
                    musicPaused = false; // Set music to unpause and play
            }
            else
            {
                if (ImGui::Button("Pause Main Menu Music"))
                    musicPaused = true; // Set music to pause and stop playing
            }

            if (ImGui::Button("Restart Main Menu Music"))
                SeekMusicStream(mainTheme, 0.0f); // Restart the music track to the beginning

            rlImGuiEnd(); // End GUI Render
        }

        EndDrawing(); // end drawing
    }

	UnloadSound(villainLaugh); // Unload Sound Effect
    UnloadMusicStream(mainTheme); // Unload Music Track
    CloseAudioDevice(); // Close Audio
    rlImGuiShutdown(); // shut down GUI
    CloseWindow(); // close the window and unload OpenGL
    return 0;
}