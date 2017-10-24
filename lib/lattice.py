import numpy as np

a_b = 0.52917720859

n_part_in_point = 4 ###

def simple_struct(x):
    """Here we define our molecule or atom in structure and return list of coord to print"""
    coord = []
    nucl_vector = np.random.rand(3)
    #nucl_vector = np.array([0.0,0.0,1.0])
    nucl_vector /= np.linalg.norm(nucl_vector)
    el_vector = np.random.rand(3)
    el_vector -= el_vector.dot(nucl_vector) * nucl_vector #make orthogonal
    el_vector /= np.linalg.norm(el_vector)
    nucl_dist = 0.25/a_b
    electron_dist = 0;
    nucl_vector = nucl_vector*nucl_dist
    el_vector = el_vector*electron_dist
    our_vect = x - nucl_vector
    coord.append(np.array([1, 1.0, 0, 0.0, our_vect[0], our_vect[1], our_vect[2]]))

    our_vect = x + nucl_vector
    coord.append(np.array([1, 1.0, 0, 0.0, our_vect[0], our_vect[1], our_vect[2]]))
    our_vect = x - el_vector
    coord.append(np.array([2, 0.0, 1, 1, our_vect[0], our_vect[1], our_vect[2]]))
    our_vect = x + el_vector
    coord.append(np.array([2, 0.0, -1, 1, our_vect[0], our_vect[1], our_vect[2]]))

    return coord

density = 0.7 # g/cc
rs = 2.0
size_a = (4.0/3*3.14*rs**3*2)**(1.0/3)
b = [0,3,0,3,0,3] #create_box
#lattice = ["hcp", 3.780/a_b, 3.780/a_b, 6.167/a_b] # a1,a2,a3
lattice = ["sc", size_a, size_a, size_a] # a1,a2,a3

mass_atom = 1.008
mass_electron = 1.008







box = [np.arange(b[0],b[1],1),np.arange(b[2],b[3],1),np.arange(b[4],b[5],1)]
style = lattice[0]

a1 = np.array([lattice[1], 0, 0])
a2 = np.array([0, lattice[2], 0])
a3 = np.array([0, 0, lattice[3]])

basis = [];
if (style == "sc"):
    basis.append(np.array([0.0,0.0,0.0]))
if (style == "bcc"):
    n_part_in_point = n_part_in_point*2
    basis.append(np.array([0.0,0.0,0.0]))
    basis.append(np.array([0.5,0.5,0.5]))
if (style == "fcc"):
    n_part_in_point = n_part_in_point*4
    basis.append(np.array([0.0,0.0,0.0]))
    basis.append(np.array([0.5,0.5,0.0]))
    basis.append(np.array([0.5,0.0,0.5]))
    basis.append(np.array([0.0,0.5,0.5]))
if (style == "hcp"):
    n_part_in_point = n_part_in_point*4
    basis.append(np.array([0.0,0.0,0.0]))
    basis.append(np.array([0.5,0.5,0.0]))
    basis.append(np.array([0.5,5.0/6.0,0.5]))
    basis.append(np.array([0.0,1.0/3.0,0.5]))
    a2[1] *= 3.0**0.5
    a3[2] *= (8.0/3.0)**0.5
if (style == "sq"):
    basis.append(np.array([0.0,0.0,0.0]))
if (style == "sq2"):
    n_part_in_point = n_part_in_point*2
    basis.append(np.array([0.0,0.0,0.0]))
    basis.append(np.array([0.5,0.5,0.0]))
if (style == "hex"):
    n_part_in_point = n_part_in_point*2
    a2[1] *= 3**0.5
    basis.append(np.array([0.0, 0.0, 0.0]))
    basis.append(np.array([0.5, 0.5, 0.0]))



N = (b[1]-b[0])*(b[3]-b[2])*(b[5]-b[4])*n_part_in_point
#make necessary density
#scale_to_density = (1.0*n_part_in_point*(mass_atom+mass_electron)*1.66e-24/2/(density*a1[0]*a2[1]*a3[2]*a_b**3*1e-24) )**(1.0/3)
#a1[0] *=scale_to_density
#a2[1] *=scale_to_density
#a3[2] *=scale_to_density

dfile = open("h.data", "w")
#dfile.write(str(N*n_part_in_point) + "\nAtoms. Timestep: 0\n")
print "File name: ", dfile.name
dfile.write("Date_file to box block\n\n")
dfile.write(str(N)+" atoms\n2 atom types\n\n")
dfile.write(str(b[0]*a1[0])+ " " +str(b[1]*a1[0])+" xlo xhi\n")
dfile.write(str(b[2]*a2[1])+ " " +str(b[3]*a2[1])+" ylo yhi\n")
dfile.write(str(b[4]*a3[2])+ " " +str(b[5]*a3[2])+" zlo zhi\n")
dfile.write("\nMasses\n\n")
dfile.write("1 "+str(mass_atom)+"\n")
dfile.write("2 "+str(mass_electron)+"\n")
dfile.write("\nAtoms\n\n")


x = np.array([0,0,0])
k = 1
for x[0] in box[0]:
    for x[1] in box[1]:
        for x[2] in box[2]:
            for start in basis:
                x_in_bas = x+start
                x_in_bas = x_in_bas[0]*a1+x_in_bas[1]*a2+x_in_bas[2]*a3
                for x_new in simple_struct(x_in_bas):
                    dfile.write(str(k));
                    for item in x_new:
                        dfile.write("\t" + str(item))
                    dfile.write("\n")
                    k = k + 1




#
dfile.close()
