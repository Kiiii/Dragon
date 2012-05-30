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
    game->play.dragon_frame_x = 0; //pozycja x pliku z grafiką klatek animacji smoka//
    game->play.down = false;   //zmiana animacji smoka przy locie w dół//
    game->play.info = false;   //napis o ilości pozostałych żyć
    game->play.dragon_floured = false;   //napis o ilości pozostałych żyć
    game->play.fodder_x=0;
    game->play.fodder_mill_y=0;
    game->play.flour_y=0;
    game->play.i=0;  //licznik wykorzystany w Play_Game do ustawiania szybkości zmiany klatek animacji smoka//
    game->play.i2=0;  //licznik wykorzystany w Play_Game do ustawiania szybkości zmiany klatek animacji wiatraka//
    game->play.score=0;  //początkowa wartość punktów//
    game->play.score_villagers=0;
    game->play.score_sheeps=0;
    game->play.score_huts=0;
    game->play.dragon_pos_y = 176; //początkowa wysokość smoka
    game->play.dragon_pos_x = 127;   //stałe położenie smoka
    game->play.dragon_lives = 0;   //stałe położenie smoka
    Absolute_Free(game);
}
//==================================================== MAIN ========================================================//
int main(int argc, char **argv) {
    struct Game game;
    game.WIDTH = 948;
    game.HEIGHT = 593;
    bool FULLSCREEN = false;
    float FPS = 60;
    srand(time(NULL));
    game.gamestate=0;//stan gry -> 0 = menu; 1 = gra; 2 = wyniki//8
    game.fireonce=0;

    game.first=NULL; //fire
    game.prev=NULL;
    game.last=NULL;
    game.sfirst=NULL; //smoke
    game.sprev=NULL;
    game.ffirst=NULL; //fodder
    game.fprev=NULL;
    game.flast=NULL;
    game.ofirst=NULL; //obstacles -> widły i mąka
    game.oprev=NULL;

    New_Game(&game);
//////////////////////*  INICJALIZACJE ALLEGRO  *//////////////////////
        if(!al_init()){al_show_native_message_box(game.display, "Error", "Error", "Failed to initialize allegro!",NULL, ALLEGRO_MESSAGEBOX_ERROR); return 0; }
        if(!al_init_image_addon()){al_show_native_message_box(game.display, "Error", "Error", "Failed to initialize al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);return 0;}
        if(!al_install_audio()){fprintf(stderr, "failed to initialize audio!\n");return -1;}
        if(!al_init_acodec_addon()){fprintf(stderr, "failed to initialize audio codecs!\n");return -1;}
        if(!al_reserve_samples(1)){fprintf(stderr, "failed to reserve samples!\n");return -1;}
        if(!al_install_keyboard()){fprintf(stderr, "failed to initialize keyboard!\n");return -1;}
        if(!al_install_mouse()){fprintf(stderr, "failed to initialize mouse!\n");return -1;}
        if(!al_init_ttf_addon()){fprintf(stderr, "failed to initialize fonts!\n");return -1;}
        al_init_primitives_addon();
//////////////////////*    RÓŻNISTOŚCI  *//////////////////////
    game.queue = al_create_event_queue();                                   if(!game.queue) {fprintf(stderr, "failed to create event_queue!\n");al_destroy_display(game.display);return -1;}
    game.timer = al_create_timer(1.0 / FPS);                                if(!game.timer) {fprintf(stderr, "failed to create timer!\n");return -1;}
    if (FULLSCREEN) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    game.display = al_create_display(game.WIDTH, game.HEIGHT);              if(!game.display) {al_show_native_message_box(game.display, "Error", "Error", "Failed to initialize display!",NULL, ALLEGRO_MESSAGEBOX_ERROR); return 0;}
    game.menu.song = al_load_sample("resources/1_song.wav");                if(!game.menu.song) {fprintf(stderr, "Audio clip sample not loaded!\n" );exit(-1);}
   // al_play_sample(game.menu.song, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    game.sfont = al_load_ttf_font("resources/Dragon_Font.ttf",al_get_display_height(game.display)*0.0245,0 );
        al_register_event_source(game.queue, al_get_display_event_source(game.display));
        al_register_event_source(game.queue, al_get_timer_event_source(game.timer));
        al_register_event_source(game.queue, al_get_keyboard_event_source());
        al_register_event_source(game.queue, al_get_mouse_event_source());
        al_start_timer(game.timer);
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
    game.play.dragon = al_load_bitmap("resources/2_smok.png");
    game.play.trees1 = al_load_bitmap("resources/2_trees1.png");
    game.play.trees2 = al_load_bitmap("resources/2_trees2.png");

    al_hide_mouse_cursor(game.display);
//////////////////////*   GŁÓWNA PĘTLA   *//////////////////////
    while(1){
        ALLEGRO_EVENT event;
        al_wait_for_event(game.queue, &event);
        if((game.gamestate==0 || game.gamestate==2) && (event.type == ALLEGRO_EVENT_TIMER) && (al_is_event_queue_empty(game.queue))) {
            Menu_Draw(&game);   //odświeżanie grafiki menu
            al_flip_display();
        }
        if((game.gamestate==1) && (event.type == ALLEGRO_EVENT_TIMER) && (al_is_event_queue_empty(game.queue))) {
            Free_Fire(&game);
            Free_Fodder(&game);
            Free_Smoke(&game);
            Free_Obstacles(&game);
            game.play.score = game.play.score_villagers + game.play.score_sheeps + game.play.score_huts;
            Play_Game(&game);   //odświeżanie grafiki podczas gry
            Increment_All(&game);
            al_flip_display();
        }
        if((game.gamestate==1) && (al_is_event_queue_empty(game.queue)) && (rand() % 150 <2/*== 0*/) && game.play.dragon_lives>0)
            Create_Fodder(&game);   //losowe tworzenie wieśniaków

    /*  KLAWIATURA   */
        if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)    break;  //ESC - wyjście z gry
            if((event.keyboard.keycode == ALLEGRO_KEY_Q) && (game.gamestate==1 || game.gamestate==2))   {game.gamestate=0, Update_File(&game);}   //coś w rodzaju pauzy
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
            Fire_Bullet(&game); //wystrzelenie pocisku - dodanie elementu do listy jednokierunkowej//
            game.fireonce=1;
        }

        if((event.type == ALLEGRO_EVENT_KEY_UP) && game.gamestate==1){
            if((event.keyboard.keycode == ALLEGRO_KEY_DOWN) || (event.keyboard.keycode == ALLEGRO_KEY_S))
                game.play.down = false;     //powrót do normalnej animacji smoka, gdy puści się klawisz DOWN
            if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                game.fireonce=0; //zmienna pomocnicza, gdy strzela się spacją [uniknięcie ciągłości strzłu po naciśnięciu spacji]
        }
    /*  MYSZKA  */
        if(event.type == ALLEGRO_EVENT_MOUSE_AXES){  //aktualna pozycja kursora
            game.pos_x = event.mouse.x;
            game.pos_y = event.mouse.y; }
    //PRZYCISKI Z MENU//
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
/*START*/   if(((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.0685/*2005*/) && (game.pos_y<=al_get_display_height(game.display)*0.14/*272*/)) && (game.gamestate==0)) {
                if(event.mouse.button & 1)
                    New_Game(&game);
                    game.play.dragon_lives=3;
                    game.gamestate=1;   }   //albo z tego zrobić coś, żeby zaczynało nową grę
/*RESUME*/  else if(game.play.dragon_lives!=0 && ((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.2005/*3325*/) && (game.pos_y<=al_get_display_height(game.display)*0.272/*404*/)) && (game.gamestate==0)) {
                if(event.mouse.button & 1)
                    game.gamestate=1;  }    //to zmienić albo właśnie zostawić, jak jest
/*SCORES*/  else if(((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.3325/*4645*/) && (game.pos_y<=al_get_display_height(game.display)*0.404/*536*/)) && (game.gamestate==0))   {
                if(event.mouse.button & 1)
                    game.gamestate=2;
                    Update_File(&game);  }
/*QUIT*/    else if(((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.4645/*5925*/) && (game.pos_y<=al_get_display_height(game.display)*0.536/*668*/)) && (game.gamestate==0))   {
                if(event.mouse.button & 1)
                    break;  }
/*MENU*/    if(game.gamestate==2 && ((game.pos_x>=al_get_display_width(game.display)*0.67) && (game.pos_x<=al_get_display_width(game.display)*0.875)) && ((game.pos_y>=al_get_display_height(game.display)*0.4645/*5925*/) && (game.pos_y<=al_get_display_height(game.display)*0.536/*668*/)))
                if(event.mouse.button & 1)
                    game.gamestate=0;
    //POCISKI//
            if(game.play.dragon_floured==false && (game.gamestate == 1) && (event.mouse.button == 2) && (game.pos_x>game.play.dragon_pos_x) && game.play.dragon_lives>0)
                Fire_Bullet(&game); //wystrzelenie pocisku - dodanie elementu do listy jednokierunkowej//
        }
        if((game.gamestate==1) && (al_is_event_queue_empty(game.queue))){
            Burn_Fodder(&game);
            Collide_Obstacles(&game);
            //Free_Fire(&game);
           // Free_Fodder(&game);
           // Free_Smoke(&game);
        }
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
    al_destroy_timer(game.timer);
    al_destroy_sample(game.menu.song);
    al_destroy_event_queue(game.queue);
    al_destroy_font(game.sfont);
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
10. Z pitagorasa dopasować prędkość lotu pocisku.
*11. Zrobić trzy pliki .c i trzy nagłówkowe [main, menu, play].
*12. System wybuchów wywala czasem grę --> śle zrobione dodawanie do listy.
*13. Żywotność smoka - 3 serduszka; za nisko -1, widły -1, chmura mąki [niemożność strzelania, +1 piksel w dół przez czas trwania].
14. Muzyka i dźwięki podczas walki.
*15. Zmienić kursor.
*16. Odwrotnie dodawać na listę [najstaszy element=first].
*17. Dodawanie wyników - gdy się powtarza, do dopisuje jeszcze raz to samo.
18. Blond-pastuchy rzucają owczym defragmentatorem [na jakiejś wysokości wybuch i rozpirza na czter owce].
19. Przysick 'OPCJE' zamiast 'SCORED' i tam instrukcję gry też dać.

*/
