// main.cpp -*- C++ -*-
#ifndef _MAIN_CPP_
#define _MAIN_CPP_

#include <iostream>
#include <string>
#include <ga/ga.h>
#include <cstdlib>
#include <sstream>

//#include "AutomodeGenome.h"

#define GENOME_SIZE 358

using namespace std;

/*
 * Flush best individual
 */
void FlushBest(const GAGenome& g,
               size_t un_generation) {
   cout << "Generation #" << un_generation
        << " " << g.score() << " "
        << g    // then write the actual values
        << endl;
}

float objfunc(GAGenome &g)
{
    stringstream call;
    call <<"python3 optimization/exp_run.py --genotype "<<g<<" > resGA.txt";
    system(call.str().c_str());

    ifstream res_file("resGA.txt");
    stringstream buffer;
    buffer << res_file.rdbuf();

    float fitness = stof(buffer.str());
    //cout <<"Genotype: "<<g<<endl << fitness << endl << endl;
    return fitness;
}
 
/****************************************/
/****************************************/
 
int main(int argc, char** argv) {
        int max_states = 4;
        int max_trans = max_states - 1;
    /*
        * Initialize GALIB
        */
    /* Create a genome with GENOME_SIZE genes, using objfunc() to evaluate it */
    GA1DBinaryStringGenome cGenome(GENOME_SIZE,objfunc,(void *)0);
    
    /*cGenome.push_back(new AutomodeBaseAllele("--nstates",1,max_states));
    for(int i=0; i<max_states; ++i) {   
        cGenome.push_back(new AutomodeBaseAllele("--sB",0,5));
            
        AutomodeAltAllele* pB = new AutomodeAltAllele("--pB");
        pB->push_back(1,100);
        pB->push_back(0,0);
        pB->push_back(0,0);
        pB->push_back(0,0);
        pB->push_back(1.0,5.0,true);
        pB->push_back(1.0,5.0,true);
        cGenome.push_back(pB);
        
        cGenome.push_back(new AutomodeBaseAllele("--nB",0,max_trans));
        for(int j=0; j<max_trans;++j) {
            cGenome.push_back(new AutomodeBaseAllele("--nBxT",0,max_trans));
            cGenome.push_back(new AutomodeBaseAllele("--cBxT",0,5));
            
            AutomodeAltAllele* pBxT = new AutomodeAltAllele("--pBxT");
            pBxT->push_back(0.0,1.0,true);
            pBxT->push_back(0.0,1.0,true);
            pBxT->push_back(0.0,1.0,true);
            pBxT->push_back(1,10);
            pBxT->push_back(1,10);
            pBxT->push_back(0.0,1.0,true);
            cGenome.push_back(pBxT);
            
            AutomodeAltAllele* wBxT = new AutomodeAltAllele("--wBxT");
            wBxT->push_back(0,0);
            wBxT->push_back(0,0);
            wBxT->push_back(0,0);
            wBxT->push_back(0.0,20.0,true);
            wBxT->push_back(0.0,20.0,true);
            wBxT->push_back(0,0);
            cGenome.push_back(wBxT);
        }
    }    */
    
    /* Create and configure a basic genetic algorithm using the genome */
    GASimpleGA cGA(cGenome);
    cGA.maximize();                     // the objective function must be minimized
    cGA.populationSize(100);              // population size for each generation
    cGA.nGenerations(50);              // number of generations
    cGA.pMutation(0.001f);               // prob of gene mutation
    cGA.pCrossover(0.06f);              // prob of gene crossover
    cGA.scoreFilename("evolution.dat"); // filename for the result log
    cGA.flushFrequency(1);              // log the results every generation
    /*
        * Launch the evolution, setting the random seed
        */
    cGA.initialize(12345);
    do {
        cGA.step();
        if(cGA.generation() % cGA.flushFrequency() == 0) {
            // Flush scores
            cGA.flushScores();
            // Flush best individual
            FlushBest(dynamic_cast<const GAGenome&>(cGA.statistics().bestIndividual()), cGA.generation());
        }
        cout << std::endl;
    }
    while(! cGA.done());
    
    // All is OK
    return 0;
}

#endif
