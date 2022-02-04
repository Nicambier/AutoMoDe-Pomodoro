#!/bin/bash
source ../argos_env.sh
mkdir experiments/current_exp

for task in 1 2 3
do
    if [ $task -eq 1 ] 
    then 
        TASK_NAME=Aggregation
        sed -e 's/exp_path/\/home\/nicolas\/experiments-loop-functions\/build\/loop-functions\/vanilla\/libaggregation_loopfunc.so/' experiments/template.argos > experiments/current_exp/experiment.argos
        sleep 0.1
        sed -i 's/exp_label/aggregation_loop_functions/' experiments/current_exp/experiment.argos
    fi
    if [ $task -eq 2 ]
    then 
        TASK_NAME=Foraging
        sed -e 's/exp_path/\/home\/nicolas\/experiments-loop-functions\/build\/loop-functions\/vanilla\/libforaging_loopfunc.so/' experiments/template.argos > experiments/current_exp/experiment.argos
        sleep 0.1
        sed -i 's/exp_label/foraging_loop_functions/' experiments/current_exp/experiment.argos
    fi
    if [ $task -eq 3 ]
    then 
        TASK_NAME=Coverage
        sed -e 's/exp_path/\/home\/nicolas\/experiments-loop-functions\/build\/loop-functions\/chocolate\/LCN\/libchocolate_lcn_loopfunc.so/' experiments/template.argos > experiments/current_exp/experiment.argos
        sleep 0.1
        sed -i 's/exp_label/chocolate_lcn_loop_functions/' experiments/current_exp/experiment.argos
    fi
    
    mkdir -p results/"$TASK_NAME"/GA
    mkdir -p results/"$TASK_NAME"/GE
    mkdir -p results/"$TASK_NAME"/chocolate
    mkdir -p results/"$TASK_NAME"/diffGA
    for i in $(seq 6 1 10) 
    do
        
        ./optimization/GA_opti/GAmode > results/"$TASK_NAME"/GA/"$i" &
        ./optimization/GE_opti/GEGCC > results/"$TASK_NAME"/GE/"$i" &
        python3 optimization/diffGA/pomodoro_diffGA.py > results/"$TASK_NAME"/diffGA/"$i" &
	mkdir results/"$TASK_NAME"/GA/exp_"$i"
	mkdir results/"$TASK_NAME"/GE/exp_"$i"
        mkdir results/"$TASK_NAME"/chocolate/exp_"$i"
	mkdir results/"$TASK_NAME"/diffGA/exp_"$i"
        cd optimization/chocolate
        irace --scenario scenario.txt > ../../results/"$TASK_NAME"/chocolate/"$i" &
        wait < <(jobs -p)
        mv ../../results/irace.Rdata ../../results/"$TASK_NAME"/chocolate/exp_"$i"/
        mv ../../results/fsm_log.txt ../../results/"$TASK_NAME"/chocolate/exp_"$i"/
        cd ../../
	mv evolution.dat results/"$TASK_NAME"/GA/exp_"$i"/
	mv is-output.dat results/"$TASK_NAME"/GE/exp_"$i"/
	mv results/f_best.npy results/"$TASK_NAME"/diffGA/exp_"$i"/
	mv results/last_f.npy results/"$TASK_NAME"/diffGA/exp_"$i"/ 
	mv results/last_x.npy results/"$TASK_NAME"/diffGA/exp_"$i"/  
    done
    rm -f experiments/current_exp/experiment.argos
done
rm -rf experiments/current_exp
