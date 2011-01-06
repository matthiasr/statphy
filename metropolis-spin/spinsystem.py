from random import random, randint

start_mag = 0.1
N = 1000
beta = -1

system = [(random() < (start_mag+1)*0.5) for i in xrange(N)]

spinflip = lambda system,pos: [s if i!=pos else (not s) for i,s in enumerate(system)]

spinval = lambda (s): 1. if s else -1.

H = lambda (system): sum( sum ( spinval(s)*spinval(ss)/(abs(i-j)) for j,ss in enumerate(system) if i!=j ) for i,s in enumerate(system) )

mag = lambda system: sum( spinval(s) for s in system) / N

p_accept = lambda system,pos: min( H(spinflip(system,pos)) - H(system), 1 )

for t in xrange(10**3):
    pos = randint(0,N-1)
    if random() < p_accept(system,pos):
        system = spinflip(system,pos)

    print mag(system)
