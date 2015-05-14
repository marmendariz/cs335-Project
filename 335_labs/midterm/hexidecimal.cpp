#include <iostream> 
using namespace std;

int getTotal();

int main () 
{
	cout << getTotal () << "\n";
	return 0;
}

int getTotal () 
{
	int total = 0, conseq = 0; // total is # of 
	int k; // the # we are working with in the iteration

	for (int i = 1;
