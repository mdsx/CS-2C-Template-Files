// Main file for iTunes project.  See Read Me file for details
// CIS 15C, Foothill College, Michael Loceff, creator

#include <iostream>
using namespace std;

#include "iTunes.h"
#include "Foothill_Sort.h"

// for timing our algorithms
#include <time.h>

// ----------- prototypes -------------
void displayOneTune(iTunesEntry tune);

// --------------- main ---------------
int main()
{
   // how we read the data from files
   iTunesEntryReader tunesInput("itunes_file.txt");
   int arraySize;

   // how we test the success of the read:
   if (tunesInput.readError())
   {
      cout << "couldn't open " << tunesInput.getFileName() << " for input.\n";
      exit(1);
   }

   cout << tunesInput.getFileName() << endl;
   cout << tunesInput.getNumTunes() << endl;

   // create an array of objects for our own use:
   arraySize = tunesInput.getNumTunes();
   iTunesEntry *tunesArray = new iTunesEntry[arraySize];
   for (int k = 0; k < arraySize; k++)
      tunesArray[k] =  tunesInput[k];
   
   // show the array, unsorted
   for (int k = 0; k < arraySize; k++)
      displayOneTune(tunesArray[k]);
   cout << endl << endl;

   // how we time our algorithms -------------------------
   clock_t startTime, stopTime; 
   startTime = clock();

   // sort
   iTunesEntry::setSortType(iTunesEntry::SORT_BY_TIME);
   arraySort(tunesArray, arraySize);

   // how we determine the time elapsed -------------------
   stopTime = clock();

   // show sorted list
   for (int k = 0; k < arraySize; k++)
      displayOneTune(tunesArray[k]);
   cout << endl << endl;

   // report algorithm time
   cout << "\nAlgorithm Elapsed Time: " 
      << (double)(stopTime - startTime)/(double)CLOCKS_PER_SEC 
      << " seconds." << endl << endl;

   delete[] tunesArray;
   return 0;
}

void displayOneTune(iTunesEntry tune)
{
   cout << tune.getArtist() << " | ";
   cout << tune.getTitle() << " | ";
   // cout << tune.GetNTime() << " | "; 
   cout << " " << tune.convertTimeToString() << endl;
}

/* --------------- Tail End of Run ----------------

...

Suzanne Vega | Luka |  3:51
Suzanne Vega | Small Blue Thing |  3:55
Bonnie Raitt | Something to Talk About |  3:47
Bonnie Raitt | I Can't Make You Love Me |  5:31
Natalie Cole | This Will Be |  2:51
Natalie Cole | Unforgettable |  3:31
Jet | Timothy |  4:20
Jet | Rip It Up |  3:20
Was (Not Was) | Where Did Your Heart Go? |  5:47
Was (Not Was) | I Blew Up The United States |  3:50


Veggie Tales | Our Big Break |  1:09
Blue Record | Bullhead's Psalm |  1:19
Yo-yo Ma | Bach: Suite for Cello No. 1 in G Major Prelude |  2:21
Caraivana | Tico-Tico No Fuba |  2:27
Yo-yo Ma | Simple Gifts |  2:34
Blue Record | Ogeechee Hymnal |  2:35
Ry Cooter | France Chance |  2:48
Natalie Cole | This Will Be |  2:51
Ry Cooter | Alimony |  2:55
Howlin' Wolf | Well That's All Right |  2:55
Nina Simone | Feeling Good |  2:57
Howlin' Wolf | Everybody's In The Mood |  2:58
John Lee Hooker | I Can't Quit You Baby |  3:02
Veggie Tales | Donuts for Benny |  3:04
Nina Simone | The Other Woman |  3:06
John Lee Hooker | Hobo Blues |  3:07
AOL Dejando Huella | Te Amo Tanto |  3:12
The Rubyz | Watch the Girl |  3:12
Reverend Gary Davis | Twelve Sticks |  3:14
Snoop Dog | Lil' Crips |  3:15
Jet | Rip It Up |  3:20
The Rubyz | Ladies and Gentleman |  3:21
Aaron Watson | The Road |  3:24
AOL Dejando Huellas | Dime Si te Vas Con El |  3:24
Sean Kingston | My Girlfriend |  3:24
Kanye West | Good Life |  3:27
Roy Buchanan | Hot Cha |  3:28
Jeff Golub | Shuffleboard |  3:30
Natalie Cole | Unforgettable |  3:31
Reverend Gary Davis | Samson and Delilah |  3:36
Snoop Dog | Think About It |  3:37
Lil Jon | Give It All U Got |  3:38
Carrie Underwood | Quitter |  3:40
Bonnie Raitt | Something to Talk About |  3:47
T-Pain | Take Your Shirt Off |  3:48
Rihanna | Russian Roulette |  3:48
Was (Not Was) | I Blew Up The United States |  3:50
Foo Fighters | Monkey Wrench |  3:50
Janiva Magness | I'm Just a Prisoner |  3:50
Suzanne Vega | Luka |  3:51
Suzanne Vega | Small Blue Thing |  3:55
Carrie Underwood | Cowboy Casanova |  3:56
Sean Kingston | Fire Burning |  3:59
Jay-Z | What We Talkin' About |  4:03
Caraivana | Noites Cariocas |  4:12
John Coltrane | In a Sentimental Mood |  4:16
Snoop Dogg | Gangsta Luv |  4:17
Jet | Timothy |  4:20
Foo Fighters | All My Life |  4:23
Terra Incogni | Lizard Skin |  4:30
Jay-Z | Empire State of Mind |  4:36
Janiva Magness | You Were Never Mine |  4:36
Steely Dan | Kid Charlemagne |  4:38
Eric Clapton | Pretending |  4:43
Mastadon | The Bit |  4:55
Terra Incognita | Clone |  4:58
Jeff Golub | Fish Fare |  4:59
Eric Clapton | Bad Love |  5:08
Steely Dan | Black Cow |  5:10
Kanye West | Stronger |  5:11
Return to Forever | Medieval Overture |  5:13
Herbie Hancock | Rockit |  5:25
Bonnie Raitt | I Can't Make You Love Me |  5:31
Snoop Dogg | That's The Homie |  5:43
Was (Not Was) | Where Did Your Heart Go? |  5:47
Mastadon | Oblivion |  5:48
Steely Dan | Haitian Divorce |  5:51
Jeff Golub | Goin' On |  5:56
McCoy Tyner | Blues On the Corner |  6:07
Nina Simone | Pirate Jenny |  6:42
John Patitucci | Monk/Trane |  7:14
Roy Buchanan | Green Onions |  7:23
John Patitucci | Sonny Side |  7:25
Herbie Hancock | Nefertiti |  7:31
John Coltrane | A Love Supreme Part 1 |  7:42
McCoy Tyner | Afro Blue |  12:22
Berliner Philharmoniker | Brahms: Symphony No. 4 in E Minor Op. 98 |  13:20
Berliner Philharmoniker | Brahms: Symphony No. 1 in C Minor Op. 68 |  13:59
Herbie Hancock | Chameleon |  15:41



Algorithm Elapsed Time: 0.015 seconds.

Press any key to continue . . .

---------------------------------------------------- */