#!/usr/bin/env python

import matplotlib
matplotlib.use('Agg')
import argparse
import matplotlib.pyplot as plt

#By Lucas Antonsen, V00923982

def create_graph(arglist, flag):
    y50 = arglist[:6]
    y100 = arglist[6:12]
    y150 = arglist[12:18]
    y200 = arglist[18:]
    x = [0, 5, 10, 15, 20, 25]

    # plotting the q=50 points 
    plt.plot(x, y50, label = "q=50", marker='o')
      
    # plotting the q=100 points 
    plt.plot(x, y100, label = "q=100", marker='*')

    # plotting the q=150 points 
    plt.plot(x, y150, label = "q=150", marker='+')

    # plotting the q=200 points 
    plt.plot(x, y200, label = "q=200", marker='x')

    plt.xlabel('Dispatch overhead')

    if flag == 'ta':
        plt.ylabel('Average turnaround time')
        fname = 'graph_turnaround.pdf'
    elif flag == 'w':
        plt.ylabel('Average waiting time')
        fname = 'graph_waiting.pdf'
        
    plt.title('Round Robin scheduler -- # tasks:1000;seed value: varies')
    plt.legend()
      
    # function to save the plot
    plt.savefig(fname)

    return

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('integers', metavar ='N', type = int, nargs='+')
    parser.add_argument('--flag', type = str, choices = ["ta", "w"])
    args = parser.parse_args()

    create_graph(args.integers, args.flag)

if __name__ == "__main__":
    main()
