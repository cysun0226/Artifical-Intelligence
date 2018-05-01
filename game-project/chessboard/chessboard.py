import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.misc import imread
import matplotlib.cbook as cbook
import sys
log_file = str(sys.argv[1])
img_name = log_file.split('.')[0]

# the location of the program
# print(os.getcwd())

# chessboard mapping
origin_x = 2.55
origin_y = 6.65
scale_x = 0.428
shift_x = -0.2145
scale_y = -0.33475

def chessboard_mapping(pos):
    x = 0
    y = 0
    if pos >= 0 and pos < 9 :
        x = origin_x + scale_x*pos
        y = origin_y
    elif pos >= 9 and pos < 18 :
        x = origin_x + shift_x*1 + scale_x*(pos-9)
        y = origin_y + scale_y
    elif pos >= 19 and pos < 29 :
        x = origin_x + shift_x*2 + scale_x*(pos-19)
        y = origin_y + scale_y*2
    elif pos >= 30 and pos < 42 :
        x = origin_x + shift_x*3 + scale_x*(pos-30)
        y = origin_y + scale_y*3
    elif pos >= 42 and pos < 55 :
        x = origin_x + shift_x*4 + scale_x*(pos-42)
        y = origin_y + scale_y*4
    elif pos >= 55 and pos < 69 :
        x = origin_x + shift_x*5 + scale_x*(pos-55)
        y = origin_y + scale_y*5
    elif pos >= 69 and pos < 84 :
        x = origin_x + shift_x*6 + scale_x*(pos-69)
        y = origin_y + scale_y*6
    elif pos >= 84 and pos <= 99 :
        x = origin_x + shift_x*7 + scale_x*(pos-84)
        y = origin_y + scale_y*7
    elif pos >= 100 and pos <= 116 :
        x = origin_x + shift_x*8 + scale_x*(pos-100)
        y = origin_y + scale_y*8
    elif pos >= 117 and pos <= 132 :
        x = origin_x + shift_x*7 + scale_x*(pos-117)
        y = origin_y + scale_y*9
    elif pos >= 133 and pos <= 147 :
        x = origin_x + shift_x*6 + scale_x*(pos-133)
        y = origin_y + scale_y*10
    elif pos >= 148 and pos <= 161 :
        x = origin_x + shift_x*5 + scale_x*(pos-148)
        y = origin_y + scale_y*11
    elif pos >= 162 and pos <= 174 :
        x = origin_x + shift_x*4 + scale_x*(pos-162)
        y = origin_y + scale_y*12
    elif pos >= 175 and pos <= 186 :
        x = origin_x + shift_x*3 + scale_x*(pos-175)
        y = origin_y + scale_y*13
    elif pos >= 187 and pos <= 197 :
        x = origin_x + shift_x*2 + scale_x*(pos-187)
        y = origin_y + scale_y*14
    elif pos >= 198 and pos <= 207 :
        x = origin_x + shift_x*1 + scale_x*(pos-198)
        y = origin_y + scale_y*15
    elif pos >= 208 and pos <= 216 :
        x = origin_x + scale_x*(pos-208)
        y = origin_y + scale_y*16

    return [x, y]


round = 0
with open(log_file,'r') as log:
    round = int(log.readline())
    chessboard = [int(s) for s in log.readline().split(' ')]

# print(chessboard)
pos = -1
# The points to show on the chessboard
color = []
x = []
y = []

for chess in chessboard:
    pos += 1
    if chess == 0:
        continue
    print("draw " + str(pos))
    x.append(chessboard_mapping(pos)[0])
    y.append(chessboard_mapping(pos)[1])
    if chess == 1:
        color.append('white')
    else:
        color.append('black')



datafile = cbook.get_sample_data(os.getcwd()+'\chessboard.png')
img = imread(datafile)
# plt.text(2, 0.65, "Hello")
plt.scatter(x,y,c=color, zorder=1)
plt.imshow(img, zorder=0, extent=[0.5, 8.0, 1.0, 7.0])
plt.xticks([])
plt.yticks([])
plt.axis('off')
# plt.savefig(img_name + '.png')
plt.show()
