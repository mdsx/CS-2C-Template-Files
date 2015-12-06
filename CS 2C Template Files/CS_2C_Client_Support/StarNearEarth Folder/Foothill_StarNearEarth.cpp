// Main file for StarNearEarth project.  See Read Me file for details
// CIS 15C, Foothill College, Michael Loceff, creator

#include <iostream>
using namespace std;

#include "StarNearEarth.h"
#include "Foothill_Sort.h"

// for timing our algorithms
#include <time.h>

// ----------- prototypes -------------
void displayOneStar(StarNearEarth star);

// --------------- main ---------------
int main()
{
   int k, arraySize;
   // how we read the data from files
   StarNearEarthReader starInput("nearest_stars.txt");

   // how we test the success of the read:
   if (starInput.readError())
   {
      cout << "couldn't open " << starInput.getFileName() << " for input.\n";
      exit(1);
   }

   cout << starInput.getFileName() << endl;
   cout << starInput.getNumStars() << endl;

   // create an array of objects for our own use:
   arraySize = starInput.getNumStars();
   StarNearEarth *starArray = new StarNearEarth[arraySize];
   for (k = 0; k < arraySize; k++)
      starArray[k] =  starInput[k];

   // display list unsorted
   for (int k = 0; k < arraySize; k++)
      displayOneStar(starArray[k]);

   // how we time our algorithms -------------------------
   clock_t startTime, stopTime; 
   startTime = clock();

   // sort
   cout << endl << "Sorting  ... " << endl;
   StarNearEarth::setSortType(StarNearEarth::SORT_BY_MASS);
   arraySort(starArray, arraySize);
   cout << endl << " Sorted! " << endl << endl;

   // how we determine the time elapsed -------------------
   stopTime = clock();

   // display list sorted
   for (int k = 0; k < arraySize; k++)
      displayOneStar(starArray[k]);

   // report algorithm time
   cout << "\nAlgorithm Elapsed Time: " 
      << (double)(stopTime - startTime)/(double)CLOCKS_PER_SEC 
      << " seconds." << endl << endl;

   delete[] starArray;   
   return 0;
}

void displayOneStar(StarNearEarth star)
{
   cout << "   #"<< star.getRank() << ".  \""  
      << star.getNameCommon() <<"\"  ----------" << endl;
   cout << "    LHS Name: " << star.getNameLhs()
      << " CNS Name: " << star.getNameCns() <<  endl;
   cout << "    Absolute Mag: " << star.getMagAbsolute() 
      << " Apparent Mag: " << star.getMagApparent() << endl;
   cout << "    Parallax Mean: " << star.getParallaxMean() 
      << " variance: " << star.getParallaxVariance() << endl;
   cout << "    Right Asc: " << star.getRAsc() 
      << " Dec: " << star.getDec() << endl;
   cout << "    Mass: " << star.getMass() 
      << " Prop Mot Mag: " << star.getPropMotionMag() << endl;
}

