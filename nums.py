from random import randint

with open("eee.txt", "w") as w:
    for i in range(1000):
        num1 = randint(1, 10)
        pol1 = ",".join([str(randint(-200, 200)) for i in range(num1)])
        num2 = randint(1, 10)
        pol2 = ",".join([str(randint(-200, 200)) for i in range(num1)])
        sss= pol1 + "|" + pol2 + "\n"
        w.write(sss)

