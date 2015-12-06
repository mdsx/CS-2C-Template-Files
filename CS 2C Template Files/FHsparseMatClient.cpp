#include <iostream>
using namespace std;
#include "SparseMat.h"

#define MAT_SIZE 100000
typedef SparseMat<float> SpMat;

// --------------- main ---------------
int main()
{
   int k;
   SpMat mat(MAT_SIZE, MAT_SIZE, 0); // 100000 x 100000 filled with 0

   cout << "testing constructor error:" << endl;
   try
   {
      SpMat matBad(MAT_SIZE, -MAT_SIZE, 0);
   }
   catch (SpMat::MatConstructorException)
   {
      cout << "oops - bad arg in constructor" << endl;
   }

   // test mutators
   for (k = 0; k < 10; k++)
   {
      mat.set(k, k, k * 1.);
      mat.set(4, k, k * 10.);
      mat.set(k, 4) = -k * 10.;
   }

   // test extra credit set
   cout << "\ntesting extra credit set:" << endl;
   try
   {
      mat.set(0, 0) = 55;
      mat.set(1, 1) = 0;
      mat.set(-4, 7);  // should throw an exception
      mat.set(2, 2) = 0;  // should not get here
   }
   catch (SpMat::MatBoundsException)
   {
      cout << "oops - bounds in extra credit set()" << endl;
   }

   cout << "\ntesting get bounds error:" << endl;
   try
   {
      cout << mat.get(7, 8) << endl;
      cout << mat.get(2, 5) << endl;
      cout << mat.get(9, 9) << endl;
      cout << mat.get(-4, 7) << endl;  // should throw an exception
   }
   catch (SpMat::MatBoundsException)
   {
      cout << "oops - bounds in get()" << endl;
   }

   // show top left 12x12
   cout << "\nFirst 12x12 subsquare of original" << endl;
   mat.showSubSquare(0, 12);

   // show bottom right  12x12
   cout << "\nLast 12x12 subsquare of original" << endl;
   mat.showSubSquare(MAT_SIZE - 12, 12);
}