/* --------------- Tail End of Run ----------------

   ...

   #17.  "epsilon Indi A"  ----------
    LHS Name: 67 CNS Name: GJ  845    A
    Absolute Mag: 6.89 Apparent Mag: 4.69
    Parallax Mean: 0.27584 variance: 0.00069
    Right Asc: 330.84 Dec: -56.7861
    Mass: 0.77 Prop Mot Mag: 4.704
   #96.  "(no common name)"  ----------
    LHS Name: 71 CNS Name: GJ  892
    Absolute Mag: 6.49 Apparent Mag: 5.56
    Parallax Mean: 0.15341 variance: 0.00054
    Right Asc: 348.321 Dec: 57.1683
    Mass: 0.81 Prop Mot Mag: 2.095
   #78.  "(no common name)"  ----------
    LHS Name: 486 CNS Name: GJ  783    A
    Absolute Mag: 6.41 Apparent Mag: 5.32
    Parallax Mean: 0.16533 variance: 0.0009
    Right Asc: 302.8 Dec: -36.1011
    Mass: 0.82 Prop Mot Mag: 1.64
   #77.  "36 Ophiuchi A"  ----------
    LHS Name: 437 CNS Name: GJ  663    A
    Absolute Mag: 6.19 Apparent Mag: 5.07
    Parallax Mean: 0.16751 variance: 0.00075
    Right Asc: 258.837 Dec: -26.6028
    Mass: 0.85 Prop Mot Mag: 1.238
   #9.  "epsilon Eridani"  ----------
    LHS Name: 1557 CNS Name: GJ  144
    Absolute Mag: 6.19 Apparent Mag: 3.73
    Parallax Mean: 0.30999 variance: 0.00079
    Right Asc: 53.2325 Dec: -9.45833
    Mass: 0.85 Prop Mot Mag: 0.977
   #50.  "omicron 2 Eridani"  ----------
    LHS Name: 23 CNS Name: GJ  166    A
    Absolute Mag: 5.92 Apparent Mag: 4.43
    Parallax Mean: 0.199 variance: 0.00077
    Right Asc: 63.8179 Dec: -7.65278
    Mass: 0.89 Prop Mot Mag: 4.088
   #65.  "sigma Draconis"  ----------
    LHS Name: 477 CNS Name: GJ  764
    Absolute Mag: 5.88 Apparent Mag: 4.68
    Parallax Mean: 0.17359 variance: 0.00041
    Right Asc: 293.09 Dec: 69.6611
    Mass: 0.89 Prop Mot Mag: 1.839
   #52.  "70 Ophiuchi A"  ----------
    LHS Name: 458 CNS Name: GJ  702    A
    Absolute Mag: 5.66 Apparent Mag: 4.2
    Parallax Mean: 0.19596 variance: 0.00087
    Right Asc: 271.364 Dec: 2.5
    Mass: 0.92 Prop Mot Mag: 0.971
   #19.  "tau Ceti"  ----------
    LHS Name: 146 CNS Name: GJ   71
    Absolute Mag: 5.68 Apparent Mag: 3.49
    Parallax Mean: 0.27439 variance: 0.00076
    Right Asc: 26.0171 Dec: -15.9375
    Mass: 0.92 Prop Mot Mag: 1.922
   #79.  "82 Eridani"  ----------
    LHS Name: 19 CNS Name: GJ  139
    Absolute Mag: 5.35 Apparent Mag: 4.26
    Parallax Mean: 0.16501 variance: 0.00055
    Right Asc: 49.9817 Dec: -43.0697
    Mass: 0.97 Prop Mot Mag: 3.124
   #81.  "delta Pavonis"  ----------
    LHS Name: 485 CNS Name: GJ  780
    Absolute Mag: 4.63 Apparent Mag: 3.56
    Parallax Mean: 0.16378 variance: 0.00065
    Right Asc: 302.182 Dec: -66.1819
    Mass: 1.1 Prop Mot Mag: 1.656
   #73.  "eta Cassiopei A"  ----------
    LHS Name: 123 CNS Name: GJ   34    A
    Absolute Mag: 4.58 Apparent Mag: 3.45
    Parallax Mean: 0.16838 variance: 0.00059
    Right Asc: 12.2763 Dec: 57.8153
    Mass: 1.11 Prop Mot Mag: 1.223
   #13.  "Procyon"  ----------
    LHS Name: 233 CNS Name: GJ  280    A
    Absolute Mag: 2.66 Apparent Mag: 0.38
    Parallax Mean: 0.28605 variance: 0.00081
    Right Asc: 114.825 Dec: 5.225
    Mass: 1.57 Prop Mot Mag: 1.259
   #53.  "Altair"  ----------
    LHS Name: 3490 CNS Name: GJ  768
    Absolute Mag: 2.22 Apparent Mag: 0.77
    Parallax Mean: 0.19497 variance: 0.00086
    Right Asc: 297.696 Dec: 8.86833
    Mass: 1.71 Prop Mot Mag: 0.661
   #5.  "Sirius"  ----------
    LHS Name: 219 CNS Name: GJ  244    A
    Absolute Mag: 1.47 Apparent Mag: -1.43
    Parallax Mean: 0.38002 variance: 0.00128
    Right Asc: 101.287 Dec: -16.7161
    Mass: 1.99 Prop Mot Mag: 1.339

Algorithm Elapsed Time: 0.015 seconds.

Press any key to continue . . .

------------------------------------------- */