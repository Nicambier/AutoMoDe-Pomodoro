// GEListGenome.h -*- C++ -*-
#ifndef _GELISTGENOME_H
#define _GELISTGENOME_H

#include<ga/ga.h>
#include "GE/Mapper.h"


struct Allele
{
  unsigned a:4;  // <- 4 bits wide only
  
  Allele() {
    a = 0;
  }
  
  Allele(char c) {
    a = c;
  }
  
  operator char() const{
    return a;
  }
  
  operator unsigned int() const { return a; }
  
  Allele& operator=(const unsigned i) {
    a = i;
    return *this;
  }
  
  Allele& operator=(const Allele& other) {
    a = other.a;
    return *this;
  }
  
  void operator++(int) { 
    a++;
  }
  
  Allele operator^(const Allele &other) const {
    Allele res = {a^other.a};
    return res;
  }
  
  Allele operator<<(const unsigned int shift) const {
    Allele res = {a<<shift};
    return res;
  }
};

class GEListGenome:public GAListGenome<Allele>{
public:

  GEListGenome();
  GEListGenome(const GEListGenome& source);
  virtual ~GEListGenome();

  virtual GEListGenome* clone(GAGenome::CloneMethod method) const;
  virtual void copy(const GAGenome& source);
  virtual int equal(const GAGenome& source) const;

  GEListGenome operator=(const GAGenome& source);

  int getEffectiveSize() const;
  void setEffectiveSize(const int newEffSize);
  static int effCrossover(const GAGenome& p1,
			  const GAGenome& p2,
			  GAGenome* c1, 
			  GAGenome* c2);
  static int pointMutator(GAGenome& g, float pmut);


private:

  void helpCopy(const GEListGenome& source);
  int helpCompare(const GEListGenome& source) const;

  unsigned int effSize;
};

#endif

