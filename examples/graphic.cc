/*
 * Attribute-Type-Value Object Language Parser: Example 2
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

#include <iostream>
#include <serializable.hh>
#include <fstream>
#include <vector>

using namespace std;
using namespace atv;

// EXAMPLE 2: Object Serialisation
//----------------------------------------------------------------

typedef unsigned char byte;

class TRGB:
  public TSerializable
{
    // tip: always define 'super' as the TSerializeable class
    // this class is derived from
    // tip: always make the 'super' definition 'private' as it
    // has a wrong meaning in classes derived from this one
    typedef TSerializable super;
  public:
    byte r,g,b;

    TCloneable* clone() const { return new TRGB(*this); }
    const char * getClassName() const { return "toad::TRGB"; }
    void store(TOutObjectStream&) const;
    bool restore(TInObjectStream&);
};

class TRGBA:
  public TRGB
{
    typedef TRGB super;
  public:
    byte alpha;
    TCloneable* clone() const { return new TRGBA(*this); }
    const char * getClassName() const { return "toad::TRGBA"; }
    void store(TOutObjectStream&) const;
    bool restore(TInObjectStream&);
};

class TGadget:
  public TSerializable
{
    typedef TSerializable super;
  public:
    bool filled;
    TRGB line_color;
    TRGB * fill_color;

    TCloneable* clone() const { return new TGadget(*this); }
    const char * getClassName() const { return "toad::TGadget"; }
    void store(TOutObjectStream&) const ;
    bool restore(TInObjectStream&);
};

class TGRectangle:
  public TGadget
{
    typedef TGadget super;
  public:
    int x, y, w, h;
  
    TCloneable* clone() const { return new TGRectangle(*this); }
    const char * getClassName() const { return "toad::TGRectangle"; }
    void store(TOutObjectStream&) const;
    bool restore(TInObjectStream&);
};

class TGText:
  public TGRectangle
{
    typedef TGRectangle super;
  public:
    string text;
  
    TCloneable* clone() const { return new TGText(*this); }
    const char * getClassName() const { return "toad::TGText"; }
    void store(TOutObjectStream&) const;
    bool restore(TInObjectStream&);
};

class TGGroup:
  public TGadget
{
    typedef TGadget super;
    typedef vector<TGadget*> TStorage;
    TStorage storage;
  public:
    TCloneable* clone() const { return new TGGroup(*this); }
    const char * getClassName() const { return "toad::TGGroup"; }
    void store(TOutObjectStream&) const;
    bool restore(TInObjectStream&);
};

bool
restore(TInObjectStream &p, TRGB *value)
{
//cerr << __PRETTY_FUNCTION__ << endl;
  if (p.what != ATV_GROUP) {
    return false;
  }
  if (!p.type.empty()) {
    cerr << "type name '" << p.type << "' specified for implicit type." << endl;
    p.err << "type name '" << p.type << "' specified for implicit type." << endl;
    return false;
  }
  
  // tip: 
  if (!p.type.empty() && p.type != value->getClassName()) {
    cerr << "type mismatch, expected '" << value->getClassName() << "' but got '"
         << p.type << "'\n";
    return false;
  }
  p.setInterpreter(value);
  return true;
}

void
TRGB::store(TOutObjectStream &out) const
{
  ::store(out, r);
  ::store(out, g);
  ::store(out, b);
}

bool 
TRGB::restore(TInObjectStream &p)
{
//  cerr << __PRETTY_FUNCTION__ << endl;
  if (
    ::restore(p, 0, &r) ||
    ::restore(p, 1, &g) ||
    ::restore(p, 2, &b) ||
    super::restore(p)
    ) return true;
  ATV_FAILED(p);
  return false;
}

void
TRGBA::store(TOutObjectStream &out) const
{
  super::store(out);
  ::store(out, alpha);
}

bool 
TRGBA::restore(TInObjectStream &p)
{
//  cerr << __PRETTY_FUNCTION__ << endl;
  if (
    ::restore(p, 3, &alpha) ||
    super::restore(p)
    ) return true;
  ATV_FAILED(p);
  return false;
}

void
TGadget::store(TOutObjectStream &out) const
{
  ::store(out, "filled", filled);
  ::store(out, "linecolor", line_color);
  ::store(out, "fillcolor", fill_color);
}

bool 
TGadget::restore(TInObjectStream &p)
{
//  cerr << __PRETTY_FUNCTION__ << endl;
  if (
    ::restore(p, "filled", &filled) ||
    ::restore(p, "linecolor", &line_color) ||
    ::restorePtr(p, "fillcolor", &fill_color) ||
    super::restore(p)
    ) return true;
  ATV_FAILED(p)
  return false;
}

void
TGRectangle::store(TOutObjectStream &out) const
{
  super::store(out);
  ::store(out, "x", x);
  ::store(out, "y", y);
  ::store(out, "w", w);
  ::store(out, "h", h);
}

bool 
TGRectangle::restore(TInObjectStream &p)
{
//cerr << __PRETTY_FUNCTION__ << endl;
  if (
    ::restore(p, "x", &x) ||
    ::restore(p, "y", &y) ||
    ::restore(p, "w", &w) ||
    ::restore(p, "h", &h) ||
    // tip: always call super::restore instead of atv::finished
    // tip: call super::restore after all other checks to avoid
    // bogus error messages from the ATV_FAILED macro
    super::restore(p) ) return true;
  // tip: place ATV_FAILED before the 'return false;' statement to
  // get more verbose error messages during development
  ATV_FAILED(p)
  return false;
}

void
TGText::store(TOutObjectStream &out) const
{
  super::store(out);
//  ::store(out, "text", text);
  ::storeRaw(out, "text", text.c_str(), text.size());
}

bool 
TGText::restore(TInObjectStream &in)
{
//cerr << __PRETTY_FUNCTION__ << endl;
#if 1
  static char *t = 0;
  static unsigned n;

  if (::restoreRaw(in, "text", &t, &n)) {
//    cerr << "found raw, location of t is " << &t << endl;
    return true;
  }
  
  if (in.what == ATV_FINISHED) {
    if (t) {
//      cerr << "tada:" << endl;
//      cerr << "got raw: " << t << endl;
      t[n]=0;
      text = t;
      free(t);
      t = 0;
    }
  }
#endif  
  if (
    ::restore(in, "text", &text) ||
    super::restore(in) ) return true;
  ATV_FAILED(in)
  return false;
}

void
TGGroup::store(TOutObjectStream &out) const
{
  super::store(out);
  TStorage::const_iterator p(storage.begin()), e(storage.end());
  while(p!=e) {
    out.indent();
    ::store(out, *p);
    ++p;
  }
}

bool 
TGGroup::restore(TInObjectStream &in)
{
  TGadget *g;
  if (::restorePtr(in, &g)) {
    cerr << "restored gadget in group" << endl;
    storage.push_back(g);
    return true;
  }

  if (
    super::restore(in) ) return true;
  ATV_FAILED(in)
  return false;
}

int
main()
{
  TObjectStore &db(getDefaultStore());

  db.registerObject(new TRGB);
  db.registerObject(new TRGBA);
  db.registerObject(new TGadget);
  db.registerObject(new TGRectangle);
  db.registerObject(new TGText);
  db.registerObject(new TGGroup);
  
  ifstream is("graphic.atv");
  TInObjectStream in(&is);
  in.setDebug(false);
  in.setVerbose(false);
  
  TOutObjectStream out;
  out.setOStream(&cerr);
  TSerializable *s;
#if 1
  while(true) {
    assert(in.getDepth()==0);
    s = in.restore();
    assert(in.getDepth()==0);
    if (!in)
      cerr << "error: \n" << in.getErrorText() << endl;
    if (!s)
      break;
    out.store(s);
  }
#else
  vector<TGadget*> *g;
  in.restore(g);
#endif
}
