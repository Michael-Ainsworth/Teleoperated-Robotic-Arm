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
#This size is based on the size of the hand image that is inputted later in the code
layout = [
        [sg.Graph(canvas_size=(310, 400), graph_bottom_left=(0,0), graph_top_right=(310, 400), background_color='white', key='graph')],
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
    values = strval.split(",", 18)
    
    #This handles if there is a string inputted by the Serial port
    #an error that happened in rare cases
    try: 
        values = [float(values[i]) for i in range(len(values))]
    except ValueError:
        values = [float(0) for i in range(len(values))]
        
    #This prints the associated values from the code to the fingers
    print("Thumb:", values[4], values[5], values[10], values[11], values[16], values[17])
    print("Index:",values[2], values[3], values[8], values[9], values[14], values[15])
    print("Middle:",values[0], values[1], values[6], values[7], values[12], values[13])
    
    #It is necesary to redraw the graph or it will begin to overlap and fail
    graph.Erase()
    
    #This image is the directory on the computer that a user will have to change
    graph.DrawImage(filename = '/Users/Dante/Desktop/Honors Instrumentation/newfingergui.png', location = (0,400))
         
    #Thumb
    #This code establishes the color of each of the 9 taxels per finger and creates a color code
    #The folowing uses the scale from 0 - 1000 which we determined was the range for some
    #Some values mentioned are scaled by subtracting the starting value and dividing by the range of change
    #Colors 11,12,17,18 has a negative range because the voltage decreases with increased force
    color5 = '#' + '%02x%02x%02x' % (int(255*(((values[4]))/1000)), int(255*(((values[4]))/1000)), int(255*(((values[4]))/1000)))
    color6 = '#' + '%02x%02x%02x' % (int(255*(((values[5]))/1000)), int(255*(((values[5]))/1000)), int(255*(((values[5]))/1000)))
    color11 = '#' + '%02x%02x%02x' % (int(255*((-(values[10]-500)/500))), int(255*((-(values[10]-500))/500)), int(255*((-(values[10]-500))/500)))
    color12 = '#' + '%02x%02x%02x' % (int(255*((-(values[11]-500)/500))), int(255*((-(values[11]-500))/500)), int(255*((-(values[11]-500))/500)))
    color17 = '#' + '%02x%02x%02x' % (int(255*(((-(values[16]-800)/500)))), int(255*((- (values[16]-800))/500)), int(255*((- (values[16]-800))/500)))
    color18 = '#' + '%02x%02x%02x' % (int(255*((-((values[17]-800)/500)))), int(255*((-(values[17]-800))/500)), int(255*((-(values[17]-800))/500)))
    
    #Index Finger
    #This code establishes the color of each of the 9 taxels per finger and creates a color code
    #The folowing uses the scale from 0 - 150 which we determined was the range for some
    color3 = '#' + '%02x%02x%02x' % (int(255*(values[2])/150), int(255*(values[2])/150), int(255*(values[2])/150))
    color4 = '#' + '%02x%02x%02x' % (int(255*(values[3])/150), int(255*(values[3])/150), int(255*(values[3])/150))
    color9 = '#' + '%02x%02x%02x' % (int(255*(values[8])/150), int(255*(values[8])/150), int(255*(values[8])/150))
    color10 = '#' + '%02x%02x%02x' % (int(255*(values[9])/150), int(255*(values[9])/150), int(255*(values[9])/150))
    color15 = '#' + '%02x%02x%02x' % (int(255*(values[14])/150), int(255*(values[14])/150), int(255*(values[14])/150))
    color16 = '#' + '%02x%02x%02x' % (int(255*(values[15])/150), int(255*(values[15])/150), int(255*(values[15])/150))
    
    #Middle Finger
    #This code establishes the color of each of the 9 taxels per finger and creates a color code
    #The folowing uses the scale from 0 - 500 and 0 - 250 which we determined was the range for some
    #Some values mentioned are scaled by subtracting the starting value and dividing by the range of change
    #Color 7 has a negative range because the voltage decreases with increased force
    color1 = '#' + '%02x%02x%02x' % (int(255*(values[0])/500), int(255*(values[0])/500), int(255*(values[0])/500))
    color2 = '#' + '%02x%02x%02x' % (int(255*(values[1])/500), int(255*(values[1])/500), int(255*(values[1])/500))
    color7 = '#' + '%02x%02x%02x' % (int(255*((-(values[6]-550)/400))), int(255*((-(values[6]-550))/400)), int(255*((-(values[6]-550))/400)))
    color8 = '#' + '%02x%02x%02x' % (int(255*(values[7])/250), int(255*(values[7])/250), int(255*(values[7])/250))
    color13 = '#' + '%02x%02x%02x' % (int(255*(values[12])/500), int(255*(values[12])/500), int(255*(values[12])/500))
    color14 = '#' + '%02x%02x%02x' % (int(255*(values[13])/250), int(255*(values[13])/250), int(255*(values[13])/250))
    
    #From the layout established in the begining, the location of the square on the graph is established
    #to overlay with the hand picture
    
    #Thumb 
    #This creates the graphic individual squares for each taxel with the above colors
    graph.DrawRectangle((0, 240),  (10,225), fill_color= color5, line_color="black")
    graph.DrawRectangle((0, 225),  (10,210), fill_color= color6, line_color="black")
    graph.DrawRectangle((10, 240),  (20,225), fill_color= color11, line_color="black")
    graph.DrawRectangle((10, 225),  (20,210), fill_color= color12, line_color="black")
    graph.DrawRectangle((20, 240),  (30,225), fill_color= color17, line_color="black")
    graph.DrawRectangle((20, 225),  (30,210), fill_color= color18, line_color="black")
    
    #Index Finger
    #This creates the graphic individual squares for each taxel with the above colors
    graph.DrawRectangle((96, 390),  (106,375), fill_color= color3, line_color="black")
    graph.DrawRectangle((96, 375), (106,360), fill_color= color4, line_color="black")
    graph.DrawRectangle((106, 390), (116,375), fill_color= color9, line_color="black")
    graph.DrawRectangle((106, 375),  (116,360), fill_color= color10, line_color="black")
    graph.DrawRectangle((116, 390), (126,375), fill_color= color15, line_color="black")
    graph.DrawRectangle((116, 375), (126,360), fill_color= color16, line_color="black")
    
    #Middle Finger
    #This creates the graphic individual squares for each taxel with the above colors
    graph.DrawRectangle((158, 400), (168,385), fill_color= color1, line_color="black")
    graph.DrawRectangle((158, 385), (168,370), fill_color= color2, line_color="black")
    graph.DrawRectangle((168, 400),  (178,385), fill_color= color7, line_color="black")
    graph.DrawRectangle((168, 385),  (178,370), fill_color= color8, line_color="black")    
    graph.DrawRectangle((178, 400), (188,385), fill_color= color13, line_color="black")
    graph.DrawRectangle((178, 385), (188,370), fill_color= color14, line_color="black")
    
    window.Refresh()
 
ser.close()
