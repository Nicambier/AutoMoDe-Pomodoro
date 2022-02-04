import os
import sys
import time

from datetime import datetime
import numpy as np

PYTHONPATH = 'optimization/diffGA'

sys.path.append(os.path.dirname(os.path.expanduser(PYTHONPATH)))

from algorithms.population_algorithm import OptimizationAlgorithm, DifferentialEvolution
from exp_run import run

pop_size = 100

num_generations = 50

de_types = ['de', 'dex3', 'ade', 'revde']
de_type = de_types[0]

MAX_STATES = 4
MAX_TRANS = MAX_STATES - 1

ranges = {}
ranges['--nstates'] = (1,MAX_STATES)
ranges['--sB'] = (0,5)
ranges['--pB'] = (1,100)
ranges['--bB'] = (1.00,5.00)
ranges['--nB'] = (0,MAX_TRANS)

ranges['--nBxT'] = (0,MAX_TRANS)
ranges['--cBxT'] = (0,5)
ranges['--pBxT'] = (0.00,1.00)
ranges['--bBxT'] = (1,10)
ranges['--wBxT'] = (0.00,20.00)

tx = MAX_TRANS * ['--nBxT', '--cBxT', '--pBxT', '--bBxT', '--wBxT']
astate =  ['--sB', '--pB', '--bB', '--nB'] + tx
chain = ['--nstates'] + MAX_STATES * astate

spec_params = {}
spec_params['--pB'] = ['--rwmB','','','','','']
spec_params['--bB'] = ['','','','','--attB','--repB']
spec_params['--pBxT'] = ['--pBxT','--pBxT','--pBxT','','','--pBxT']
spec_params['--bBxT'] = ['','','','--pBxT','--pBxT','']
spec_params['--wBxT'] = ['','','','--wBxT','--wBxT','']

min_bounds = [ranges[key][0] for key in chain]
max_bounds = [ranges[key][1] for key in chain]
bounds = [min_bounds,max_bounds]
D = len(bounds[0])

def to_phenotype(args):
    phenotype = ''
    i = 0
    nstates = MAX_STATES
    ntrans = MAX_TRANS
    curB = -1
    curT = -1
    last_id = 100
    for key_id in range(len(chain)):
        key = chain[key_id]
        elem = args[key_id]

        if(type(ranges[key][0])==type(1)):
            val = int(round(elem,0))
        else:
            val = round(elem,2)

        if(key == '--nB'):
            if(nstates-1>0):
                val %= nstates-1
            else:
                val = 0
        elif(key == '--nBxT'):
            if(nstates-2>0): #substract two because max transitions is nstates-1 and because count starts at 0
                val %= nstates-2
            else:
                val = 0

        if(key == '--nstates'):
            nstates = val
        elif(key == '--sB'):
            curB = curB + 1
            ntrans = MAX_TRANS #reinitialise ntrans to pass condition until we know
            curT = -1
            last_id = int(val)
        elif(key == '--nB'):
            ntrans = val
            if(ntrans == 0):
                ntrans = -1 #doesn't pass condition below so that -nB 0 is not displayed
        elif(key == '--nBxT'):
            curT = curT + 1
        elif(key == '--cBxT'):
            last_id = int(val)

        if(key[:4] == '--pB' or key[:4] == '--bB' or key == '--wBxT'):
            key = spec_params[key][last_id]
        if(nstates>curB and ntrans>curT and key != ''):
            phenotype += key.replace('B',str(curB)).replace('T',str(curT)) + ' ' + str(val) + ' '
    return phenotype


def automode_run(args,params=None,y_real=None):
    return -run(to_phenotype(args))    #negative because diffGA minimizes

if __name__ == '__main__':
    np.random.seed()

    x0 = np.random.uniform(low=bounds[0], high=bounds[1], size=(pop_size, D))

    # Experiment
    objective = automode_run
    params = {}

    params['evaluate_objective_type'] = 'single'
    params['pop_size'] = pop_size
    params['CR'] = 0.9
    params['F'] = 0.5

    de = DifferentialEvolution(objective,
                               args=(params, None),
                               x0=x0,
                               bounds=(bounds[0], bounds[1]),
                               population_size=pop_size,
                               de_type=de_type)

    opt_alg = OptimizationAlgorithm(pop_algorithm=de, num_epochs=num_generations)

    res, f = opt_alg.optimize(directory_name='./results')

    for i in range(len(res)):
        print('Phenotype: ',to_phenotype(res[i]), 'Score:',f[i],sep='\n')
