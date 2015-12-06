 // File EBookEntry.cpp
// Implementation for classes
//   EBookEntry
//   EBookEntryReader
// Author: Michael Loceff c 2009

// reads a file containing lines in the format desribed in
// EBookEntry_FormatKey, and provides a vector-like structure
// (a EBookEntryReader object) filled with EBookEntry objects
// for use by the client

#include "EBookEntry.h"

// EBookEntryReader methods ---------------------------------------------
// reads one string in the above record (all lines of the record
// are assumed to be concatenated into a single line prior to 
// this call surrounded by <pgterms:etext> ... </pgterms:etext> )
// and leaves data in an EBookEntry object for return to client
EBookEntry EBookEntryReader::readOneBook(string entryString)
{
   EBookEntry book;

   book.SetNEtextNum(readIDFromLine(entryString));
   book.setCreator(readStringFromEntry(entryString, "<dc:creator"));
   book.setTitle(readStringFromEntry(entryString, "<dc:title"));
   book.setSubject(readStringFromEntry(entryString, "<dc:subject"));

   return book;
}

int EBookEntryReader::readIDFromLine(string line)
{
   unsigned int startNumPos;
   int numLength;

   startNumPos = line.find("ID=\"etext") + 9;
   numLength = line.substr(startNumPos).find( "\"");

   if (startNumPos < 0 || startNumPos > EBookEntry::MAX_STRING 
      || numLength < 0 || numLength > 20 )
      return 0;
   else
      return convertToInt(line.substr(startNumPos, numLength));
}

// looks for a line of the form "<pgterms:etext --- " signifying a
// new record to be read and returns true if found
bool EBookEntryReader::isDataLine(string line)
{
   string s;
   if (line.length() < 15)
      return false;
   
   // check for a line of the form "<pgterms:etext --- "
   
   if (line.substr(0,14) == "<pgterms:etext")
      return true;
   return false;     
 }

string EBookEntryReader::readStringFromEntry
   (string entryString, string delimiter)
{
   unsigned int start, stop, k;
   string stringAfterDelimiter;

   if (delimiter.length() < 1 || entryString.length() < delimiter.length())
      return "(no data found)";

   // first find "<dc:title", e.g.
   start = entryString.find(delimiter);
   if (start > EBookEntry::MAX_ENTRY_LENGTH)
      return "(no data found)";
   stringAfterDelimiter = entryString.substr(start+delimiter.length());

   // we're looking for something line ">A ..." rather than ">< ... "
   for (k = 0; k < stringAfterDelimiter.length() - 1; k++)
   {
      // rather than using isalpha() we check manually to avoid foreign
      if (stringAfterDelimiter[k] == '>' 
         &&
         // home made isalpha()
         (
            (stringAfterDelimiter[k+1]>='a' && stringAfterDelimiter[k+1] <= 'z')
            ||
            (stringAfterDelimiter[k+1]>='A' && stringAfterDelimiter[k+1] <= 'Z')
         )
      )
         break;
   }
   if (k == stringAfterDelimiter.length() - 1)
      return "(no data found)";

   // we've got the starging position for the raw data
   start = k + 1;
   stringAfterDelimiter = stringAfterDelimiter.substr(start); // cut off tags
   stop = stringAfterDelimiter.find("<");   // by above condition, cannot be 0
   
   return stringAfterDelimiter.substr(0, stop);
}

EBookEntry &EBookEntryReader::operator[](int k)
{
   static EBookEntry dummyReturn;
   if (k < 0 || k >= numBooks)
      return dummyReturn;
   return books[k];
}

