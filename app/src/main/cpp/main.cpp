#include "raylib.h"
#include <sys/stat.h>

struct Jugador {
    Vector2 posicion;
    float velocidad;
    float velocidad_correr;
    float escala;
    
    float velocidadY;
    float sueloY;
    bool enElSuelo;

    Texture2D texturaQuieto;
    
    Image imagenGif;
    Texture2D texturaGif;
    int totalFrames;
    int frameActual;
    float temporizador;
    float velocidadAnim;
    bool mirandoDerecha;
};

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    
    InitWindow(800, 450, "Nahual");
    MaximizeWindow();
    SetTargetFPS(60);

    int anchoPantalla = GetScreenWidth();
    int altoPantalla = GetScreenHeight();

    Jugador player;
    player.escala = 1.5f;
    player.velocidad = 300.0f;
    player.velocidad_correr = 450.0f;
    player.mirandoDerecha = true;

    player.texturaQuieto = LoadTexture("Luis.png");
    SetTextureFilter(player.texturaQuieto, TEXTURE_FILTER_BILINEAR);

    player.totalFrames = 0;
    player.imagenGif = LoadImageAnim("MovimientoL.gif", &player.totalFrames);
    player.texturaGif = LoadTextureFromImage(player.imagenGif);
    SetTextureFilter(player.texturaGif, TEXTURE_FILTER_BILINEAR);
    
    player.frameActual = 0;
    player.temporizador = 0.0f;
    player.velocidadAnim = 0.08f;

    player.posicion = (Vector2){
        50.0f,
        (altoPantalla * (2.0f / 3.0f)) - ((player.texturaQuieto.height * player.escala) / 2.0f)
    };

    player.sueloY = player.posicion.y;
    player.velocidadY = 0.0f;
    player.enElSuelo = true;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        bool seMueve = false;
        
        float velocidadActual = player.velocidad;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            velocidadActual = player.velocidad_correr;
        }

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            player.posicion.x += velocidadActual * deltaTime;
            player.mirandoDerecha = true;
            seMueve = true;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            player.posicion.x -= velocidadActual * deltaTime;
            player.mirandoDerecha = false;
            seMueve = true;
        }
        
        float fuerzaSalto = -650.0f;
        float gravedad = 1500.0f;

        if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && player.enElSuelo) {
            player.velocidadY = fuerzaSalto;
            player.enElSuelo = false;
        }

        player.velocidadY += gravedad * deltaTime;
        player.posicion.y += player.velocidadY * deltaTime;

        if (player.posicion.y >= player.sueloY) {
            player.posicion.y = player.sueloY;
            player.velocidadY = 0.0f;
            player.enElSuelo = true;
        }

        if (seMueve && player.enElSuelo && player.totalFrames > 1) {
            player.temporizador += deltaTime;
            if (player.temporizador >= player.velocidadAnim) {
                player.temporizador = 0.0f;
                player.frameActual++;
                if (player.frameActual >= player.totalFrames) {
                    player.frameActual = 0;
                }
                unsigned int offsetBytes = player.imagenGif.width * player.imagenGif.height * 4 * player.frameActual;
                UpdateTexture(player.texturaGif, ((unsigned char*)player.imagenGif.data) + offsetBytes);
            }
        } else {
            player.frameActual = 0;
            player.temporizador = 0.0f;
        }

        Texture2D texturaActual = (seMueve && player.enElSuelo) ? player.texturaGif : player.texturaQuieto;
        float anchoTex = (float)texturaActual.width;
        float altoTex = (float)texturaActual.height;

        Rectangle sourceRec = {
            0.0f,
            0.0f,
            player.mirandoDerecha ? anchoTex : -anchoTex,
            altoTex
        };

        Rectangle destRec = {
            (float)((int)(player.posicion.x + 0.5f)),
            (float)((int)(player.posicion.y + 0.5f)),
            anchoTex * player.escala,
            altoTex * player.escala
        };
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("A/D o Flechas: Mover | Shift: Correr | W/Arriba: Saltar | ESC: Cerrar", 15, 15, 20, DARKGRAY);
            DrawTexturePro(texturaActual, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadTexture(player.texturaQuieto);
    UnloadTexture(player.texturaGif);
    UnloadImage(player.imagenGif);
    CloseWindow();
    
    return 0;
}


