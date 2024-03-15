#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
  ofstream fout;
  fout.open("sample.txt");
fout << "1" << endl;
fout << "1"<< endl;
 fout << "4096 4096" << endl;

 srand(time(NULL));

 for (int i = 0; i < 4096; i++)
  {	
	for(int j = 0; j< 4096; j++)
	{
	fout << 1+ rand()%10 << " ";


	}
	fout << endl;


  }
  
  fout << "4096 4096" << endl;

 srand(time(NULL));

 for (int i = 0; i < 4096; i++)
  {	
	for(int j = 0; j< 4096; j++)
	{
	fout << 1+ rand()%10 << " ";


	}
	fout << endl;


  }



  return 0;

 }
