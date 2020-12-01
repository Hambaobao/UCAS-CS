import os

def Get_cwnd(path, filename):
    llist = []

    with open(str(path + filename), 'r') as f:
        with open(str(path + "get_cwnd.txt"), 'w') as g:
            line = f.readline()
            while line:
                llist = line.split(" ")
                g.write(llist[0] + ' ' + llist[1] + ' ' + llist[2] + ' ' + llist[6])
                g.write("\n")
                line = f.readline()

def Get_ping(path, filename):
    llist = []

    with open(str(path + filename), 'r') as f:
        f.readline()
        with open(str(path + "get_ping.txt"), 'w') as g:
            line = f.readline()
            while line:
                llist = line.split(" ")
                wline = llist[6].split("=")
                g.write(wline[1])
                g.write("\n")
                line = f.readline()

def Get_qlen(path, filename):
    llist = []

    with open(str(path + filename), 'r') as f:
        with open(str(path + "get_qlen.txt"), 'w') as g:
            line = f.readline()
            while line:
                llist = line.split(",")
                if(len(llist) == 2 and llist[1] != ""):
                    g.write(llist[0] + " " + llist[1])
                line = f.readline()

def Get_taildrop():
    llist = []

    with open("../taildrop/ping.txt", 'r') as f:
        f.readline()
        with open("../taildrop/get_ping.txt", 'w') as g:
            line = f.readline()
            while line:
                llist = line.split(" ")
                wline = llist[6].split("=")
                g.write(wline[1])
                g.write("\n")
                line = f.readline()

def Get_red():
    llist = []

    with open("../red/ping.txt", 'r') as f:
        f.readline()
        with open("../red/get_ping.txt", 'w') as g:
            line = f.readline()
            while line:
                llist = line.split(" ")
                wline = llist[6].split("=")
                g.write(wline[1])
                g.write("\n")
                line = f.readline()

def Get_codel():
    llist = []

    with open("../codel/ping.txt", 'r') as f:
        f.readline()
        with open("../codel/get_ping.txt", 'w') as g:
            line = f.readline()
            while line:
                llist = line.split(" ")
                wline = llist[6].split("=")
                g.write(wline[1])
                g.write("\n")
                line = f.readline()

if __name__ == "__main__":
    path = ["../qlen-10/", "../qlen-20/", "../qlen-30/", "../qlen-40/", "../qlen-50/",\
         "../qlen-60/", "../qlen-70/", "../qlen-80/", "../qlen-90/", "../qlen-100/"]
    for p in path:
        Get_cwnd(p, "cwnd.txt")
        Get_qlen(p, "qlen.txt")
        Get_ping(p, "ping.txt")

    Get_taildrop()
    Get_red()
    Get_codel()
