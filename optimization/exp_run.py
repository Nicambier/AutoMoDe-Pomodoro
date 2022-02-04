import subprocess
import sys
from statistics import mean
import geno2pheno

SAMPLE = 10

def run(phenotype):
    processes = []
    for i in range(SAMPLE):
        call = "../ARGoS3-AutoMoDe/bin/automode_main -c experiments/current_exp/experiment.argos --fsm-config "+phenotype
        p = subprocess.Popen(call, shell=True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
        processes.append(p)
    res = []
    for p in processes:
        output = p.communicate()[0].decode()
        score_idx = output.rfind("Score")+6
        score = float(output[score_idx:output.find("\n",score_idx)])
        res.append(score)
    return mean(res)

def main(args = sys.argv[1:]):
    if(len(args)>0):
        if(args[0]=="--genotype"):
            print(run(geno2pheno.toPhenotype(args[1])))
        else:
            print(run(' '.join(args)))

if __name__ == '__main__':
    main()
        
