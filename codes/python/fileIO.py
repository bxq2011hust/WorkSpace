#!/usr/bin/python3

# Open a file
fo = open("./foo.txt", "r+")
print ("Name of the file: ", fo.name)
print ("Closed or not : ", fo.closed)
print ("Opening mode : ", fo.mode)

sstr = fo.read()
print ("Read String is : ", sstr)

fo.close()