//---------------------------------------------------------------------------
// WordEngine.h
//
// A class to handle the loading and searching of words.
//
// Copyright 2004 Michael W Thelen.  Permission to copy, use, modify, sell and
// distribute this software is granted provided this copyright notice appears in
// all copies. This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//---------------------------------------------------------------------------

#ifndef WORD_ENGINE_H
#define WORD_ENGINE_H

#include "DAWG.h"
#include <qstring.h>

class WordEngine
{
  public:
    WordEngine() { };
    ~WordEngine() { };

    int importFile (const QString& filename);
    bool isAcceptable (const QString& word) const;

  private:
    DAWG graph;
};

#endif // WORD_ENGINE_H
