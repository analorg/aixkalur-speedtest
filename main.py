#!/usr/bin/python
# -*- coding: utf-8 -*-

#Elapsed time: 159031 μs
#Elapsed time: 0.159031 s
#Results:  ['irask', 'kirsa', 'raisk', 'rakis', 'rikas', 'sakri']
import io
import sys
import re
from datetime import datetime
import multiprocessing as mp

#import chardet

if (len(sys.argv) < 3):
    print ("python v1.py /home/awesomedev/dict.txt word")
    exit(0)

start = datetime.now()

word = sys.argv[2].lower()
stat = {}

wordLength = 0
for i in word:
    #chr = i.lower()
    chr = i
    wordLength = wordLength + 1
    stat[chr] = 1 if chr not in stat else stat[chr] + 1

results = []
with io.open(sys.argv[1], mode='r', encoding='windows-1257') as f:
    #text = f.read()
    lines = f.readlines()
    for text in lines:
        if (text[-1] == '\n'):
            text = text[:-1]
        if (text[-1] == '\r'):
            text = text[:-1]
        l = len(text)
        if (l < wordLength or l > (2*wordLength) - 2) or text == word:
            continue
        s = dict(stat)
        #print (s)
        for i in text:
            char = i.lower()
            if (char not in s):
                s = False
                break
            s[char] = s[char] - 1
            if (s[char] == 0):
                del s[char]
        if (s != False and len(s) == 0):
            results.append(text)
    f.close()

end = datetime.now()
delta = end-start
print str(delta.microseconds) + "," + str(",".join(results))