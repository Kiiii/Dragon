#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "main.h"
#include "lists.h"
#include "play.h"
#include "menu.h"

//====================================================  NEW_GAME ========================================================//
void New_Game(struct Game *game) {
    game->menu.cldpos1=0;
    game->menu.cldpos2=1000;
    game->menu.cldpos3=0;
    game->menu.mills_y=0;
    game->play.for_mtn1 = 0;
    game->play.for_mtn2 = 948;
    game->play.for_grs1 = 0;
    game->play.for_grs2 = 950;
    game->play.dragon_frame_x = 0; //pozycja x pliku z grafiką klatek animacji smoka
    game->play.down = false;   //zmiana animacji smoka przy locie w dół
    game->play.info = false;   //napis o ilości pozostałych żyć
    game->play.dragon_floured = false;   //napis o ilości pozostałych żyć
    game->play.fodder_x=0;
    game->play.fodder_mill_y=0;
    game->play.flour_y=0;
    game->play.i=0;  //licznik wykorzystany w Play_Game do ustawiania szybkości zmiany klatek animacji smoka
    game->play.i2=0;  //licznik wykorzystany w Play_Game do ustawiania szybkości zmiany klatek animacji wiatraka
    game->play.score=0;  //początkowa wartość punktów
    game->play.score_villagers=0;
    game->play.score_sheeps=0;
    game->play.score_huts=0;
    game->play.dragon_pos_y = 176; //początkowa wysokość smoka
    game->play.dragon_pos_x = 127;   //stałe położenie smoka
    game->play.dragon_lives = 0;   //smocze życia
    Absolute_Free(game);
}
//==================================================== MAIN ========================================================//
int main(int argc, char **argv) {
    struct Game game;
    bool FULLSCREEN = false;
    float FPS = 60;
    game.WIDTH = 948; game.HEIGHT = 593;
    srand(time(NULL));  game.fireonce=0;
    game.gamestate=0;
    game.first=NULL;  game.prev=NULL;      game.ffirst=NULL; game.fprev=NULL;
    game.sfirst=NULL; game.sprev=NULL;     game.ofirst=NULL; game.oprev=NULL;
    New_Game(&game);
//////////////////////*  INICJALIZACJE ALLEGRO  *//////////////////////
        if(!al_init()){al_show_native_message_box(game.display, "Error", "Error", "Failed to initialize allegro!",NULL, ALLEGRO_MESSAGEBOX_ERROR); return 0; }
        if(!al_init_image_addon()){al_show_native_message_box(game.display, "Error", "Error", "Failed to initialize al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);return 0;}
        if(!al_install_audio()){fprintf(stderr, "failed to initialize audio!\n");return -1;}
        if(!al_init_acodec_addon()){fprintf(stderr, "failed to initialize audio codecs!\n");return -1;}
        if(!al_install_keyboard()){fprintf(stderr, "failed to initialize keyboard!\n");return -1;}
        if(!al_install_mouse()){fprintf(stderr, "failed to initialize mouse!\n");return -1;}
        if(!al_init_ttf_addon()){fprintf(stderr, "failed to initialize fonts!\n");return -1;}
        al_init_primitives_addon();
//////////////////////*    RÓŻNISTOŚCI  *//////////////////////
    game.queue = al_create_event_queue();                                   if(!game.queue) {fprintf(stderr, "failed to create event_queue!\n");al_destroy_display(game.display);return -1;}
    game.timer = al_create_timer(1.0 / FPS);                                if(!game.timer) {fprintf(stderr, "failed to create timer!\n");return -1;}
    if (FULLSCREEN) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    game.display = al_create_display(game.WIDTH, game.HEIGHT);              if(!game.display) {al_show_native_message_box(game.display, "Error", "Error", "Failed to initialize display!",NULL, ALLEGRO_MESSAGEBOX_ERROR); return 0;}
    game.sfont = al_load_ttf_font("resources/Dragon_Font.ttf",al_get_display_height(game.display)*0.0245,0);
    al_register_event_source(game.queue, al_get_display_event_source(game.display));    al_register_event_source(game.queue, al_get_timer_event_source(game.timer));    al_register_event_source(game.queue, al_get_keyboard_event_source());   al_register_event_source(game.queue, al_get_mouse_event_source());
    al_start_timer(game.timer);
    al_hide_mouse_cursor(game.display);
//////////////////////*   MUZYKA I INNE DŹWIĘKI   *//////////////////////
    game.sfxvol=0.3;
    game.musvol=0.25;
    game.voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    game.mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    game.mixer_music = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    game.mixer_sfx = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    game.mixer_sfx2 = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    al_attach_mixer_to_voice(game.mixer, game.voice); //gówny mixer do voica
    al_attach_mixer_to_mixer(game.mixer_music, game.mixer); //mixer muzyki do głównego mixera
    al_attach_mixer_to_mixer(game.mixer_sfx, game.mixer); //mixer efektów do głównego mixera
    al_attach_mixer_to_mixer(game.mixer_sfx2, game.mixer); //mixer efektów do głównego mixera

    game.menu.song = al_load_sample("resources/1_song.wav"); //muzyka z menu
    game.menu.click1 = al_load_sample("resources/1_click1.wav"); //zwykły klik
    game.menu.click2 = al_load_sample("resources/1_click2.wav"); //wychodząc klik
    game.play.psong = al_load_sample("resources/2_song.wav"); //muzyka z gry
   // game.play.expl = al_load_sample("resources/2_expl.wav"); //bum!
    game.play.flap1 = al_load_sample("resources/2_flap1.wav"); //flap!
    game.play.flap2 = al_load_sample("resources/2_flap2.wav"); //flap!
    game.play.flap3 = al_load_sample("resources/2_flap3.wav"); //flap!
    game.play.flap4 = al_load_sample("resources/2_flap4.wav"); //flap!
    game.play.die = al_load_sample("resources/2_die.wav"); //ryk padającego smoka
    game.play.hit1 = al_load_sample("resources/2_hit1.wav"); //warknięcie obrywającego smoka
    game.play.hit2 = al_load_sample("resources/2_hit2.wav"); //warknięcie obrwającego smoka
    //game.play.hutexpl = al_load_sample("resources/2_hutexpl.wav"); //wybuch domku
    game.play.whoosh1 = al_load_sample("resources/2_whoosh1.wav"); //rzut widłami
    game.play.whoosh2 = al_load_sample("resources/2_whoosh2.wav"); //rzut widłami
    game.play.lfire = al_load_sample("resources/2_launchfire.wav"); //zionięcie
    game.play.sheep = al_load_sample("resources/2_sheep.wav"); //zionięcie
    game.play.hiss = al_load_sample("resources/2_hiss.wav"); //zionięcie

    game.menu.in_song = al_create_sample_instance(game.menu.song);        al_attach_sample_instance_to_mixer(game.menu.in_song, game.mixer_music);         al_set_sample_instance_gain(game.menu.in_song, game.musvol-0.05);        al_set_sample_instance_playmode(game.menu.in_song, ALLEGRO_PLAYMODE_LOOP);
    game.menu.in_click1 = al_create_sample_instance(game.menu.click1);        al_attach_sample_instance_to_mixer(game.menu.in_click1, game.mixer_sfx);        al_set_sample_instance_gain(game.menu.in_click1, game.sfxvol+0.1);
    game.menu.in_click2 = al_create_sample_instance(game.menu.click2);        al_attach_sample_instance_to_mixer(game.menu.in_click2, game.mixer_sfx);        al_set_sample_instance_gain(game.menu.in_click2, game.sfxvol+0.1);

    game.play.in_psong = al_create_sample_instance(game.play.psong);        al_attach_sample_instance_to_mixer(game.play.in_psong, game.mixer_music);         al_set_sample_instance_gain(game.play.in_psong, game.musvol+0.2);        al_set_sample_instance_playmode(game.play.in_psong, ALLEGRO_PLAYMODE_LOOP);
    //game.play.in_expl = al_create_sample_instance(game.play.expl);        al_attach_sample_instance_to_mixer(game.play.in_expl, game.mixer_sfx2);        al_set_sample_instance_gain(game.play.in_expl, game.sfxvol);
    game.play.in_flap1 = al_create_sample_instance(game.play.flap1);        al_attach_sample_instance_to_mixer(game.play.in_flap1, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_flap1, game.sfxvol);
    game.play.in_flap2 = al_create_sample_instance(game.play.flap2);        al_attach_sample_instance_to_mixer(game.play.in_flap2, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_flap2, game.sfxvol);
    game.play.in_flap3 = al_create_sample_instance(game.play.flap3);        al_attach_sample_instance_to_mixer(game.play.in_flap3, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_flap3, game.sfxvol);
    game.play.in_flap4 = al_create_sample_instance(game.play.flap4);        al_attach_sample_instance_to_mixer(game.play.in_flap4, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_flap4, game.sfxvol);
    game.play.in_die = al_create_sample_instance(game.play.die);        al_attach_sample_instance_to_mixer(game.play.in_die, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_die, game.sfxvol);
    game.play.in_hit1 = al_create_sample_instance(game.play.hit1);        al_attach_sample_instance_to_mixer(game.play.in_hit1, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_hit1, game.sfxvol);
    game.play.in_hit2 = al_create_sample_instance(game.play.hit2);        al_attach_sample_instance_to_mixer(game.play.in_hit2, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_hit2, game.sfxvol);
   // game.play.in_hutexpl = al_create_sample_instance(game.play.hutexpl);        al_attach_sample_instance_to_mixer(game.play.in_hutexpl, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_hutexpl, game.sfxvol);
    game.play.in_whoosh1 = al_create_sample_instance(game.play.whoosh1);        al_attach_sample_instance_to_mixer(game.play.in_whoosh1, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_whoosh1, game.sfxvol);
    game.play.in_whoosh2 = al_create_sample_instance(game.play.whoosh2);        al_attach_sample_instance_to_mixer(game.play.in_whoosh2, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_whoosh2, game.sfxvol);
    game.play.in_lfire = al_create_sample_instance(game.play.lfire);        al_attach_sample_instance_to_mixer(game.play.in_lfire, game.mixer_sfx2);        al_set_sample_instance_gain(game.play.in_lfire, game.sfxvol);
    game.play.in_sheep = al_create_sample_instance(game.play.sheep);        al_attach_sample_instance_to_mixer(game.play.in_sheep, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_sheep, game.sfxvol);
    game.play.in_hiss = al_create_sample_instance(game.play.hiss);        al_attach_sample_instance_to_mixer(game.play.in_hiss, game.mixer_sfx);        al_set_sample_instance_gain(game.play.in_hiss, game.sfxvol);

//////////////////////*   INICJALIZACJA BITMAP   *//////////////////////
    game.menu.buttons = al_load_bitmap("resources/1_buttons.png");
    game.menu.cloud = al_load_bitmap("resources/1_clouds.png");
    game.menu.cursor = al_load_bitmap("resources/1_cursor.png");
    game.menu.mills = al_load_bitmap("resources/1_mills.png");
    game.menu.menu_bitmap = al_load_bitmap("resources/1_menu.png");
    game.menu.smok = al_load_bitmap("resources/1_smok.png");
    game.play.background = al_load_bitmap("resources/2_background.png");
    game.play.fire = al_load_bitmap("resources/2_fire.png");
    game.play.fodder = al_load_bitmap("resources/2_fodder.png");
    game.play.grass1 = al_load_bitmap("resources/2_grs1.png");
    game.play.grass2 = al_load_bitmap("resources/2_grs2.png");
    game.play.mtn1 = al_load_bitmap("resources/2_mtn1.png");
    game.play.mtn2 = al_load_bitmap("resources/2_mtn2.png");
    game.play.pitchfork = al_load_bitmap("resources/2_pitchfork.png");
    game.play.score_table = al_load_bitmap("resources/2_score.png");
    game.play.sheep_bmp = al_load_bitmap("resources/2_sheep.png");
    game.play.dragon = al_load_bitmap("resources/2_smok.png");
    game.play.trees1 = al_load_bitmap("resources/2_trees1.png");
    game.play.trees2 = al_load_bitmap("resources/2_trees2.png");

//////////////////////*   GŁÓWNA PĘTLA   *//////////////////////
    while(1){
        ALLEGRO_EVENT event;
        al_wait_for_event(game.queue, &event);
/*MENU*/if((game.gamestate==0 || game.gamestate==2) && (event.type == ALLEGRO_EVENT_TIMER) && (al_is_event_queue_empty(game.queue))) {
            Menu_Draw(&game);   //odświeżanie grafiki menu
            Increment_All(&game);
            al_flip_display();
        }
/*GRA*/ if((game.gamestate==1) && (event.type == ALLEGRO_EVENT_TIMER) && (al_is_event_queue_empty(game.queue))) {
            Free_Fire(&game);
            Free_Fodder(&game);
            Free_Smoke(&game);
            Free_Obstacles(&game);
            game.play.score = game.play.score_villagers + game.play.score_sheeps + game.play.score_huts;
            Play_Game(&game);   //odświeżanie grafiki podczas gry
            Increment_All(&game);
            al_flip_display();
        }
        if((game.gamestate==1) && (al_is_event_queue_empty(game.queue)) && (rand() % 150 <2) && game.play.dragon_lives>0)
            Create_Fodder(&game);   //losowe tworzenie wieśniaków

    // KLAWIATURA //
        if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)    break;  //ESC - wyjście z gry
            if((event.keyboard.keycode == ALLEGRO_KEY_Q) && (game.gamestate==1 || game.gamestate==2)) {    //coś w rodzaju pauzy [wyjście do menu]
                if(game.gamestate==1)
                    Update_File(&game);
                game.gamestate=0;
                al_play_sample_instance(game.menu.in_click2);
            }
        }
        al_get_keyboard_state(&game.keyboard);
        if (game.play.dragon_floured==false && (al_key_down(&game.keyboard, ALLEGRO_KEY_UP) || al_key_down(&game.keyboard, ALLEGRO_KEY_W)) && game.gamestate==1 && (event.type == ALLEGRO_EVENT_TIMER) && game.play.dragon_pos_y>-80){
            game.play.dragon_pos_y -=5; //UP - poruszanie smoka w górę
        }
        if (game.play.dragon_floured==false && (al_key_down(&game.keyboard, ALLEGRO_KEY_DOWN)  || al_key_down(&game.keyboard, ALLEGRO_KEY_S)) && game.gamestate==1 && (event.type == ALLEGRO_EVENT_TIMER) && game.play.dragon_pos_y<(al_get_display_height(game.display)-150/*230*/)){
            game.play.dragon_pos_y +=5; //DOWN - poruszanie smoka w dół
            game.play.down = true;      //zmiana animacji podczas zniżania lotu
        }
        if(game.play.dragon_floured==false && al_key_down(&game.keyboard, ALLEGRO_KEY_SPACE) && (game.gamestate == 1) && (game.fireonce==0) && (game.pos_x>game.play.dragon_pos_x) && game.play.dragon_lives>0) {
/*STRZAŁ*/  Fire_Bullet(&game); //wystrzelenie pocisku - dodanie elementu do listy jednokierunkowej
            game.fireonce=1;
            //Add_Obstacles(&game, 848, 553, 3, 0, 0);
            if(rand() % 100 < 50){ //dźwięk wystrzału
                game.play.in_lfire = al_create_sample_instance(game.play.lfire);   al_attach_sample_instance_to_mixer(game.play.in_lfire, game.mixer_sfx2);   al_set_sample_instance_gain(game.play.in_lfire, game.sfxvol-0.1);   al_play_sample_instance(game.play.in_lfire);   }
            else al_play_sample_instance(game.play.in_lfire);
        }
        if((event.type == ALLEGRO_EVENT_KEY_UP) && game.gamestate==1){
            if((event.keyboard.keycode == ALLEGRO_KEY_DOWN) || (event.keyboard.keycode == ALLEGRO_KEY_S))     //powrót do normalnej animacji smoka, gdy puści się klawisz DOWN
                game.play.down = false;
            if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)     //zmienna pomocnicza, gdy strzela się spacją [uniknięcie ciągłości strzAłu po naciśnięciu spacji]
                game.fireonce=0;
        }
    // MYSZKA //
        if(event.type == ALLEGRO_EVENT_MOUSE_AXES){  //aktualna pozycja kursora
            game.pos_x = event.mouse.x;
            game.pos_y = event.mouse.y; }
    // PRZYCISKI Z MENU //
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
/*START*/   if(((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.0685/*2005*/) && (game.pos_y<=al_get_display_height(game.display)*0.14/*272*/)) && (game.gamestate==0)) {
                if(event.mouse.button & 1){
                    New_Game(&game);
                    game.play.dragon_lives=3;
                    game.gamestate=1;
                    al_stop_sample_instance(game.menu.in_song);
                    al_stop_sample_instance(game.play.in_psong);
                    al_play_sample_instance(game.menu.in_click1);
                } }
/*RESUME*/  else if(game.play.dragon_lives!=0 && ((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.2005/*3325*/) && (game.pos_y<=al_get_display_height(game.display)*0.272/*404*/)) && (game.gamestate==0)) {
                if(event.mouse.button & 1){
                    game.gamestate=1;
                    al_play_sample_instance(game.menu.in_click1);
                    al_stop_sample_instance(game.menu.in_song);
                } }
/*OPTIONS*/ else if(((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.3325/*4645*/) && (game.pos_y<=al_get_display_height(game.display)*0.404/*536*/)) && (game.gamestate==0))   {
                if(event.mouse.button & 1){
                    game.gamestate=2;
                    Update_File(&game);
                    al_play_sample_instance(game.menu.in_click1);
                } }
/*QUIT*/    else if(((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.4645/*5925*/) && (game.pos_y<=al_get_display_height(game.display)*0.536/*668*/)) && (game.gamestate==0))   {
                if(event.mouse.button & 1){
                    al_play_sample_instance(game.menu.in_click1);
                    al_rest(0.1);
                    al_play_sample_instance(game.menu.in_click2);
                    al_rest(0.7);
                    break;
                } }
/*MENU*/    if(game.gamestate==2 && ((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.4645/*5925*/) && (game.pos_y<=al_get_display_height(game.display)*0.536/*668*/))) {
                if(event.mouse.button & 1){
                    game.gamestate=0;
                    al_play_sample_instance(game.menu.in_click1);
                } }
    // POCISKI //
            if(game.play.dragon_floured==false && (game.gamestate == 1) && (event.mouse.button == 2) && (game.pos_x>game.play.dragon_pos_x) && game.play.dragon_lives>0){
                Fire_Bullet(&game); //wystrzelenie pocisku - dodanie elementu do listy jednokierunkowej
                if(rand() % 100 < 50){ //dźwięk wystrzału
                    game.play.in_lfire = al_create_sample_instance(game.play.lfire);  al_attach_sample_instance_to_mixer(game.play.in_lfire, game.mixer_sfx2);  al_set_sample_instance_gain(game.play.in_lfire, game.sfxvol-0.1);  al_set_sample_instance_playmode(game.play.in_lfire,ALLEGRO_PLAYMODE_ONCE);  al_play_sample_instance(game.play.in_lfire);    }
                else al_play_sample_instance(game.play.in_lfire);
            }
        }
        if((game.gamestate==1) && (al_is_event_queue_empty(game.queue))){ //sprawdzanie kolizji pocisków z fodderami i smoka z przeszkodami latającymi
            Burn_Fodder(&game);
            Collide_Obstacles(&game);
        }
    // MUZYCZKA //
        if(game.gamestate!=1 && game.play.dragon_lives==0){
            al_play_sample_instance(game.menu.in_song);
            al_stop_sample_instance(game.play.in_psong);
        }
        else if(game.gamestate==1)
            al_play_sample_instance(game.play.in_psong);
    }
