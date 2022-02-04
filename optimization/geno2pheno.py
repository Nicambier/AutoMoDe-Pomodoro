import random
import sys

MAX_STATES = 4
MAX_TRANS = MAX_STATES - 1

ranges = {}
ranges['--nstates'] = (1,MAX_STATES)
ranges['--sB'] = (0,5)
ranges['--pB'] = [(1,100),(0,0),(0,0),(0,0),(1.00,5.00),(1.00,5.00)]
ranges['--nB'] = (0,MAX_TRANS)

ranges['--nBxT'] = (0,MAX_TRANS)
ranges['--cBxT'] = (0,5)
ranges['--pBxT'] = [(0.00,1.00),(0.00,1.00),(0.00,1.00),(1,10),(1,10),(0.00,1.00)]
ranges['--wBxT'] = [(0,0),(0,0),(0,0),(0.00,20.00),(0.00,20.00),(0,0)]

tx = MAX_TRANS * ['--nBxT', '--cBxT', '--pBxT', '--wBxT']
astate =  ['--sB', '--pB', '--nB'] + tx
chain = ['--nstates'] + MAX_STATES * astate


state_params = ['--rwmB','','','','--attB','--repB']

def increment(bitvector):
    res = ''
    for i in range(1,len(bitvector)+1):
        if(bitvector[-i]=='0'):
            res = bitvector[0:-i]+'1'+res
            break
        else:
            res = '0'+res

    return res

def bit_size(arange):
    range_size = arange[1] - arange[0] + 1
    if(type(arange[0])==type(1.0)):
        range_size *= 100
    i = 0
    while(pow(2,i)<range_size):
        i+=1
    return i

def max_range(rangelist):
    max_size = 0
    max_range = (0,0)
    for arange in rangelist:
        s = bit_size(arange)
        if(s>max_size):
            max_size = s
            max_range = arange

    return max_range

def genome_size():
    bit_count = 0
    for key in chain:
        arange = ranges[key]
        if(type(arange) != type(())):
            arange = max_range(arange)
        bit_count += bit_size(arange)
    return bit_count

def toPhenotype(genome):
    phenotype = ''
    i = 0
    good_genome = True
    nstates = MAX_STATES
    ntrans = MAX_TRANS
    curB = -1
    curT = -1
    last_id = 100
    for key in chain:
        elem = ranges[key]
        if(type(elem) == type(())):
            arange = elem
            datasize = bit_size(arange)
        else:
            arange = elem[last_id]
            datasize = bit_size(max_range(elem))
        
        if(arange[1]-arange[0]>0):
            if(key == '--nB'):
                arange = (arange[0],nstates-1)
            elif(key == '--nBxT'):
                arange = (arange[0],nstates-2) #substract two because max transitions is nstates-1 and because count starts at 0
            
            bit_val = int(genome[i:i+datasize],2)
            mul = 1
            max_value = max(arange[1] - arange[0],0)
            if(type(arange[0])==type(1.0)):
                mul = 0.01
            
            val = round(arange[0] + (bit_val % (max_value+1))*mul, 2)

            if(key == '--nstates'):
                nstates = val
            elif(key == '--sB'):
                curB = curB + 1
                ntrans = MAX_TRANS #reinitialise ntrans to pass condition until we know
                curT = -1
                last_id = val
            elif(key == '--nB'):
                ntrans = val
                if(ntrans == 0):
                    ntrans = -1 #doesn't pass condition below so that -nB 0 is not displayed
            elif(key == '--nBxT'):
                curT = curT + 1
            elif(key == '--cBxT'):
                last_id = val
            #print(key,nstates,curB,ntrans,curT)
            if(nstates>curB and ntrans>curT):
                if(key == '--pB'):
                    key = state_params[last_id]
                phenotype += key.replace('B',str(curB)).replace('T',str(curT)) + ' ' + str(val) + ' '
        i+=datasize

    return phenotype

def main(args = sys.argv[1:]):
    if(len(args)>0):
        print(toPhenotype(args[0]))
    else:
        genotype = ''.join([str(random.randint(0, 1)) for i in range(genome_size())])
        phenotype = toPhenotype(genotype)

        print(genome_size(),genotype,phenotype,end='\n')

if __name__ == '__main__':
    main()
    

