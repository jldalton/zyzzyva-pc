//---------------------------------------------------------------------------
// WordGraph.h
//
// A Directed Acyclic Word Graph class.
//
// Copyright 2004, 2005 Michael W Thelen <mike@pietdepsi.com>.
//
// This file is part of Zyzzyva.
//
// Zyzzyva is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Zyzzyva is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//---------------------------------------------------------------------------

#ifndef WORD_GRAPH_H
#define WORD_GRAPH_H

#include <qstring.h>
#include <qstringlist.h>

class WordGraph
{
  public:
    WordGraph();
    ~WordGraph();

    void addWord (const QString& w);
    bool containsWord (const QString& w) const;
    QStringList getWordsMatchingPattern (const QString& pattern) const;
    QStringList getAnagrams (const QString& input, bool subanagrams) const;
    void compress();
    void print() const;
    int getNumNodes() { return numNodes; }

  private:
    class Node {
      public:
        Node (char c = 0, bool e = false);
        ~Node();
        bool operator== (const Node& rhs);
        char letter;
        bool eow;
        Node* next;
        Node* child;
    };

    Node* top;
    int numNodes;
};

#endif // WORD_GRAPH_H
