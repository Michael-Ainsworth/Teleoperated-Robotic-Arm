# -*- coding: utf-8 -*-
"""
Created on Wed Jan 15 11:08:24 2020

@author: Arik
"""
##NOTE: Due to the variable nature of the Serial output from the Arduino, it was necessary at times
#to rerun, this error was often "Index not in range"

#Imports
import serial
import time
import numpy as np
import PySimpleGUI as sg

#This is specific to each computer and is the port that the Arduino is plugged into
ser = serial.Serial(port='/dev/tty.usbmodem14101', baudrate = 9600)

#This establishes an initial layout that is a square that the sensor outputs will be overlaid
layout = [
        [sg.Graph(canvas_size=(400, 400), graph_bottom_left=(0,0), graph_top_right=(400, 400), background_color='white', key='graph')],
        ]

window = sg.Window("rect on image", layout)
window.Finalize()

graph = window.Element("graph")

time.sleep(.5)

#This while loop sets up the constant reading of the Serial port
while(1):    
    
    #Begin to read in the values from the Serial port
    val = ser.readline()
    encoding = 'utf-8'
    strval = str(val, encoding)
    #These values are then stored in this array
    values = strval.split(",", 10)
    values = [float(values[i]) for i in range(len(values))]
    print(values)
    
    #It is necesary to redraw the graph or it will begin to overlap and fail
    graph.Erase()
    
    #This code establishes the color of each of the 9 taxels and creates a color code
    #The folowing uses the scale from 0 - 1000 which we determined was the range
    color1 = '#' + '%02x%02x%02x' % (int(255*(values[0])/1000), int(255*(values[0])/1000), int(255*(values[0])/1000))
    color2 = '#' + '%02x%02x%02x' % (int(255*(values[1])/1000), int(255*(values[1])/1000), int(255*(values[1])/1000))
    color3 = '#' + '%02x%02x%02x' % (int(255*(values[2])/1000), int(255*(values[2])/1000), int(255*(values[2])/1000))
    color4 = '#' + '%02x%02x%02x' % (int(255*(values[3])/1000), int(255*(values[3])/1000), int(255*(values[3])/1000))
    color5 = '#' + '%02x%02x%02x' % (int(255*(values[4])/1000), int(255*(values[4])/1000), int(255*(values[4])/1000))
    color6 = '#' + '%02x%02x%02x' % (int(255*(values[5])/1000), int(255*(values[5])/1000), int(255*(values[5])/1000))
    color7 = '#' + '%02x%02x%02x' % (int(255*(values[6])/1000), int(255*(values[6])/1000), int(255*(values[6])/1000))
    color8 = '#' + '%02x%02x%02x' % (int(255*(values[7])/1000), int(255*(values[7])/1000), int(255*(values[7])/1000))
    color9 = '#' + '%02x%02x%02x' % (int(255*(values[8])/1000), int(255*(values[8])/1000), int(255*(values[8])/1000))
    
   #Create a color code for the two circles that represent the thermistors 
   #The values mentioned are scaled by subtracting the starting value and dividing by the range of change
   #The second color has a negative range because the thermistor decreases with increased temperature
    color10 = '#' + '%02x%02x%02x' % (int(255*((values[9])-90)/25), 0, int(255-255*((values[9])-90)/25))
    color11 = '#' + '%02x%02x%02x' % (int((255*((values[10]-250)/(-70)))), 0, int(255-(255*((values[10]-260)/(-70)))))
    
    #This creates the graphic individual squares for each taxel with the above colors
    #From the layout established in the begining, the location of the square on the graph is established 
    graph.DrawRectangle((50, 150), (150,50), fill_color= color1, line_color="black")
    graph.DrawRectangle((50, 250), (150,150), fill_color= color2, line_color="black")
    graph.DrawRectangle((50, 350), (150,250), fill_color= color3, line_color="black")
    graph.DrawRectangle((150, 150), (250,50), fill_color= color4, line_color="black")
    graph.DrawRectangle((150, 250), (250,150), fill_color= color5, line_color="black")
    graph.DrawRectangle((150, 350), (250,250), fill_color= color6, line_color="black")
    graph.DrawRectangle((250, 150), (350,50), fill_color= color7, line_color="black")
    graph.DrawRectangle((250, 250), (350,150), fill_color= color8, line_color="black")
    graph.DrawRectangle((250, 350), (350,250), fill_color= color9, line_color="black")
    
    #This creates the same as above but circles with the colors associated
    graph.DrawCircle((150, 150), 25, fill_color = color10, line_color = "red", line_width = 0)
    graph.DrawCircle((250, 250), 25, fill_color = color11, line_color = "red", line_width = 0)
    
    window.Refresh()

ser.close()
