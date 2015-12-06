// File EBookEntry.h
// Interface for classes
//   EBookEntry - Single ebook object from the Project Gutenberg database
//   EBookEntryReader - Used to read and return EBookEntry objects
// Author: Michael Loceff c 2009

#ifndef EBookEntry_H
#define EBookEntry_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class EBookEntry
{
public:

private:
   string title, creator, subject;
   int eTextNum;

public:
   static const int MIN_STRING = 1;
   static const int MAX_STRING = 300;
   static const int MAX_ENTRY_LENGTH = 10000;
   static const int MAX_ID = 100000L;

   EBookEntry();

   //mutators
   bool setTitle(string strArg);
   bool setCreator(string strArg);
   bool setSubject(string strArg);
   bool SetNEtextNum(int intArg);

   // accessors
   string getTitle() const { return title; }
   string getCreator() const { return creator; }
   string getSubject()const { return subject; }
   int getETextNum() const { return eTextNum; }

   // comparator tools
   // could use static const int, instead:
private:
   static int sortKey;

public:
   enum { SORT_BY_TITLE, SORT_BY_CREATOR, SORT_BY_SUBJECT, SORT_BY_ID };
   static bool setSortType( int whichType );
   bool operator<(const EBookEntry &other) const;
   bool operator>(const EBookEntry &other) const;
   bool operator==(const EBookEntry &other) const;
   bool operator!=(const EBookEntry &other) const;
};

class EBookEntryReader
{
private:
   vector <EBookEntry> books;
   int numBooks;
   bool fileOpenError;
   string bookFile;
   EBookEntry readOneBook(string line);
   bool isDataLine(string line);
   int readIDFromLine(string line);
   string readOneEntry(ifstream &infile, string line);
   string readStringFromEntry(string entryString, string delimiter);

public:
   EBookEntryReader(string fileName);
   EBookEntry &operator[](int k);
   string getFileName() { return bookFile; }
   bool readError() { return fileOpenError; }
   int getNumBooks() { return numBooks; }

private:
   static int convertToInt(string strToCnvrt);
};
#endif