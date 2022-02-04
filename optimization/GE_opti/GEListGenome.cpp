// GEListGenome.cpp -*- C++ -*-
#ifndef _GELISTGENOME_CPP
#define _GELISTGENOME_CPP

#include"GEListGenome.h"

using namespace std;

bool operator==(const Allele& a1, const Allele& a2) {
    return a1.a == a2.a;
}
  
bool operator!=(const Allele& a1, const Allele& a2) {
    return !(a1 == a2);
}

GEListGenome::GEListGenome()
  : effSize(0)
{
}

GEListGenome::GEListGenome(const GEListGenome& source)
{
  this->copy(source);
}

GEListGenome::~GEListGenome()
{
}

GEListGenome* GEListGenome::clone(GAGenome::CloneMethod) const
{
  return new GEListGenome(*this);
}

void GEListGenome::copy(const GAGenome& source)
{
  if (&source != this)
  {
    GAListGenome<Allele>::copy(source);
    const GEListGenome& geListGenome = 
      static_cast<const GEListGenome&>(source);
    this->helpCopy(geListGenome);
  }
}

int GEListGenome::equal(const GAGenome& source) const
{
  if (GAListGenome<Allele>::equal(source))
  {
    return 1;
  }

  const GEListGenome& geListGenome = static_cast<const GEListGenome&>(source);
  return this->helpCompare(geListGenome);
}

GEListGenome GEListGenome::operator=(const GAGenome& source)
{
    this->copy(source);
    return *this;
}

void GEListGenome::helpCopy(const GEListGenome& source)
{
  effSize = source.effSize;
}

int GEListGenome::helpCompare(const GEListGenome& source) const
{
  if (&source == this)
  {
    return 1;
  }

  if (effSize == source.effSize)
  {
    return 1;
  }

  return 0;
}

int GEListGenome::getEffectiveSize() const{
	return effSize;
}
void GEListGenome::setEffectiveSize(const int newEffSize){
	effSize=newEffSize;
}

// Allow crossover only on effective length of individuals
int GEListGenome::effCrossover(const GAGenome &p1, 
			       const GAGenome &p2, 
			       GAGenome *c1, 
			       GAGenome *c2){
	const GEListGenome &mom=static_cast<const GEListGenome &>(p1);
	const GEListGenome &dad=static_cast<const GEListGenome &>(p2);
	int nc=0;
	int a,b;// Crossover points
	if(mom.getEffectiveSize()>mom.size()){
		// Wrapping was used, choose random location
		a = GARandomInt(0, mom.size());
	}
	else{
		// Choose point from effective length
		a = GARandomInt(0, mom.getEffectiveSize());
	}
	if(dad.getEffectiveSize()>dad.size()){
		// Wrapping was used, choose random location
		b = GARandomInt(0,dad.size());
	}
	else{
		// Choose point from effective length
		b = GARandomInt(0, dad.getEffectiveSize());
	}
	GAList<Allele> * list;
	if(c1){
		GAListGenome<Allele> &sis=
			static_cast<GAListGenome<Allele> &>(*c1);
		sis.GAList<Allele>::copy(mom);
		list = dad.GAList<Allele>::clone(b);
		if(a < mom.size()){
			Allele *site = sis.warp(a);
			while(sis.tail() != site){
				sis.destroy();		// delete the tail node
			}
			sis.destroy();		// trash the trailing node (list[a])
		}
		else{
			sis.tail();		// move to the end of the list
		}
		sis.insert(list);		// stick the clone onto the end
		delete list;
		sis.head();			// set iterator to head of list
		nc += 1;
	}
	
	if(c2){
		GAListGenome<Allele> &bro=
			static_cast<GAListGenome<Allele> &>(*c2);
		bro.GAList<Allele>::copy(dad);
		list = mom.GAList<Allele>::clone(a);
		if(b < dad.size()){
			Allele *site = bro.warp(b);
			while(bro.tail() != site){
				bro.destroy();		// delete the tail node
			}
			bro.destroy();		// trash the trailing node (list[a])
		}
		else{
			bro.tail();		// move to the end of the list
		}
		bro.insert(list);		// stick the clone onto the end
		delete list;
		bro.head();			// set iterator to head of list
		nc += 1;
	}
	return nc;
}

//Mutate each bit of genome with probability pmut
int GEListGenome::pointMutator(GAGenome & g, float pmut){
	GAListGenome<Allele> &genome= static_cast<GAListGenome<Allele> &>(g);
	register Allele mutator;
	register int nMut = 0;
	unsigned int n = genome.size();
	unsigned int bits = 4;

	//Don't mutate if prob. mut. is <= 0
	if(pmut <= 0.0) return 0;
	//Go through each codon
	for(unsigned int ii=0;ii<n;ii++){
		mutator=0;
		//Go through each bit
		for(unsigned int jj=0;jj<bits;jj++){
			mutator=mutator<<1;
			//Flip the bit with prob. pmut
			if(GAFlipCoin(pmut)){
				mutator++;
				nMut++;
			}
		}
		//Update the genome
		*genome[ii]=*genome[ii]^mutator;
	}
	//Return number of mutations made
	return nMut;
}


#endif

