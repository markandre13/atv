/*
 * Attribute-Type-Value Object Language Parser: Example 1
 * Copyright (C) 2001-2003 by Mark-André Hopf <mhopf@mark13.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <atvparser.hh>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;
using namespace atv;

// EXAMPLE 1: Configuration File
//----------------------------------------------------------------

class TMenuEntry;

class TMenuDefinition:
  public TATVInterpreter
{
  public:
    bool interpret(TATVParser&);
    typedef list<TMenuEntry> TList;
    TList children;
    void print(unsigned depth=0);
};


class TMenuEntry:
  public TMenuDefinition
{
  public:
    string name;
    string label;
    string shortcut;
    string icon;
    
    bool interpret(TATVParser&);
};

bool
TMenuDefinition::interpret(TATVParser &p)
{
  switch(p.getInterpreterState()) {
    case 0:
      switch(p.what) {
        case ATV_GROUP:
          if (p.attribute.empty() && p.type=="menu") {
            p.setInterpreterState(1);
            return true;
          }
          break;
        case ATV_FINISHED:
          return true;
          break;
      } break;
    case 1:
      switch(p.what) {
        case ATV_GROUP:
          if (p.attribute.empty() && p.type=="entry") {
            TMenuEntry *entry = &(*children.insert(children.end(), TMenuEntry()));
            p.setInterpreter(entry);
            return true;
          }
          break;
        case ATV_FINISHED:
          return true;
      } break;
  }
  return false;
}

void
TMenuDefinition::print(unsigned depth)
{
  TList::iterator p, e;
  p = children.begin();
  e = children.end();
  while(p!=e) {
    for(unsigned i=0; i<depth; ++i)
      cout << "  ";
    cout << "name=\"" << (*p).name << "\" "
         << "label=\"" << (*p).label << "\" "
         << "shortcut=\"" << (*p).shortcut << "\"" << endl;
    (*p).print(depth+1);
    ++p;
  }
}

bool
TMenuEntry::interpret(TATVParser &p) {
  switch(p.what) {
    case ATV_VALUE:
      if (p.attribute == "name" && p.type.empty()) {
        name = p.value;
        return true;
      }
      if (p.attribute == "label" && p.type.empty()) {
        label = p.value;
        return true;
      }
      if (p.attribute == "shortcut" && p.type.empty()) {
        shortcut = p.value;
        return true;
      }
      if (p.attribute == "icon" && p.type.empty()) {
        icon = p.value;
        return true;
      }
      break;
    case ATV_GROUP:
      if (p.attribute.empty() && p.type=="entry") {
        TMenuEntry *entry = &(*children.insert(children.end(), TMenuEntry()));
        p.setInterpreter(entry);
        return true;
      }
      break;
    case ATV_FINISHED:
      if (name.empty()) {
        p.err << "no 'name' specified";
        return false;
      }
      return true;
  }
  return false;
}

int
main()
{
  ifstream in("menu.atv");
  TMenuDefinition menu;
  TATVParser atv(&in);
  atv.setVerbose(true);
  atv.setInterpreter(&menu);
  if (!atv.parse()) {
    cout << atv.getErrorText() << endl;
    return EXIT_FAILURE;
  } else {
    menu.print();
  }
  return EXIT_SUCCESS;
}
