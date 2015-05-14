// Sarahbeth Ramirez
// CS 335 - Midterm Exam

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <cmath>
using namespace std;

//frequency definitions
#define A4 440.0
#define F0 A4
#define F0_note A
#define F0_octave 4

int count = 0;
typedef enum {
	C = 1,
	Cs = 2,
	D = 3,
	Ds = 4,
	E = 5,
	F = 6,
	Fs = 7,
	G = 8,
	Gs = 9,
	A = 10,
	As = 11,
	B = 12,
	Z,
	END = B,
	HALF_STEPS_PER_OCTAVE = B 
} note_t;

double freq(note_t note, int octave_delta);
double checkValue (double diff, double given, double tolerance);

int main(int argc, char *argv[])
{	

	double givenVal [17] = { 
		16.3516, 
		17.3239, 
		18.354, 
		19.4454,  
		20.6017, 
		21.8268,  
		23.1247,  
		24.4997,  
		25.9565,  
		27.5,     
		29.1352,  
		30.8677,  
		32.7032,  
		34.6478,  
		36.7081,  
		38.8909,  
		41.2034 
	}; 	

	note_t note;
	int octave_delta;
	double returnVal [17];
	double result [17];
	bool add;
	double tolerance;
	if (argc != 4) {
		cout << "Usage: " << argv[0] << " <NOTE>  <OCTAVE_DELTA> <tolerance>" << endl;
		return 0;
	}
	
	tolerance = atoi(argv[3]);
	
	//
	note = (note_t)(toupper(argv[1][0]) - 64);
	switch(toupper(argv[1][0])) {
		case 'A': note = A; break;
		case 'B': note = B; break;
		case 'C': note = C; break;
		case 'D': note = D; break;
		case 'E': note = E; break;
		case 'F': note = F; break;
		case 'Z': note = Z; break;
	}
	
	octave_delta = atoi (argv[2]);

	if (note == Z && octave_delta == 0) {
		cout << "./mid335	0	" << tolerance << "\n";
		cout << "tolerance: " << tolerance << "\n";
		cout << "freq function unit-test\n";
		cout << "\nnote\toctave\tvalue\t\tdiff\n";
		
		for (int i = 0; i < 17; i++) {
			returnVal [i] = freq (note, octave_delta);
			result [i] = checkValue (returnVal [i], givenVal [i], tolerance);
			cout << " " << i+1 << "\t";
			if (i < 12)
				cout << octave_delta; 
			else 
				cout << "1";
			cout << "\t" << givenVal[i] << "\t\t\t" << result[i] << "\n";
		}

	}	
	
	if (note > END && note != Z) {
		cout << "Invalid note!" << endl;
		return 1;
	}
	
	cout << HALF_STEPS_PER_OCTAVE << endl;
	cout << freq(note, octave_delta) << endl;
	cout << "Bad element count: " << count << "\n";
	cout << "Unit test complete\n";
	return 0;
}

//-----------------------------------------------------------
//Generate a frequency in hz that is half_steps away from C_4
//Do not modify this function.
//-----------------------------------------------------------
double freq(note_t note, int octave_delta)
{
	double freq;
	double factor;
	double a;
	int n;
	int octave = octave_delta - F0_octave;

	a = pow(2.0, 1.0/(double)HALF_STEPS_PER_OCTAVE);
	n = note - F0_note;
	freq = F0 * pow(a, (double)n);
	factor = pow(2.0, (double)octave);
	freq = freq * factor;
	return freq; // returning the frequency 
}

double checkValue (double returnVal, double given, double tolerance) 
{
	double diff = 0;
	
	diff = (returnVal - given);
	diff = 	fabs (diff);
	if (diff > tolerance) {
		cout << "bad!\n";
		count++;
	}
	else 
		cout << "good!\n";
	return diff;
}
