#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include <iostream>
#include <fstream>

#include<ga/ga.h>
#include<GE/ge.h>
#include "GEListGenome.h"

using namespace std;

// GE mapper, declared in main.cpp
extern GEGrammarSI mapper;

void replace_with_nb(string& s, size_t pos, unsigned int nb) {
    s.erase(pos,1);
    s.insert(pos, to_string(nb));
}

//Loads the start and end code required to compile phenotypes,
//and initialises the mapper
void app_init(unsigned int wrappingEvents, string grammarFile){


  	/* Init GE mapper */
	//Set maximum number of wrapping events per mapping
	mapper.setMaxWraps(wrappingEvents);
	//Load grammar
	if (!mapper.readBNFFile(grammarFile)){
	        cerr << "Could not read "<< grammarFile << "\n";
		cerr << "Execution aborted.\n";
		exit(0);
	}
  }

//Attributes a fitness score to a genome
float objfunc(GAGenome &g)
{
    GEListGenome &genome = static_cast<GEListGenome&>(g);
	//Assign genotype to mapper
	mapper.setGenotype(genome);
	//Grab phenotype
    Phenotype const *phenotype=mapper.getPhenotype();
	if(phenotype->getValid())
	  {
		float fitness;

		// Set effective size of genome
		genome.setEffectiveSize(mapper.getGenotype()->getEffectiveSize());
        string phenotype = mapper.getPhenotype()->getString();
        
        unsigned int t, b = 0;
        size_t next_state, this_state = phenotype.find("--sB");
        while(this_state != string::npos) {
            replace_with_nb(phenotype,this_state+3,b);
            next_state = phenotype.find("--sB");
            t=-1;
            size_t next_connection = phenotype.find("--nBxT");
            size_t end_state = next_state;
            if(end_state==string::npos) end_state = phenotype.size();
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
        string argos_call = "python3 optimization/exp_run.py " + phenotype + " > resGE.txt";
        system(argos_call.c_str());

        ifstream res_file("resGE.txt");
        stringstream buffer;
        buffer << res_file.rdbuf();

        fitness = stof(buffer.str());
        //cout << phenotype << " (" << genome.getEffectiveSize() << ", "<< fitness << ")\n";
		return fitness;
	  }
	else {
	  return 0;
    }
}

//Print an individual to stdout
void print_individual(const GAGenome &g)
{
	GAListGenome<unsigned char> &genome =
		(GAListGenome<unsigned char> &) g;
	//Assign genotype to mapper
	mapper.setGenotype(genome);
	cout << "Best individual:\n";
	//Print phenotype
        cout << "Score = " << g.score() << "\n";
	cout << *(mapper.getPhenotype());
	cout << endl;
	cout << "Genotype = " << *mapper.getGenotype() << "\n";
	cout << "Total length     = "
		<< mapper.getGenotype()->size() << "\n";
	cout << "Effective length = "
		<< mapper.getGenotype()->getEffectiveSize() << "\n";
	cout << "Wrapping events = "
		<< mapper.getGenotype()->getWraps() << "\n";
}

