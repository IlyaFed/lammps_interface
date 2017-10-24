import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import pandas as pd
import plot_all
import lib.density_analysis.density_analysis_api as dens_api
import lib.lattice as lattice
import Tkinter, Tkconstants, tkFileDialog
from Tkinter import *

def read_log():
    #output.delete("0.0","end")
    filename = tkFileDialog.askopenfilename(title = "Import file")
    energy = plot_all.read_log(filename)

    fig, axes = plt.subplots(nrows=2, ncols=1)
    energy.plot(x='Step', y='Temp', kind="line", ax=axes[0])
    energy.plot(x='Step', y='Press', kind="line", ax=axes[1])
    plt.show()
    output.insert("end", "read log success\n")
    output.insert("end","smth else\n")

def density_plot():
    filename = tkFileDialog.askopenfilename(title = "Import file", filetypes = (("xyz","*.xyz"), ("all files", "*.*")))
    filename = filename.replace(" ", "\ ")
    outputfilename = tkFileDialog.asksaveasfilename(title = "Export file")
    outputfilename = outputfilename.replace(" ", "\ ")
    ret = dens_api.xyz_collect(filename, outputfilename, wall=12.1838359794)
    if not ret:
        output.insert("end", "Success collect coordinate\n")
    else:
        output.insert("end", "Unsuccess collect coordinate\n")

root = Tk()

frame = Frame(root)
frame.grid()

title = Label(frame, text="Some instruments")
title.grid(row=1, column=1)

import_btn1 = Button(frame, text="Plot lammps log", command=read_log)
import_btn1.grid(row=2, column=0, pady=4)


import_btn2 = Button(frame, text="Density analysis", command=density_plot)
import_btn2.grid(row=2, column=2, pady=4)



output = Text(frame, width=45, height=3)
output.grid(row=4, columnspan=3)

root.mainloop()



