#ifndef BUTTON_H
#define BUTTON_H

#define KELL_BUTTON				PA6
#define VAJUTA_MIND_BUTTON		PA5
#define KELL_FR_BUTTON			PA4
#define KELL_BUTTON_PIN			PINA
#define VAJUTA_MIND_BUTTON_PIN	PINA
#define KELL_FR_BUTTON_PIN		PINA

#define kell_button_pressed				~(KELL_BUTTON_PIN | ~(1 << KELL_BUTTON))
#define vajuta_mind_button_pressed		~(VAJUTA_MIND_BUTTON_PIN | ~(1 << VAJUTA_MIND_BUTTON))
#define kell_fr_button_pressed			~(KELL_FR_BUTTON_PIN | ~(1 << KELL_FR_BUTTON))

bool kell_button_previously_pressed = false;
bool vajuta_mind_button_previously_pressed = false;
bool kell_fr_button_previously_pressed = false;

#endif

