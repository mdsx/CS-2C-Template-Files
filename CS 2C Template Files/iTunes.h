// File iTunes.h
// Interface for classes
//   iTunesEntry - Single iTune object from the simplified iTunes data file
//   iTunesEntryReader - Used to read and return iTunesEntry objects
// Author: Michael Loceff c 2009

#ifndef ITUNES_H
#define ITUNES_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class iTunesEntry
{
public:

private:
   string title, artist;
   int tuneTime;

public:
   static const unsigned int MIN_STRING = 1;
   static const unsigned int MAX_STRING = 300;
   static const int MAX_TIME = 10000000;

   iTunesEntry();

   //mutators
   bool setTitle(string sArg);
   bool setArtist(string sArg);
   bool setTime(int nArg);

   // accessors
   string getTitle() const { return title; }
   string getArtist() const { return artist; }
   int getTime() const { return tuneTime; }

   // helpers
   static int convertStringToSeconds(string sToCnvrt);
   string convertTimeToString() const;

   // comparator tools
   // could use static const int, instead:
private:
   static int sortKey;

public:
   static enum { SORT_BY_TITLE, SORT_BY_ARTIST, SORT_BY_TIME } eSortType;
   static bool setSortType( int whichType );
   bool operator<(const iTunesEntry &other) const;
   bool operator>(const iTunesEntry &other) const;
   bool operator==(const iTunesEntry &other) const;
   bool operator!=(const iTunesEntry &other) const;
   string getArtistLastName() const;
};

class iTunesEntryReader
{
private:
   vector <iTunesEntry> tunes;
   int numTunes;
   bool fileOpenError;
   string tuneFile;
   bool readOneEntry(ifstream &infile, iTunesEntry &tune);
   bool isDataLine(string line);

public:
   iTunesEntryReader(string fileName);
   iTunesEntry &operator[](int k);
   string getFileName() { return tuneFile; }
   bool readError() { return fileOpenError; }
   int getNumTunes() { return numTunes; }
};
#endif