// reads lines from the input stream, concatenating until it finds
// the terminating tag, "</pgterms:etext>".  Result is a single long
// line containing entire record wrapped in 
// "<pgterms:etext> ... </pgterms:etext>" which it returns to client.
// assumes first line containing <pgterms:etext> is already in 
// line parameter - required for call
string EBookEntryReader::readOneEntry(ifstream &infile, string line)
{
   string fullEntry = line;
   string nextLine;

   while ( !infile.eof() && fullEntry.length() < EBookEntry::MAX_ENTRY_LENGTH - 20 )
   {
      getline (infile, nextLine);
      fullEntry += nextLine;
      if (nextLine == "</pgterms:etext>")
         break;
   }

   // if we have an unterminated entry, force it to be terminated.
   if (nextLine != "</pgterms:etext>")
      fullEntry += "</pgterms:etext>";

   return fullEntry;
}

// constructor which does entire read operation
EBookEntryReader::EBookEntryReader(string fileName)
{
   string name;
   string line, entryString;
   EBookEntry book;

   numBooks = 0;
   fileOpenError = false;
   bookFile = "NO FILE NAME PROVIDED";

   if (fileName.length() == 0)
   {
      fileOpenError = true;
      return;
   }
   bookFile = fileName;

   // open file for reading
   ifstream infile(fileName.c_str());
   if (!infile)
   {
      fileOpenError = true;
      return;
   }

   // for each line that starts #. read and add to vector
   numBooks = 0;
   while ( !infile.eof() )
   {
      getline (infile, line);
      if (isDataLine(line))
      {
         entryString = readOneEntry(infile, line); // expands  line to an entry
         // if it is not an English entry, we'll have problems with chars
         if ( entryString.find( ">en<" ) > EBookEntry::MAX_ENTRY_LENGTH )
            continue;
         book = readOneBook(entryString);
         books.push_back(book);
         numBooks++;
      }
    }
    infile.close();
}

int EBookEntryReader::convertToInt(string strToCnvrt)
{
   int retInt;

   istringstream(strToCnvrt) >> retInt;
   return retInt;
}
// EBookEntry methods ---------------------------------------------------

// static int initialization
int EBookEntry::sortKey = EBookEntry::SORT_BY_CREATOR;

// default constructor
EBookEntry::EBookEntry() 
   : title(""), creator(""), subject(""), eTextNum(0)
{
}

// mutators
bool EBookEntry::setTitle(string strArg)
{
   if (strArg.length() < MIN_STRING || strArg.length() > MAX_STRING)
      return false;
   title = strArg;
   return true;
}
bool EBookEntry::setCreator(string strArg)
{
   if (strArg.length() < MIN_STRING || strArg.length() > MAX_STRING)
      return false;
   creator = strArg;
   return true;
}
bool EBookEntry::setSubject(string strArg)
{
   if (strArg.length() < MIN_STRING || strArg.length() > MAX_STRING)
      return false;
   subject = strArg;
   return true;
}

bool EBookEntry::SetNEtextNum(int intArg)
{
   if (intArg < 1 || intArg > MAX_ID)
      return false;
   eTextNum = intArg;
   return true;
}

bool EBookEntry::setSortType( int whichType )
{
   switch (whichType)
   {
   case SORT_BY_TITLE:
   case SORT_BY_CREATOR:
   case SORT_BY_SUBJECT:
   case SORT_BY_ID:
      sortKey = whichType;
      return true;
   default:
      return false;
   }
   return true;
}

bool EBookEntry::operator<(const EBookEntry &other) const
{
   switch (sortKey)
   {
   case SORT_BY_TITLE:
      return (title < other.title);
   case SORT_BY_CREATOR:
      return (creator < other.creator);
   case SORT_BY_SUBJECT:
      return (subject < other.subject);
   case SORT_BY_ID:
      return (eTextNum < other.eTextNum);
   default:
      return false;
   }
   return true;
}

bool EBookEntry::operator>(const EBookEntry &other) const
{
   return (other < *this);
}

bool EBookEntry::operator==(const EBookEntry &other) const
{
   return !(other < *this) && !(*this < other);
}

bool EBookEntry::operator!=(const EBookEntry &other) const
{
   return !(other == *this);
}
