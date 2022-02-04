MAX_STATES = 4
filename = "grammar.bnf"

file = open(filename,'w')
grammar = ""

grammar += "<automaton> ::= --nstates 1 <module>"
for stateTATE in range(2,MAX_STATES+1):
    grammar += " | --nstates "+str(stateTATE)
    for i in range(0,stateTATE):
        grammar += " <state"+str(stateTATE)+">"
grammar +="\n"

grammar += "<module> ::= --sB 0 --rwmB <RW> | --sB 1 | --sB 2 | --sB 3 | --sB 4 --attB <AttRep> | --sB 5 --repB <AttRep>\n"

for NSTATE in range(2,MAX_STATES+1):
    grammar += "<state"+str(NSTATE)+"> ::="
    for i in range(1,NSTATE):
        grammar += " <module> --nB "+str(i)
        for j in range(0,i):
            grammar += " <ntrans"+str(NSTATE)+"> <transition>"
        if(i<NSTATE-1):
            grammar += " |"
        else:
            grammar+="\n"

for NSTATE in range(2,MAX_STATES+1):
    MAX_TRANS = NSTATE - 1
    grammar += "<ntrans"+str(NSTATE)+"> ::="
    for TRANS_STATE in range(0,MAX_TRANS):
        grammar += " --nBxT "+str(TRANS_STATE)
        if(TRANS_STATE<MAX_TRANS-1):
            grammar += " |"
        else:
            grammar+="\n"


grammar += "<transition> ::= --cBxT 0 --pBxT <P> | "
grammar += "--cBxT 1 --pBxT <P> | "
grammar += "--cBxT 2 --pBxT <P> | "
grammar += "--cBxT 5 --pBxT <P> |"
grammar += "--cBxT 3 --pBxT <B> --wBxT <W> | "
grammar += "--cBxT 4 --pBxT <B> --wBxT <W>\n"

grammar += "<RW> ::= <digit><digit>\n"
grammar += "<AttRep> ::= 0.<digit><digit> | 1.<digit><digit> | 2.<digit><digit> | 3.<digit><digit> | 4.<digit><digit>\n"
grammar += "<P> ::= 0.<digit><digit>\n"
grammar += "<B> ::= 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10\n"
grammar += "<W> ::= <digit>.<digit><digit> | 1.<digit><digit>\n"
grammar += "<digit> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9"

file.write(grammar)
file.close()
