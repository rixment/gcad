/***************************************************************************
 *   Copyright (C) 2005 by Eryk Klebanski                                  *
 *   rixment@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/ 

#ifndef _GCAD_ANAGRAMSET_H_
#define _GCAD_ANAGRAMSET_H_

#include "GcadException.h"
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace Gcad {
namespace Utilities {

class AnagramSetBase {
 public:
   typedef std::string  Word;
   typedef std::string  AnagramId;

   static AnagramId changeToAnagram(const Word& word);
};

/**
  @brief
    Klasa opisujaca strukture anagramow, sluzaca do szybkiego
    wyszukiwania elementow o zadanych identyfikatorach

  @code
    #include "GcadAnagramSet.h"
    #include <iostream>
    using namespace Gcad::Utilities;
    using namespace std;

    int main()
    {
      AnagramSet<int>  anagramsCollection;

      anagramsCollection.add("devil", 666);
      anagramsCollection.add("pool", 0);
      anagramsCollection.add("loop", 10);
      anagramsCollection.add("olop", 100);

      cout << "word: devil, anagram: " + 
        AnagramSetBase::changeToAnagram("devil") << endl;
      cout << "word: pool, anagram: " + 
        AnagramSetBase::changeToAnagram("pool") << endl;
      cout << "word: loop, anagram: " + 
        AnagramSetBase::changeToAnagram("loop") << endl;

      cout << "word devil exists? " <<
        anagramsCollection.isWordExists("devil") << endl;
      cout << "devil: " <<
        anagramsCollection.getElement("devil") << endl;
      
      cout << "pool: " << anagramsCollection.getElement("pool") << endl;
      cout << "loop: " << anagramsCollection.getElement("loop") << endl;
      cout << "olop: " << anagramsCollection.getElement("olop") << endl;
    }
  @endcode
*/
template<typename T>
class AnagramSet : public AnagramSetBase {
 public:
   typedef std::pair<Word, T>             WordAndElement;
   typedef std::vector<WordAndElement>    Anagrams;
   typedef typename Anagrams::iterator    AnagramsItor;
   typedef std::map<AnagramId, Anagrams>  Collection;
   typedef typename Collection::iterator  CollectionItor;

 public:
   /**
     @brief
       That exception occure when client is trying to add a word
       identifier that already exists within anagram entity
   */
   class WordAlreadyExists : public Exception {
    public:
      WordAlreadyExists(const std::string& e)
        : Exception(e)
      {}
   };

   /**
     @brief
       Exception object of type ElementDontExists occure, when client
       is trying dereference, or remove entity that dosnt exist 
       in anagram collection
   */
   class ElementDontExists : public Exception {
    public:
      ElementDontExists(const std::string& e)
        : Exception(e)
      {}
   };

 public:
   void add(const Word& word, T element);
   void remove(const Word& word);
   bool isWordExists(const Word& word);
   T& getElement(const Word& word);

 private:
   bool isWordExists(const Anagrams& anagrams, const Word& word);

 private:
   Collection anagramsCollection_;
};

AnagramSetBase::AnagramId 
AnagramSetBase
::changeToAnagram(const Word& word) 
{
  AnagramId anagram(word);
  std::sort(anagram.begin(), anagram.end());
  return anagram;
}

template<typename T>
void
AnagramSet<T>
::add(const Word& word, T element) 
{
  AnagramId anagramId(changeToAnagram(word));
  
  if( anagramsCollection_.count(anagramId) != 0 &&
    isWordExists(anagramsCollection_[anagramId], word) )
  {
    throw WordAlreadyExists( "Anagram [" + anagramId + 
      "] already possess an [" + word + "] identifier!");
  }  
  
  anagramsCollection_[anagramId].push_back(make_pair(word, element));
}

template<typename T>
void 
AnagramSet<T>
::remove(const Word& word)
{
  if(!isWordExists(word))
    throw ElementDontExists("Cannot remove word " + word);

  AnagramId anagramId(changeToAnagram(word));
  
  for(AnagramsItor itor = anagramsCollection_[anagramId].begin();
    itor != anagramsCollection_[anagramId].end();
    ++itor)
  {
    if(itor->first == word) { 
      anagramsCollection_[anagramId].erase(itor);
      break;
    }
  }
}

template<typename T>
bool
AnagramSet<T>
::isWordExists(const Anagrams& anagrams, const Word& word)
{
  for(size_t anagramsItor = 0;
    anagramsItor < anagrams.size();
    ++anagramsItor)
  {
    if(anagrams[anagramsItor].first == word)
      return true;
  }
  return false;
}

template<typename T>
bool
AnagramSet<T>
::isWordExists(const Word& word) 
{
  AnagramId a(changeToAnagram(word));
  CollectionItor founded = anagramsCollection_.find(a);
  if(founded == anagramsCollection_.end())
    return false;
  return isWordExists(founded->second, word);
}

template<typename T>
T& 
AnagramSet<T>
::getElement(const Word& word) 
{
  AnagramId anagram(changeToAnagram(word));
  CollectionItor founded = anagramsCollection_.find(anagram);
  
  if(founded == anagramsCollection_.end())
    throw ElementDontExists("Anagram specified by " + word +
      " identifier dosnt exists in collection!");
  
  AnagramsItor wordsCurr(founded->second.begin());
  AnagramsItor wordsEnd(founded->second.end());

  while(wordsCurr != wordsEnd) {
    if(wordsCurr->first == word)
      return wordsCurr->second;
    ++wordsCurr;
  }

  throw ElementDontExists("Element specified by " + word +
    " dosnt exists in set of anagrams!");
}

}
}

#endif
