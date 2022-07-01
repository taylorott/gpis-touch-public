from gp_wrapper import gp_wrapper
import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
    my_gp = gp_wrapper()

    varNoise = [1e-5, 1e-5, 1e-5] # measurement noise
    priorNoise = [1e-3, 1e-3, 1e-3] # circular prior noise
    #GP parameters
    testLim = 1.5
    testRes =  0.01
    isLocal = True
    priorRad = 0.03

    fname = "/home/taylorott/Documents/gpis-touch-public/data/contacts/contacts-rect1-20200810-1811.txt"
    read_file = open(fname,'r')
    Lines = read_file.readlines()

    fig, axs = plt.subplots(1,1)
    my_gp.init_gp(varNoise, priorNoise, testLim, testRes, isLocal, priorRad)

    for line in Lines:
        num_list = line.split(',')
        pos_vec = [float(num_list[0]),float(num_list[1])]
        normal_vec = [float(num_list[2]),float(num_list[3])]

        my_gp.evalAtPoint(pos_vec[0],pos_vec[1])
        # my_gp.update_gp(pos_vec,normal_vec)
        # contour_x,contour_y = my_gp.eval_contour()

        # axs.cla()
        # axs.plot(contour_x,contour_y)
        # axs.axis("equal")

        # axs.set_ylim([-0.09, 0.09])
        # axs.set_xlim([-0.09, 0.09])

        # plt.pause(0.01)




    print ('hello!')