// File iTunes.cpp
// Implementation for classes
//   iTunesEntry
//   iTunesEntryReader
// Author: Michael Loceff c 2009

// reads a file containing lines in the format desribed in
// iTunes_FormatKey, and provides a vector-like structure
// (an iTunesEntryReader object) filled with iTunesEntry objects
// for use by the client

#include "iTunes.h"

// iTunesEntryReader methods ---------------------------------------------
// looks for a line of the form "#" signifying a
// new record to be read and returns true if found
bool iTunesEntryReader::isDataLine(string line)
{
   string s;
   if (line.length() < 1)
      return false;  
   if (line == "#")
      return true;
   return false;     
 }

iTunesEntry &iTunesEntryReader::operator[](int k)
{
   static iTunesEntry dummyReturn;
   if (k < 0 || k >= numTunes)
      return dummyReturn;
   return tunes[k];
}

/* reads 3 lines from the input stream, for example 

Eric Clapton
Pretending
283

*/

bool iTunesEntryReader::readOneEntry(ifstream &infile, iTunesEntry &tune)
{
   string nextLine, fileTitle, fileArtist, fileTime;
   int tuneTime;

   if ( !infile.eof() )
      getline (infile, fileArtist);
   else
      return false;

   if ( !infile.eof() )
      getline (infile, fileTitle);
   else
      return false;

   if ( !infile.eof() )
      getline (infile, fileTime);
   else
      return false;
   // convert string to int
   istringstream(fileTime) >> tuneTime;

   tune.setArtist(fileArtist);
   tune.setTitle(fileTitle);
   tune.setTime(tuneTime);

   return true;
}

// constructor which does entire read operation
iTunesEntryReader::iTunesEntryReader(string fileName)
{
   string name;
   string line;
   iTunesEntry tune;

   numTunes = 0;
   fileOpenError = false;
   tuneFile = "NO FILE NAME PROVIDED";

   if (fileName.length() == 0)
   {
      fileOpenError = true;
      return;
   }
   tuneFile = fileName;

   // open file for reading
   ifstream infile(fileName.c_str());
   if (!infile)
   {
      fileOpenError = true;
      return;
   }

   // for each line that starts #. read and add to vector
   numTunes = 0;
   while ( !infile.eof() )
   {
      getline (infile, line);
      if (isDataLine(line))
      {
         if ( !readOneEntry(infile, tune) )
         {
            fileOpenError = true;
            break;
         }
         tunes.push_back(tune);
         numTunes++;
      }
    }
    infile.close();
}

// iTunesEntry methods ---------------------------------------------------
int iTunesEntry::sortKey = iTunesEntry::SORT_BY_ARTIST;

// default constructor
iTunesEntry::iTunesEntry() 
   : title(""), artist(""), tuneTime(0)
{
}

// mutators
bool iTunesEntry::setTitle(string sArg)
{
   if (sArg.length() < MIN_STRING || sArg.length() > MAX_STRING)
      return false;
   title = sArg;
   return true;
}
bool iTunesEntry::setArtist(string sArg)
{
   if (sArg.length() < MIN_STRING || sArg.length() > MAX_STRING)
      return false;
   artist = sArg;
   return true;
}
bool iTunesEntry::setTime(int nArg)
{
   if (nArg < 0 || nArg > MAX_TIME)
      return false;
   tuneTime = nArg;
   return true;
}


int iTunesEntry::convertStringToSeconds(string sToCnvrt)
{
   unsigned int colonPos;
   int minutes, seconds, lengthOfSecString;

   if (sToCnvrt.length() < 1)
      return 0;
   colonPos = sToCnvrt.find_first_of(":");
   if (colonPos < 0 || colonPos > iTunesEntry::MAX_STRING)
      return 0;

   istringstream(sToCnvrt.substr(0, colonPos)) >> minutes;
   lengthOfSecString = sToCnvrt.length()-1 - colonPos;
   istringstream(sToCnvrt.substr(colonPos + 1, lengthOfSecString))
      >> seconds;

   return minutes*60 + seconds;
}

string iTunesEntry::convertTimeToString() const
{
   int minutes, seconds;
   ostringstream cnvrt1, cnvrt2;
   string retString = "", strSeconds, strMinutes;

   minutes = tuneTime / 60;
   seconds = tuneTime % 60;

   cnvrt1 << minutes;
   cnvrt2 << seconds;

   strMinutes += cnvrt1.str();
   strSeconds = cnvrt2.str();

   if (strSeconds.length() < 2)
      strSeconds = "0" + strSeconds;

   return strMinutes + ":" + strSeconds;
}

bool iTunesEntry::setSortType( int whichType )
{
   switch (whichType)
   {
   case SORT_BY_TITLE:
   case SORT_BY_ARTIST:
   case SORT_BY_TIME:
      sortKey = whichType;
      return true;
   default:
      return false;
   }
   return true;
}

bool iTunesEntry::operator<(const iTunesEntry &other) const
{
   switch (sortKey)
   {
   case SORT_BY_TITLE:
      return (title < other.title);
   case SORT_BY_ARTIST:
      // get last name from string
      // stack the last name before the first - no worries about trailing last
      return 
      (
         getArtistLastName() + artist 
            < 
         other.getArtistLastName() + other.getArtist()
      );
   case SORT_BY_TIME:
      return (tuneTime < other.tuneTime);
   default:
      return false;
   }
   return true;
}

bool iTunesEntry::operator>(const iTunesEntry &other) const
{
   return (other < *this);
}

bool iTunesEntry::operator==(const iTunesEntry &other) const
{
   return !(other < *this) && !(*this < other);
}

bool iTunesEntry::operator!=(const iTunesEntry &other) const
{
   return !(other == *this);
}

string iTunesEntry::getArtistLastName() const
{
   // search for first blank from end of string
   // assume no trailing spaces
   string retString;
   int k, length;

   length = artist.length();
   if ( length < 1 )
      return "";

   for (k = length-1; k >= 0; k--)
   {
      if (artist[k] == ' ')
         break;
   }

   if (k >= length-1 )
      return "";

   return artist.substr(k + 1, artist.length()-1 - k);
}