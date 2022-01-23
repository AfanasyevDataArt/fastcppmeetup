#!/bin/python3

import seaborn as sns
import pandas as pd
import json
import argparse

import matplotlib.pyplot as plt
sns.set_theme(style="ticks", color_codes=True)


def test():
    tips = sns.load_dataset("tips")
    sns.catplot(x="day", y="total_bill", data=tips)

    plt.savefig('test.png')


def print_graph(inf, outf):
    data = open(inf).readlines()
    data_set = {}
    for line in data:
        jline = json.loads(line)
        key = str(jline['n'])
        if key not in data_set:
            data_set[key] = []
        data_set[key].append(jline['time_ms'])

    pdata = pd.DataFrame(data_set)
    sns.catplot(data=pdata)

    plt.savefig(outf)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Draw graph from file')
    parser.add_argument('--infile', type=str, help='Input file')
    parser.add_argument('--outfile', type=str, help='Output file')

    args = parser.parse_args()

    #test()
    print_graph(args.infile, args.outfile)
