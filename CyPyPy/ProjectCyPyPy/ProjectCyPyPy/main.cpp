#include <GLFW/glfw3.h>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <SDL_mixer.h>
#include <SDL.h>
#include <vector>
#include <filesystem>
#undef main
#include <string>
#include <Windows.h>
#define SDL_MAIN_HANDLED

#define Draw0(x,y)  glBegin(GL_LINE_LOOP);\
glVertex2f(x, y);\
glVertex2f(x+0.1, y);\
glVertex2f(x+0.1, y+0.2);\
glVertex2f(x, y+0.2 );\
glEnd()

#define Draw1(x,y)  glBegin(GL_LINE_STRIP);\
glVertex2f(x+0.05, y+0.1);\
glVertex2f(x+0.1, y+0.2);\
glVertex2f(x+0.1,y);\
glEnd()

#define Draw2(x,y)  glBegin(GL_LINE_STRIP);\
glVertex2f(x, y+0.2);\
glVertex2f(x+0.1, y+0.2);\
glVertex2f(x+0.1,y+0.1);\
glVertex2f(x,y+0.1);\
glVertex2f(x,y);\
glVertex2f(x+0.1,y);\
glEnd()

#define Draw3(x,y)  glBegin(GL_LINE_STRIP);\
glVertex2f(x, y+0.2);\
glVertex2f(x+0.1, y+0.2);\
glVertex2f(x+0.1,y+0.1);\
glVertex2f(x,y+0.1);\
glVertex2f(x+0.1,y+0.1);\
glVertex2f(x+0.1,y);\
glVertex2f(x,y);\
glEnd()

#define Draw4(x,y)  glBegin(GL_LINE_STRIP);\
glVertex2f(x+0.04, y+0.2);\
glVertex2f(x, y+0.07);\
glVertex2f(x+0.1,y+0.07);\
glVertex2f(x+0.06,y+0.07);\
glVertex2f(x+0.06,y+0.13);\
glVertex2f(x+0.06,y);\
glEnd()

#define Draw5(x,y)  glBegin(GL_LINE_STRIP);\
glVertex2f(x+0.1, y+0.2);\
glVertex2f(x, y+0.2);\
glVertex2f(x,y+0.1);\
glVertex2f(x+0.1,y+0.1);\
glVertex2f(x+0.1,y);\
glVertex2f(x,y);\
glEnd()

#define DrawP(x,y)  glBegin(GL_LINE_STRIP);\
glVertex2f(x, y);\
glVertex2f(x, y+0.2);\
glVertex2f(x+0.1,y+0.2);\
glVertex2f(x+0.1,y+0.1);\
glVertex2f(x,y+0.1);\
glEnd()

#define DrawWIN(x,y) glBegin(GL_LINE_STRIP);\
glVertex2f(x,y+0.2);\
glVertex2f(x+0.025,y);\
glVertex2f(x+0.05,y+0.07);\
glVertex2f(x+0.075,y);\
glVertex2f(x+0.1,y+0.2);\
glEnd();\
glBegin(GL_LINE_STRIP);\
glVertex2f(x+0.15,y+0.2);\
glVertex2f(x+0.15,y);\
glEnd();\
glBegin(GL_LINE_STRIP);\
glVertex2f(x+0.2,y);\
glVertex2f(x+0.2,y+0.2);\
glVertex2f(x+0.3,y);\
glVertex2f(x+0.3,y+0.2);\
glEnd()






typedef struct Players {
    float p1;
    float p2;
} Players;

std::vector<Mix_Chunk*> sounds;
std::vector<Mix_Music*> music;

bool running;

int loadMusic(const char* filename) {
    Mix_Music* m = NULL;
    m = Mix_LoadMUS(filename);
    if (m == NULL) {
        printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
        return -1;
    }
    music.push_back(m);
    return music.size() - 1;
}
int loadSound(const char* filename) {
    Mix_Chunk* m = NULL;
    m = Mix_LoadWAV(filename);
    if (m == NULL) {
        printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
        return -1;
    }
    sounds.push_back(m);
    return sounds.size() - 1;
}

int volume;
void setVolume(int v) {
    volume = (MIX_MAX_VOLUME * v) / 100;
}

int playMusic(int m) {
    if (Mix_PlayingMusic() == 0) {
        Mix_Volume(1, volume / 4);
        Mix_PlayMusic(music[m], -1);
    }
    return 0;
}
int playSound(int s) {
    Mix_Volume(-1, volume);
    Mix_PlayChannel(-1, sounds[s], 0);
    return 0;
}

int initMixer() {
    Mix_Init(MIX_INIT_MP3);
    SDL_Init(SDL_INIT_AUDIO);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_Mixer couldnt init. Err: %s\n", Mix_GetError());
        return -1;
    }
    setVolume(80);
    return 0;
}

void quitMixer() {
    for (size_t s = 0; s < sounds.size(); s++) {
        Mix_FreeChunk(sounds[s]);
        sounds[s] = NULL;
    }
    for (size_t s = 0; s < music.size(); s++) {
        Mix_FreeMusic(music[s]);
        music[s] = NULL;
    }
    Mix_Quit();
}
Players players = { 0.0f,0.0f };
float palettesize = 0.1f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS) && (players.p1 + palettesize <= 1)) {
        players.p1 += 0.04;
    }
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS) && (players.p1 - palettesize >= -1)) {
        players.p1 -= 0.04;
    }
    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS) && (players.p2 + palettesize <= 1)) {
        players.p2 += 0.04;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS) && (players.p2 - palettesize >= -1)) {
        players.p2 -= 0.04;
    }
}

