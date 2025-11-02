#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#define VELIKOST_T_OBTIZNOST 200
#define VELIKOST_R 100

char *nullTerminate(const char *inp) {
    size_t len = strnlen(inp, 255);
    char *txt = (char *)malloc(len+1);
    if (txt==NULL || inp==NULL) return NULL;
    txt[len] = '\0';
    while (len--) txt[len]=inp[len];
    return txt;
}
Font LoadFontWithCodepointsFromText(const char *path, int fontSize, const char *text) {
    char *check = nullTerminate(text);
    if (check!=NULL)  {
        int count = 0;
        int * codepoints = LoadCodepoints(check, &count);
        Font result = LoadFontEx(path, fontSize, codepoints, count);
        UnloadCodepoints(codepoints);
        free(check);
        return result;
    }
    return GetFontDefault();
}

typedef enum Obtiznost {
    Easy,
    Normal,
    Hard
} Obtiznost;

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "KV War Simulator GUI");
    const char *znaky = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%'()*+,-./0123456789:;<=>?@[\\]^_`{|}~ÁČĎÉĚÍŇÓŘŠŤÚŮÝŽáčďéěíňóřšťúůýž";
    Font font = LoadFontWithCodepointsFromText("ttf/Hack-Regular.ttf", 20, znaky);
    InitAudioDevice();
    Music music = LoadMusicStream("kv_war_simulator_soundtrack.wav");
    PlayMusicStream(music);

    Rectangle easyTlacitko = { screenWidth/4.0f - VELIKOST_R/2, screenHeight/2.0f - VELIKOST_R/4, (float)VELIKOST_R, VELIKOST_R/2 };
    Rectangle normalTlacitko = { screenWidth/2.0f - VELIKOST_R/2, screenHeight/2.0f - VELIKOST_R/4, (float)VELIKOST_R, VELIKOST_R/2 };
    Rectangle hardTlacitko = { screenWidth/1.32f - VELIKOST_R/2, screenHeight/2.0f - VELIKOST_R/4, (float)VELIKOST_R, VELIKOST_R/2 };

    Vector2 mousePos;
    bool hra = false;
    Obtiznost obtiznost;
    
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, easyTlacitko) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            obtiznost = Easy;
            hra = true;
        }
        if (CheckCollisionPointRec(mousePos, normalTlacitko) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            obtiznost = Normal;
            hra = true;
        }
        if (CheckCollisionPointRec(mousePos, hardTlacitko) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            obtiznost = Hard;
            hra = true;
        }

        BeginDrawing();
            ClearBackground(GRAY);
            if (hra) {
                DrawTextEx(font, u8"Hlavní obrazovka", (Vector2) { screenWidth/2-VELIKOST_T_OBTIZNOST*1.2, screenHeight/2-VELIKOST_T_OBTIZNOST }, VELIKOST_T_OBTIZNOST/6, 0, WHITE);
                EndDrawing();
                continue;
            }
            DrawTextEx(font, u8"Na jakou chceš hrát obtížnost?", (Vector2) { screenWidth/2-VELIKOST_T_OBTIZNOST*1.2, screenHeight/2-VELIKOST_T_OBTIZNOST }, VELIKOST_T_OBTIZNOST/6, 0, WHITE);
            DrawRectangleRec(easyTlacitko, GREEN);
            DrawRectangleRec(normalTlacitko, YELLOW);
            DrawRectangleRec(hardTlacitko, RED);
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    return 0;
}
