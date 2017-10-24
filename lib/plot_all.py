import pandas as pd
import os


def read_log(name='./log.lammps'):
    """
    Read date from lammps log
    :param name: full name of you log
    :return: pandas of date
    """
    print "read lammps log ... ",
    read_flag = 0
    f = open(name, 'r')
    for k in range(5):
        for line in f:
            if (line[0] == 'S'):
                if (line.split()[0] == 'Step'):
                    energy = pd.DataFrame(columns=line.split())
                    break

        for line in f:
            try:
                energy.loc[len(energy)] = [float(i) for i in line.split()]
            except ValueError:
                break

    f.close()
    '''
    for num_line, line in enumerate(open(name, 'r')):
        if read_flag:
            try:
                energy.loc[len(energy)] = [float(i) for i in line.split()]
            except ValueError:
                read_flag = 0
            continue
        if line[0] == 'S':
            if (line.split()[0] == 'Step'):
                read_flag = 1
                energy = pd.DataFrame(columns=line.split())
                continue
            #if (line.split()[0] == 'Loop'):
             #   read_flag = 0
             #   continue

    '''
    energy['Step'] = energy['Step'].astype(int)
    del read_flag
    print "complete, {:d} steps".format(len(energy))
    return energy


def read_eff_temp(path='./all'):
    """
    Return temperature of eff model
    :param path: path to you lammpstr files
    :return: pandas of [step, T_el, T_at]
    """
    print "read eFF temperature ... ",
    path = path + '/'

    m_i_al = 1
    m_e = 1
    bohr = 0.5291772
    kb = 1.3806488
    Avogadro = 6.022141
    T_coeff = 2 / 3 / kb / Avogadro * 10 ** 7  # 2*4.026*10**5
    atu = 1  # .03275                     #atomic time unit

    label_to_pandas = ['step', 'T_el', 'T_nu']
    temp = pd.DataFrame(columns = label_to_pandas)

    llist = []
    for msdfile in os.listdir(path):
        if msdfile.split('.')[-1] == 'lammpstrj':
            llist.append(msdfile)
    llist = sorted(llist, key=lambda x: int(x.split('.')[1]))

    for num_file, file in enumerate(llist):
        if int(file.split('.')[-2]) % 1000 == 0:
            if file.split('.')[-1] == 'lammpstrj':
                E_al_ion = 0
                E_el_free = 0
                E_rad = 0
                n_al_ion = 0
                n_el_free = 0
                for num_line, line in enumerate(open(path + file, 'r')):
                    if num_line > 8:
                        v2 = (bohr ** 2) / (atu ** 2) * (float(line.split()[8]) ** 2 + float(line.split()[9]) ** 2 + float(
                            line.split()[10]) ** 2)  # velocity squared (A/fs)^2
                        r_vel = bohr / atu * float(line.split()[11])  # electron radius vel  A/fs
                        if line.split()[1] == '1':
                            n_al_ion += 1
                            E_al_ion += 0.5 * m_i_al * v2
                        if line.split()[1] == '2':
                            n_el_free += 1
                            E_el_free += 0.5 * m_e * v2 + 0.5 * 0.75 * m_e * r_vel * r_vel
                            E_rad += 0.5 * 0.75 * m_e * r_vel * r_vel
                temp.loc[len(temp)] = [int(file.split('.')[1]), 3 / 4 * T_coeff * E_el_free / n_el_free,
                                      T_coeff * E_al_ion / n_al_ion ]
    print "complete"
    return temp

# path = "/Users/scientist/sciwork/hydrogen_dense/src/eff_to_vasp/from eff/H_2/output/"
# read_log(path + "log.lammps")
