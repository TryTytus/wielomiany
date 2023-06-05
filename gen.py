A = []
with open("eee.txt", "r") as r:
    A = r.readlines()

with open("ggg.txt", "w") as w:
    for l in A:
        B = l.replace("\n", "").split("|")
        D = B[0].split(",")
        E = B[1].split(",")
        s = f'cout << POLYNONIAL({str(len(D) - 1) + "," + ", ".join(D)}) + POLYNONIAL({str(len(E) - 1) + "," +", ".join(E)}) << endl;\n'
        w.write(s)