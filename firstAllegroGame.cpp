/*
* This is a working project in learning how to use Allegro with C++
* NOTES:
* Allegro is a library that is considered a use what you pay for program.
*	Allegro is a modular library and does not install all the aspects of the
*	library without the programmer specifing which tools they want to use.
*
* The sprite sheet I decided to use was more advanced than anticipated, I will temporarily
* put this project on hold. I may will comeback to this project once I have a better understanding
* of animations.
* 
* FUTURE GOAL:
* Finish implementing all the animations in the Alucard sprite sheet if possible.
*/
// Setting up relevant allegro headers
#include "stdafx.h"
#include "objects.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

// Prototype functions
void InitHero(Hero &hero, int height, int width);

int main() {
	//Allegro macro types
	ALLEGRO_DISPLAY *display;
	// Input queue
	ALLEGRO_EVENT_QUEUE *queue;
	// Sprite being used
	ALLEGRO_BITMAP *bitmap = NULL;
	//Used to time game frame rate
	ALLEGRO_TIMER *timer = NULL;

	//initializing Allegro
	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "Failed to initialize allegro,", NULL, NULL);

		return 0;
	};

	//create display
	display = al_create_display(640, 480);
	if (!display) {
		al_show_native_message_box(NULL, NULL, NULL, "Failed to initialize display", NULL, NULL);
		al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_bitmap(bitmap);
		
		return 0;
	}

	//Set to 60 FPS
	timer = al_create_timer(1.0 / 60.0);
	if (!timer) {
		al_show_native_message_box(NULL, NULL, NULL, "Failed to initialize timer", NULL, NULL);
		//Cleaning up memory before exiting program
		//al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_bitmap(bitmap);
		
		return 0;
	}
	//Addons for tools being used for this program
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT *default_font = al_load_font("fonts/ARIALN.ttf", 12, 0);

	queue = al_create_event_queue();

	//Registering all event sources
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	// Allows for timer to trigger events allowing for consistent rendering and actions
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bitmap = al_load_bitmap("images/exampleIdleAnimation.png");
	assert(bitmap != NULL);

	//Starting game contents here
	bool running = true, redraw = true;
	float x = 210, y = 270;

	int width = al_get_display_width(display);
	int height = al_get_display_height(display);

	// Object variables
	//Hero hero;
	//Object functions
	//InitHero(hero, al_get_bitmap_height(bitmap), al_get_bitmap_width(bitmap));

	// Animation variables
	int maxFrame = 5, loopFrame = -1, curFrame = 3, scale = 1, check = 0;
	int frameCount = 0, frameDelay = 10, frameWidth = 60, frameHeight = 48;
	int idleExtendFrameCount = 0, idleExtendFrameMaxCount = 30;
	int curWalkFrame = 0, maxWalkFrames = 31, loopWalkFrame = 16, curIdleFrame = 3, curTransitionFrame = 0;
	bool loopBack = false, idleAnim = true, walkAnim = false, transition = false;//attackAnim = false;
	float frameSourceY = 0;

	//Start frame timer
	al_start_timer(timer);

	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;

		if (!al_is_event_queue_empty(queue)) {
			al_wait_for_event(queue, &event);
			//If display is closed, game is code stops running
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				running = false;
			}

			//Check for bitmap off bound loop
			if (x > width) {
				x = -frameWidth;
			}

			if (x < -frameWidth) {
				x = width;
			}

			//if (y > height) {
			//	y = -al_get_bitmap_height(bitmap);
			//}

			//if (y < -al_get_bitmap_height(bitmap)) {
			//	y = height - 1;
			//}

			/* 
			 * Only poll keyboard inputs every 1/60 of a second
			 * Allows for smooth input actions
			 * Rendering isn't added her because you want to keep event handler runtimes as short as possible
			 * Rendering to the screen is separated because rendering is the most intensive process
			 * of any games. As a result, rendering is a lenghty process and other process is can become
			 * delayed. A player will not notice a slight dip in rendering speed, but will notice a dip in input speeds.
			 */
			if (event.type == ALLEGRO_EVENT_TIMER) {
				redraw = true;

				// Actively poll the keyboard
				al_get_keyboard_state(&keyState);
				if (al_key_down(&keyState, ALLEGRO_KEY_DOWN)) {
					idleAnim = false;
					y += 10;
				}
				else if (al_key_down(&keyState, ALLEGRO_KEY_UP)) {
					idleAnim = false;
					y -= 10;
				}
				else if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
					idleAnim = false;
					walkAnim = true;
					scale = 1;
				}
				else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
					idleAnim = false;
					walkAnim = true;
					scale = -1;
				}
				else if (al_key_down(&keyState, NULL) == NULL) {
					if (walkAnim) {
						walkAnim = false;
						transition = true;
						curFrame = 0;
					}
					//idleAnim = true;
				}

				// Trigger Animation using space key
				//if (al_key_down(&keyState, ALLEGRO_KEY_SPACE) && !attackAnim) {
					//attackAnim = true;
				//}

				// When trigger, the attack animation occurs
				//if (attackAnim) {
					// The bitmap sheet used was not all the same dimensions, as a result
					// I needed to adjust certain sprites so that the animation is more
					// fluid
				
				// The idle animation improves upon the hellfire animation by adjusting the
				// bitmap sprite sheet so that all the sprites had the same dimensions in the set
				if (transition) {
					frameHeight = 50;
					frameSourceY = 100;
					if (++frameCount >= 4) {
						if (++curTransitionFrame >= 13) {
							curTransitionFrame = 0;
							transition = false;
							idleAnim = true;
							frameDelay = 10;
							x += 5 * scale;
							curFrame = 2;
							maxFrame = 5;
							idleExtendFrameCount = 0;
						}
						else {
							curFrame = curTransitionFrame;
						}
						frameCount = 0;
					}
				}

				if (idleAnim) {
					frameHeight = 48;
					frameSourceY = 0;
					//frameDelay = 10;

					curWalkFrame = 0;
					if (++frameCount >= frameDelay) {
						// Looping back the animation so sprite looks more lively rather than 
						// playing like a slideshow
						if (loopBack) {
							if (--curIdleFrame <= loopFrame) {
								loopBack = false;
								curIdleFrame += 2;
							}
						}
						else {
							if (++curIdleFrame >= maxFrame) {
								// Setup looping
								curIdleFrame -= 2;
								loopBack = true;
							}
						}

						// Once character has been idle for enough frames, extend the idle animation
						if (idleExtendFrameCount == idleExtendFrameMaxCount) {
							maxFrame = 14;
							// Hold the final sprite frame for extend time
							if (curIdleFrame == 13) {
								frameDelay = 30;
							}
							// Speed up at the start of the arm movement for smoother animation
							// NOTES:
							//	The seventh animation sprite needed to be push back for a cleaner
							//	stepping animation, or else it looks clunky
							else if (curIdleFrame == 6 || curIdleFrame == 12) {
								frameDelay = 7;
							}
							// When the extended animation is finished, reset back to normal idle
							// animation
							else if (curIdleFrame == 5 && loopBack) {
								frameDelay = 10;
								idleExtendFrameCount = -1;
								maxFrame = 5;
							}
						}
						else {
							++idleExtendFrameCount;
						}
						curFrame = curIdleFrame;
						frameCount = 0;
					}
				}

				// Testing Walk Animation
				if (walkAnim) {
					frameHeight = 52;
					frameSourceY = 48;

					if (++frameCount >= 3) {
						if (++curWalkFrame >= maxWalkFrames) {
							curWalkFrame = loopWalkFrame;
						}
						frameCount = 0;
						curFrame = curWalkFrame;
						//x += (5 * scale);
					}
				}
				//}
			}

			// Rendering occurs when no other processes are waiting and has been flagged for redraw
			if (redraw && al_is_event_queue_empty(queue)) {
				redraw = false;

				//Generating display
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_textf(default_font, al_map_rgb(0, 0, 0), width / 2, height / 2, ALLEGRO_ALIGN_CENTER, "Animation Frame: %d", curFrame);
				//al_draw_bitmap_region(bitmap, curFrame * frameWidth, frameSourceY, frameWidth, frameHeight, x, y, 0);
				al_draw_tinted_scaled_rotated_bitmap_region(bitmap, curFrame * frameWidth, frameSourceY, frameWidth, frameHeight, al_map_rgb(255, 255, 255), frameWidth/2, frameHeight/2, x, y, scale, 1, 0, 0);
				//al_draw_bitmap(bitmap, hero.x, hero.y, 0);
				al_flip_display();
			}
		}
	}

	al_stop_timer(timer);

	//Cleaning up memory
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_destroy_bitmap(bitmap);
	al_destroy_font(default_font);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}

// Helper functions
void InitHero(Hero &hero, int height, int width) {
	hero.ID = Player;
	hero.x = 100;
	hero.y = 100;
	hero.lives = 4;
	hero.speed = 10;
	hero.boundx = 10;
	hero.boundy = 10;
}