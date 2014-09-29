#!/usr/bin/env python

f = open("mediciones",'r')
lines = f.readlines()

def suma(arr):
    ret = 0
    for i in range(len(arr)):
        ret += int(arr[i])

    return ret

def restarEspe(arr, esp):
    for i in range(len(arr)):
        arr[i]  = float(arr[i]) - esp
	arr[i] *= int(arr[i])
    return arr

for i in range(4):
    filtro = lines[0]
    print filtro[:len(filtro)-1]

    ret = suma(lines[1:10])
    esp = ret/float(10)
    tmp = restarEspe(lines[1:10], esp)
    ret = suma(tmp)
    print "10 pasadas: " + str(ret/float(10))

    ret = suma(lines[11:110])
    esp = ret/float(100)
    restarEspe(lines[11:110], esp)
    ret = suma(lines)
    print "100 pasadas: " + str(ret/float(100))

    ret = suma(lines[111:310])
    esp = ret/float(200)
    tmp = restarEspe(lines[111:310], esp)
    ret = suma(tmp);
    print "200 pasadas: " + str(ret/float(200))

    ret = suma(lines[311:1310])
    esp = ret/float(1000)
    tmp = restarEspe(lines[311:1310], esp)
    ret = suma(tmp)
    print "1000 pasadas: " + str(ret/float(1000))

    lines = lines[1311:]

