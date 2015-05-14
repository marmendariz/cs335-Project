#include <iostream>
#include <cmath> // for absolute value function
using namespace std;

float getValue(float val)
{
	return val * 1.5f;
}

const float tolerance = 0.1;

int main () 
{
	float arr [] = {5.0,5.1,5.2,5.3};
	float result[] = {7.50, 7.8, 7.9};
		
	cout << "input a value: ";
	
	for (int i = 0; i < 3; i++) {
		float ret = getValue(arr[i]);
		float diff = ret - result[i];
		diff = fabs(diff); // absolute value of floats 
		if (diff > tolerance)
			cout << "error! at input " << arr[i] << "\n";
	cout << "\n";
	return (0);
}
