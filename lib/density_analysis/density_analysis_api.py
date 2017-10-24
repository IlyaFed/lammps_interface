import os

dir = "/Users/scientist/sciwork/hydrogen_dense/src/others/lammps_interface/density_analysis/"
if not os.system("g++ -std=c++11 -ggdb " + dir + "lib/density.cpp " + dir + "lib/areas.cpp " + dir + "lib/atom.cpp " + dir + "lib/box.cpp " + dir + "main.cpp -o " + dir + "density_analysis"):
    print "error in compilation"


def xyz_collect(filename, outputname, wall):
    """
        Read xyz file and plot density of electrons
        :param filename: name of .xyz file
        :param outputname: name of output file
        :param wall: len of wall
        :return: 0
    """
    print "xyz collect ...",
    print dir + "density_analysis -xyz_collect "+filename+ " " + str(wall) + " -convchg " + outputname
    ret = os.system(dir + "density_analysis -xyz_collect "+filename+ " " + str(wall) + " -convchg " + outputname)
    print "complete "
    return ret

def chg(filename, outputname):
    """
    Read CHGCAR file and split particles into molecules. NOT TESTED
    :param filename: name of CHGCAR file
    :param outputname: name of output file
    :return: 0
    """

def chg_mov(filename):
    """
    Read CHGCAR files and split particles into molecules. DONT WORK YET
    :param filename: name of CHGCAR file
    :param outputname: name of output file
    :return: 0
    """


