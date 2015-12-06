// Main file for EBookEntry project.  See Read Me file for details
// CIS 15C, Foothill College, Michael Loceff, creator

#include <iostream>
using namespace std;

#include "EBookEntry.h"
#include "Foothill_Sort.h"

// for timing our algorithms
#include <time.h>

// ----------- prototypes -------------
void displayOneEBook(EBookEntry book);

// --------------- main ---------------
int main()
{
   int k, arraySize;

   // how we read the data from files
   EBookEntryReader bookInput("catalog-short4.txt");

   // how we test the success of the read:
   if (bookInput.readError())
   {
      cout << "couldn't open " << bookInput.getFileName() << " for input.\n";
      exit(1);
   }

   cout << bookInput.getFileName() << endl;
   cout << bookInput.getNumBooks() << endl;

   // create an array of objects for our own use:
   arraySize = bookInput.getNumBooks();
   EBookEntry *bookArray = new EBookEntry[arraySize];
   for (k = 0; k < arraySize; k++)
      bookArray[k] =  bookInput[k];

   // display first 50 books (only) unsorted
   for (int k = 0; k < 50; k++)
      displayOneEBook(bookArray[k]);
   cout << endl << "Sorting  ... " << endl;

   // how we time our algorithms -------------------------
   clock_t startTime, stopTime; 
   startTime = clock();

   // sort (only half since there are thousands and this is slow)
   EBookEntry::setSortType(EBookEntry::SORT_BY_CREATOR);
   arraySort(bookArray, arraySize/2);
   cout << endl << " Sorted! " << endl << endl ;

   // how we determine the time elapsed -------------------
   stopTime = clock();

   // display first 50 books (only) sorted
   for (int k = 0; k < 50; k++) 
      displayOneEBook(bookArray[k]);

   // report the elapsed time
   cout << "\nAlgorithm Elapsed Time: " 
      << (double)(stopTime - startTime)/(double)CLOCKS_PER_SEC 
      << " seconds." << endl << endl;

   delete[] bookArray;
   return 0;
}

void displayOneEBook(EBookEntry book)
{
   cout << "   # "<< book.getETextNum() << "  ---------------" << endl;
   cout << "   \"" << book.getTitle() <<"\"" << endl;
   cout << "   by " << book.getCreator()  << endl;
   cout << "   re: " << book.getSubject() << endl << endl;
}

/* --------------- Tail End of Run ----------------

   # 28352  ---------------
   "The Children's LongfellowTold in Prose"
   by Hayman, Doris
   re: American poetry -- Adaptations

   # 28304  ---------------
   "Harper's Young People, January 13, 1880An Illustrated Weekly"
   by Various
   re: Children's periodicals, American

   # 28322  ---------------
   "Pioneer Surgery in KentuckyA Sketch"
   by Yandell, David Wendel, 1826-1898
   re: Surgeons -- Kentucky

   # 30160  ---------------
   "Christ, Christianity and the Bible"
   by Haldeman, Isaac Massey, 1845-1933
   re: Jesus Christ -- Divinity


Sorting  ...

 Sorted!

   # 28351  ---------------
   "Dick and His Cat and Other Tales"
   by (no data found)
   re: Animals -- Juvenile fiction

   # 28257  ---------------
   "The Story of the Great War, Volume I (of 8)Introductions; Special Articles;
Causes of War; Diplomatic and State Papers"
   by (no data found)
   re: World War, 1914-1918



 ---------------- [ snip ] ---------------



   # 9506  ---------------
   "Georgian Poetry 1913-15"
   by (no data found)
   re: (no data found)

   # 29265  ---------------
   "The Story of the Great War, Volume III (of 8)History of the European War fro
m Official Sources"
   by (no data found)
   re: World War, 1914-1918

   # 30109  ---------------
   "The Russian Garlandbeing Russian Folk Tales"
   by (no data found)
   re: Tales -- Russia

   # 28282  ---------------
   "Egyptian LiteratureComprising Egyptian tales, hymns, litanies, invocations,
the Book of the Dead, and cuneiform writings"
   by (no data found)
   re: Egyptian literature -- Translations into English

   # 28907  ---------------
   "Book of Philemon"
   by (no data found)
   re: Bible. N.T. Epistles of Paul

   # 28094  ---------------
   "Mediaeval Tales"
   by (no data found)
   re: Literature, Medieval -- Translations into English


Algorithm Elapsed Time: 2.86 seconds.

Press any key to continue . . .
-------------------------------- */