//////////////////////*   USUWANIE   *//////////////////////
    al_destroy_display(game.display);
    al_destroy_bitmap(game.menu.menu_bitmap);
    al_destroy_bitmap(game.menu.cloud);
    al_destroy_bitmap(game.menu.cursor);
    al_destroy_bitmap(game.menu.mills);
    al_destroy_bitmap(game.menu.buttons);
    al_destroy_bitmap(game.menu.smok);
    al_destroy_bitmap(game.play.background);
    al_destroy_bitmap(game.play.trees1);
    al_destroy_bitmap(game.play.trees2);
    al_destroy_bitmap(game.play.grass1);
    al_destroy_bitmap(game.play.grass2);
    al_destroy_bitmap(game.play.mtn1);
    al_destroy_bitmap(game.play.mtn2);
    al_destroy_bitmap(game.play.pitchfork);
    al_destroy_bitmap(game.play.dragon);
    al_destroy_bitmap(game.play.fire);
    al_destroy_bitmap(game.play.fodder);
    al_destroy_bitmap(game.play.score_table);
    al_destroy_bitmap(game.play.sheep_bmp);
    al_destroy_timer(game.timer);

    al_destroy_sample(game.menu.song);     al_destroy_sample_instance(game.menu.in_song);
    al_destroy_sample(game.menu.click1);   al_destroy_sample_instance(game.menu.in_click1);
    al_destroy_sample(game.menu.click2);   al_destroy_sample_instance(game.menu.in_click2);
    al_destroy_sample(game.play.psong);    al_destroy_sample_instance(game.play.in_psong);
    al_destroy_sample(game.play.flap1);    al_destroy_sample_instance(game.play.in_flap1);
    al_destroy_sample(game.play.flap2);    al_destroy_sample_instance(game.play.in_flap2);
    al_destroy_sample(game.play.flap3);    al_destroy_sample_instance(game.play.in_flap3);
    al_destroy_sample(game.play.flap4);    al_destroy_sample_instance(game.play.in_flap4);
    al_destroy_sample(game.play.die);      al_destroy_sample_instance(game.play.in_die);
    al_destroy_sample(game.play.hit1);     al_destroy_sample_instance(game.play.in_hit1);
    al_destroy_sample(game.play.hit2);     al_destroy_sample_instance(game.play.in_hit2);
    al_destroy_sample(game.play.whoosh1);     al_destroy_sample_instance(game.play.in_whoosh1);
    al_destroy_sample(game.play.whoosh2);     al_destroy_sample_instance(game.play.in_whoosh2);
    al_destroy_sample(game.play.lfire);     al_destroy_sample_instance(game.play.in_lfire);
    al_destroy_sample(game.play.sheep);     al_destroy_sample_instance(game.play.in_sheep);
    al_destroy_sample(game.play.hiss);     al_destroy_sample_instance(game.play.in_hiss);

    al_destroy_mixer(game.mixer);
    al_destroy_mixer(game.mixer_sfx);
    al_destroy_mixer(game.mixer_sfx2);
    al_destroy_mixer(game.mixer_music);
    al_destroy_voice(game.voice);

    al_destroy_event_queue(game.queue);
    al_destroy_font(game.sfont);

    al_uninstall_audio();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
