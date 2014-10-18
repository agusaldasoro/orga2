#!/usr/bin/env python

f = open("mediciones",'r')
lines = f.readlines()

def suma(arr):
    ret = 0
    for i in range(len(arr)):
        ret += int(arr[i])

    return ret

for i in range(4):
    filtro = lines[0]
    print filtro[:len(filtro)-1]

    ret = suma(lines[1:10])
    print "10 pasadas: " + str(ret/float(10))

    ret = suma(lines[11:110])
    print "100 pasadas: " + str(ret/float(100))

    ret = suma(lines[111:310]);
    print "200 pasadas: " + str(ret/float(200))

    ret = suma(lines[311:1310])
    print "1000 pasadas: " + str(ret/float(1000))

    lines = lines[1311:]

