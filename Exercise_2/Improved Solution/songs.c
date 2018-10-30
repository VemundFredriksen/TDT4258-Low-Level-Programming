#include "efm32gg.h"
#include "tones.h"

int happy[] = { C4, R, C4, D4, C4, F4,
	E4, C4, R, C4, D4,
	C4, G4, F4,
	C4, R, C4, C5, A4, F4, R, F4,
	E4, D4, As4, R, As4, A4,
	F4, G4, F4
};
float happyBeats[] = { 1, 1, 1, 3, 3, 3,
	6, 1, 1, 1, 3,
	3, 3, 6,
	1, 1, 1, 3, 3, 1, 1, 1,
	3, 3, 1, 1, 1, 3,
	3, 3, 4
};

int happyBPM = 600;

int acid1[] =
    { C4, R, C4, D4, D4, C4, R, C4, D4, D4, C4, R, C4, D4, D4, C4, R, C4,
  D4, D4 };
float acid1Beats[] = {[0 ... 19] = 1 };

int acid1BPM = 2646;

int acid2[] =
    { G4, B4, G5, B5, G6, G6, Fs5, C5, G4, B4, G5, B5, G6, G6, Fs5, C5, G4,
  B4, G5, B5, G6, G6, Fs5, C5 };
float acid2Beats[] = {[0 ... 23] = 1 };

int acid2BPM = 2646;

int explosion[] =
    { G4 / 12, B4 / 12, G5 / 12, B5 / 12, G6 / 12, G6 / 12, Fs5 / 12,
C5 / 12, G4 / 12, B4 / 6, G5 / 6, B5 / 6, G6 / 4, G6 / 4, Fs5 / 4, C5 / 4,
G4 / 4, B4 / 4, G5 / 4, B5 / 4, G6 / 4, G6 * 7, Fs5 / 4, C5 / 4, G4 * 2,
B4 * 4, G5 / 12, B5 / 12, G6 / 12, G6 / 12, Fs5 / 12, C5 / 12, G4 / 12, B4 / 6,
G5 / 6, B5 / 6, G6 / 4, G6 / 4, Fs5 / 4, C5 / 4, G4 / 4, B4 / 4, G5 / 4, B5 * 5,
G6 / 4, G6 / 4, Fs5 / 4, C5 / 4 };
float explosionBeats[] = {[0 ... 47] = 1 };

int explosionBPM = 4000;

int *instantiateSong(int songIndex)
{
	if (songIndex == 0) {
		static int happySamples[31];
		for (int i = 0; i < (31); i++) {
			happySamples[i] =
			    (int) (happyBeats[i] *
				   (44100.0 * (60.0 / happyBPM)));
		}
		return happySamples;
	} else if (songIndex == 1) {
		static int acid1Samples[20];
		for (int i = 0; i < (20); i++) {
			acid1Samples[i] =
			    (int) (acid1Beats[i] *
				   (44100.0 * (60.0 / acid1BPM)));
		}
		return acid1Samples;
	} else if (songIndex == 2) {
		static int acid2Samples[24];
		for (int i = 0; i < (24); i++) {
			acid2Samples[i] =
			    (int) (acid2Beats[i] *
				   (44100.0 * (60.0 / acid2BPM)));
		}
		return acid2Samples;
	} else if (songIndex == 3) {
		static int explosionSamples[48];
		for (int i = 0; i < (48); i++) {
			explosionSamples[i] =
			    (int) (explosionBeats[i] *
				   (44100.0 * (60.0 / explosionBPM)));
		}
		return explosionSamples;
	}
	return 0;
}