return 0;
}

/*

//TO DO//
*1.  Niemożność dodawania nowych pocisków do listy, gdy last zostanie usunięty jako pierwszy (lub po prostu nie jako ostatni).
*2.  Dopracować celność (czasem nie nadąża ze sprawdzeniem dla każdego wieśniaka, czy akurat pocisk go walnął, bo pocisk zdąży się usunąć z listy).
*3.  Zapisywanie wyników.
*4.  Dodatkowy przycisk - options (tam będzie wyłączanie muzyki, instrukcja obsługi itp.).
*5.  Rozdzielenie wyników (ikonka wieźniaka (x2,x1), owcy (x3), domku (x5) i punkty przy każdym; obok gdzieś suma wszystkich punktów).
*6.  Randomować fodder (nowe grafiki).
7.  Special_Encounter(&game) i tam dwa if'y dla wartości jakiejś odpowiedzialnej za to zmiennej (dla owcy i zamku).
*8.  Inna czcionka.
*9.  Żywotność domków - 3 strzały.
*10. Z pitagorasa dopasować prędkość lotu pocisku.
*11. Zrobić trzy pliki .c i trzy nagłówkowe [main, menu, play].
*12. System wybuchów wywala czasem grę --> śle zrobione dodawanie do listy.
*13. Żywotność smoka - 3 serduszka; za nisko -1, widły -1, chmura mąki [niemożność strzelania, +1 piksel w dół przez czas trwania].
*14. Muzyka i dźwięki podczas walki.
*15. Zmienić kursor.
*16. Odwrotnie dodawać na listę [najstaszy element=first].
*17. Dodawanie wyników - gdy się powtarza, do dopisuje jeszcze raz to samo.
*18. Blond-pastuchy rzucają owczym defragmentatorem [na jakiejś wysokości wybuch i rozpirza na czter owce].
*19. Przysick 'OPCJE' zamiast 'SCORED' i tam instrukcję gry też dać.

*/




/*    al_set_mixer_gain(game.mixer, 50.0); //od wersji 5.1 Allegro
    al_set_mixer_gain(game.mixer_music, 0.5);
    al_set_mixer_gain(game.mixer_sfx, 0.5); */
