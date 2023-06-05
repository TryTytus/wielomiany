from numpy import poly1d, polyadd
import numpy

with open("eee.txt", "r") as r:
    for l in r.readlines():
        g = l.replace("\n", "")
        a = g.split("|")
        e1 = [int(i) for i in a[0].split(",")][::-1]
        e2 = [int(i) for i in a[1].split(",")][::-1]
        pol1 = poly1d(e1)
        pol2 = poly1d(e2)
        pol_out = polyadd(pol1, pol2)
        aa = list(pol_out)
        lc = numpy.gcd.reduce(aa)
        aa = [ int(x / lc) for x in aa][::-1]
        bb = ", ".join([str(i) for i in aa])
        print(f'( {bb} )')