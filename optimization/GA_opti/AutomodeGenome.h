#include <ga/ga.h>
#include <sstream>
#include "AutomodeAllele.h"

class AutomodeGenome: public GA1DBinaryStringGenome {
public:
    AutomodeGenome(GAGenome::Evaluator f=(GAGenome::Evaluator)0, void * u=(void *)0): GA1DBinaryStringGenome(0,f,u)  {}
    AutomodeGenome(const AutomodeGenome& g): GA1DBinaryStringGenome(g) {copy(g);}
    ~AutomodeGenome() {for(unsigned int i=0; i<alleles.size(); ++i) delete alleles[i]; alleles.clear();}

    AutomodeGenome* clone(GAGenome::CloneMethod) const
    {
        return new AutomodeGenome(*this);
    }

    void copy(const AutomodeGenome& g)
    {
        if (&g != this)
        {
            GA1DBinaryStringGenome::copy(g);
            for(std::vector<AutomodeAllele*>::const_iterator iter=g.alleles.begin();iter!=g.alleles.end();++iter) 
                push_back(&(*iter)->clone());
        }
    }

    int equal(const GAGenome& g) const
    {
        return GA1DBinaryStringGenome::equal(g);
    }

    AutomodeGenome operator=(const AutomodeGenome& g) {
        this->copy(g);
        return *this;
    }
    
    
    long unsigned int push_back(AutomodeAllele* a) {alleles.push_back(a); return length(length() + a->getLength());}
    
    std::string getPhenotype() const {
        std::stringstream rawPheno;
        int currIdx = 0;
        for(unsigned int i=0; i<alleles.size(); ++i) {
            rawPheno << alleles[i]->getName()<< " "<< alleles[i]->value(data,currIdx,0)<<" ";
            currIdx += alleles[i]->getLength();
        }
        
        std::string phenotype = rawPheno.str();
        unsigned int t, b = 0;
        size_t next_state, this_state = phenotype.find("--sB");
        while(this_state != std::string::npos) {
            replace_with_nb(phenotype,this_state+3,b);
            next_state = phenotype.find("--sB");
            t=-1;
            size_t next_connection = phenotype.find("--nBxT");
            size_t end_state = next_state;
            if(end_state==std::string::npos) end_state = phenotype.size();
            for(size_t i=this_state; i<end_state; ++i) {
                if(phenotype[i]=='B') {
                    replace_with_nb(phenotype,i,b);
                } else if(phenotype[i]=='T') {
                    if(i>next_connection) {
                        next_connection = phenotype.find("--nBxT");
                        ++t;
                    }
                    replace_with_nb(phenotype,i,t);
                }
            }
            this_state = next_state;
            ++b;
        }
        
        return phenotype;
    }
    
private:
    void static replace_with_nb(std::string& s, size_t pos, unsigned int nb) {
        s.erase(pos,1);
        s.insert(pos, std::to_string(nb));
    }
    
    
    std::vector<AutomodeAllele*> alleles;
};
