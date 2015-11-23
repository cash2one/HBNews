# -*- coding: UTF-8 -*-

def add(t):
	a = 0;
	for i in xrange(len(t)):
		print "len(t)", len(t)
		print "t[i]"
		a += t[i]
	print "a: ", a
	return a

"""	
def add(a, b):
	print "a: ", a
	print "b: ", b
	return a + b
	
"""