import spatialaudiopy


import sys
import math
from time import sleep

def start():
    # setup scenegraph
    # serversocket, num_user_slots
    sas = spatialaudiopy.SpatialAudioSender("127.0.0.1:9000", 4)
    sas.start()
    angle = 0.0
    while True:
        sleep(0.01)
        angle = angle + 0.01
        # user_id, group_id, pos front up
        sas.setUserData(0, 0, math.sin(angle), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0)
        sas.setUserData(1, 0, 0.0, math.sin(angle), 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0)
        sas.setUserData(2, 1, math.cos(angle), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0)
        sas.setUserData(3, 1, 0.0, math.cos(angle), 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0)
	sas.setUserMatrixA(0, 1.0 , 0.0 , 0.0, 0.0)
	sas.setUserMatrixB(0, 0.0 , 1.0 , 0.0, 0.0)
	sas.setUserMatrixC(0, 0.0 , 0.0 , 1.0, 0.0)
	sas.setUserMatrixD(0, math.sin(angle) , 0.0 , 0.0, 0.0)


if __name__ == '__main__':
    start()
