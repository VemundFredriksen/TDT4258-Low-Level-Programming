#include "efm32gg.h"
#include "timer.h"
#include "tones.h"

const int happyBirthday[] = { C4, R, C4, D4, D4, D4, C4, C4, C4, F4, F4, F4, E4, E4, E4, E4, E4, R, C4, R, C4, D4, D4, D4, C4, C4, C4, G4, G4, G4, F4, F4, F4, F4, F4, R, C4, R, C4, C5, C5, C5, A4, A4, A4, F4, R, F4, E4, E4, E4, D4, D4, D4, B4, R, B4, A4, A4, A4, F4, F4, F4, G4, G4, G4, F4, F4, F4, F4 };

const int acidSong[] = {C4, R, C4, D4, D4,C4, R, C4, D4, D4,C4, R, C4, D4, D4,C4, R, C4, D4, D4};

const int acidSound[] = {G4, B4, G5, B5, G6, G6, Fs5, C5,G4, B4, G5, B5, G6, G6, Fs5, C5,G4, B4, G5, B5, G6, G6, Fs5, C5};

const int explosion[] = {G4/12, B4/12, G5/12, B5/12, G6/12, G6/12, Fs5/12, C5/12,G4/12, B4/6, G5/6, B5/6, G6/4, G6/4, Fs5/4, C5/4,G4/4, B4/4, G5/4, B5/4, G6/4, G6*7, Fs5/4, C5/4,G4*2, B4*4, G5/12, B5/12, G6/12, G6/12, Fs5/12, C5/12,G4/12, B4/6, G5/6, B5/6, G6/4, G6/4, Fs5/4, C5/4,G4/4, B4/4, G5/4, B5*5, G6/4, G6/4, Fs5/4, C5/4};