void drawScores(const int scores[2])
{
    glColor4f(0.001f, 0.996f, 0.014f, 1.0f);
    switch (scores[0]) {
    case 0: Draw0(-0.45f, 0.6f); break;
    case 1: Draw1(-0.45f, 0.6f); break;
    case 2: Draw2(-0.45f, 0.6f); break;
    case 3: Draw3(-0.45f, 0.6f); break;
    case 4: Draw4(-0.45f, 0.6f); break;
    case 5: Draw5(-0.45f, 0.6f); break;
    }
    switch (scores[1]) {
    case 0: Draw0(0.45f, 0.6f); break;
    case 1: Draw1(0.45f, 0.6f); break;
    case 2: Draw2(0.45f, 0.6f); break;
    case 3: Draw3(0.45f, 0.6f); break;
    case 4: Draw4(0.45f, 0.6f); break;
    case 5: Draw5(0.45f, 0.6f); break;
    }
}
int main(void)
{
    initMixer();
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit Failed";
        std::cin;
        return -1;
    }


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    float dx = 0.004f * pow(-1.0, rand() % 2);
    float dy = ((rand() % 1000) - 500) / 100000.0;
    float x, y, sizex, sizey;
    x = 0.0f;
    y = 0.0f;
    sizex = 0.03f;
    sizey = 0.05f;
    int song = loadMusic("../res/audio/Szczur.wav");
    int sound = loadSound("../res/audio/anime.wav");
    playMusic(song);
    int bounces = 0;
    int scores[2] = { 0,0 };
    bool win = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glColor4f(0.3f, 0.5f, 0.8f, 1.0f);
        glLineWidth(50);

        glBegin(GL_LINE_LOOP);
        glVertex2f(0.9f, 0.9f);
        glVertex2f(-0.9f, 0.9f);
        glVertex2f(-0.9f, -0.9f);
        glVertex2f(0.9f, -0.9f);
        glEnd();
        if (!win)
        {
            glColor4f(1.0f, 0.3f, 0.2f, 1.0f);
            glBegin(GL_POLYGON);
            glVertex2f(x + sizex / 2, y + sizey / 2);
            glVertex2f(x + sizex / 2, y - sizey / 2);
            glVertex2f(x - sizex / 2, y - sizey / 2);
            glVertex2f(x - sizex / 2, y + sizey / 2);
            glEnd();

            glColor4f(0.996f, 0.996f, 0.03f, 1.0f);
            glBegin(GL_POLYGON);
            glVertex2f(-0.805, players.p1 + palettesize);
            glVertex2f(-0.8, players.p1 + palettesize);
            glVertex2f(-0.8, players.p1 - palettesize);
            glVertex2f(-0.805, players.p1 - palettesize);
            glEnd();

            glColor4f(0.996f, 0.996f, 0.03f, 1.0f);
            glBegin(GL_POLYGON);
            glVertex2f(0.805, players.p2 + palettesize);
            glVertex2f(0.8, players.p2 + palettesize);
            glVertex2f(0.8, players.p2 - palettesize);
            glVertex2f(0.805, players.p2 - palettesize);
            glEnd();

            x += dx;
            y += dy;

            if ((x - sizex <= -0.8f && y + sizey >= players.p1 - palettesize && y - sizey <= players.p1 + palettesize && dx < 0) || (dx > 0 && x + sizex >= 0.8f && y + sizey >= players.p2 - palettesize && y - sizey <= players.p2 + palettesize))
            {
                playSound(sound);
                dx = dx * (-1);
                dy = ((rand() % 10) - 5.0) / 1000.0;
                bounces++;
            }
            if (bounces == 10)
            {
                dx *= 2.0;
                bounces = 0;
            }
            if (y + sizey >= 0.9f || y - sizey <= -0.9f)
                dy = dy * (-1);
            if (x + sizex >= 0.9f)
            {
                x = 0.0f;
                y = 0.0f;
                scores[0]++;
                dx = 0.004f * pow(-1.0, rand() % 2);
                dy = ((rand() % 1000) - 500) / 100000.0;

            }
            if (x - sizex <= -0.9f)
            {
                x = 0.0f;
                y = 0.0f;
                scores[1]++;
                dx = 0.004f * pow(-1.0, rand() % 2);
                dy = ((rand() % 1000) - 500) / 100000.0;
            }
            if (scores[0] == 1 || scores[1] == 1) {
                win = true;
            }
        }
        else {
            glColor4f(1.0, 0.563, 0.532, 1.0);
            DrawP(-0.3, 0.0);
            if (scores[0] > scores[1])
            {
                Draw1(-0.15, 0.0);
            }
            else
            {
                Draw2(-0.15, 0.0);
            }
            DrawWIN(0.1, 0.0);
        }


        drawScores(scores);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    quitMixer();
    glfwTerminate();
    return 0;
}