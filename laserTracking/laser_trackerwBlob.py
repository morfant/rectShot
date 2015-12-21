#! /usr/bin/env python
import argparse
import OSC
import cv2
import sys
import numpy as np
import math
import serial
import time
from xbee import XBee

#OSC
#send_addr = '192.168.0.2', 5555
#send_addr = 'localhost', 57120
#send_addr = '10.0.1.61', 20000 #set addr of openframeworks
send_addr = '192.168.1.104', 20000 #set addr of openframeworks
c = OSC.OSCClient()
c.connect(send_addr)
oscmsg = OSC.OSCMessage()
#print "osc ready\n"

class XbeeClass(object):

    def __init__(self):
        self.isShot = False
        self.isButPressed = False
        self.ser = serial.Serial('COM6', 57600)
        self.xbee = XBee(self.ser, callback=self.xbee_get)

    def xbee_get(self, data):
#    print data
        rxData = ord(data['rf_data'][0])
        if rxData == ord('s'):
            self.isShot = True
        elif rxData == ord('b'):
            self.isButPressed = True

    def xbeeToOSC(self):
        #osc send
        self.blobPos = blobDtr.keypoints
        if xb.isShot:
            oscmsg.setAddress("/laserPos")
            if len(self.blobPos) > 0:
                oscmsg.append(self.blobPos[0].pt[0])
                oscmsg.append(self.blobPos[0].pt[1])
                print "Gun shot!!\n"
                print "x: %d / y: %d" %(self.blobPos[0].pt[0], self.blobPos[0].pt[1])
            else:
                oscmsg.append(-1)
                oscmsg.append(-1)
                print "Gun shot with No tracking info!!\n"
            c.send(oscmsg)
            oscmsg.clearData()
            xb.isShot = False
 
        if xb.isButPressed:
            oscmsg.setAddress("/gunButPressed")
            oscmsg.append(1)
            c.send(oscmsg)
            oscmsg.clearData()
            print "Gun ButtonPressed!!\n"
            xb.isButPressed = False
         
 
    def oscSend(self):
        oscmsg.setAddress("/laserPos")
        oscmsg.append(100)
        oscmsg.append(101)
        c.send(oscmsg)
        oscmsg.clearData()



class BlobDetector(object):

    def __init__(self):

        self.params = cv2.SimpleBlobDetector_Params()

        self.params.minThreshold = 100
        self.params.maxThreshold = 200

        self.params.filterByArea = True
        self.params.minArea = 90
        self.params.maxArea = 250

        # Filter by Circularity
        self.params.filterByCircularity = True
        self.params.minCircularity = 0.3
         
        # Filter by Convexity
        self.params.filterByConvexity = True
        self.params.minConvexity = 0.3
         
        # Filter by Inertia
#        self.params.filterByInertia = True
#        self.params.minInertiaRatio = 0.8


        self.ver = (cv2.__version__).split('.')
        if int(self.ver[0]) < 3 :
            self.detector = cv2.SimpleBlobDetector(self.params)
        else :
            self.detector = cv2.SimpleBlobDetector_create(params)
    
    
    def detectBlob(self, frame):
        self.keypoints = self.detector.detect(frame)

        if len(self.keypoints) > 0: #for test
#            oscmsg.setAddress("/laserPos")
#            oscmsg.append(self.keypoints[0].pt[0])
#            oscmsg.append(self.keypoints[0].pt[1])
#            c.send(oscmsg)
#            oscmsg.clearData()
            print "x: %d / y: %d" %(self.keypoints[0].pt[0], self.keypoints[0].pt[1])
            print "size: %f\n" % self.keypoints[0].size
            print "rough calced Area: %f\n" % (pow(self.keypoints[0].size, 2) * math.pi)
#            xb.isShot = False

        im_with_key = cv2.drawKeypoints(frame, self.keypoints, np.array([]), (0, 0, 255),
            cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

        return im_with_key


class LaserTracker(object):

    def __init__(self, cam_width, cam_height, hue_min, hue_max,
                 sat_min, sat_max, val_min, val_max,
                 display_thresholds, thsFile):
        """
        * ``cam_width`` x ``cam_height`` -- This should be the size of the
        image coming from the camera. Default is 640x480.

        HSV color space Threshold values for a RED laser pointer are determined
        by:

        * ``hue_min``, ``hue_max`` -- Min/Max allowed Hue values
        * ``sat_min``, ``sat_max`` -- Min/Max allowed Saturation values
        * ``val_min``, ``val_max`` -- Min/Max allowed pixel values

        If the dot from the laser pointer doesn't fall within these values, it
        will be ignored.

        * ``display_thresholds`` -- if True, additional windows will display
          values for threshold image channels.

        """

        self.cam_width = cam_width
        self.cam_height = cam_height

        # By params        
        self.hue_min = hue_min
        self.hue_max = hue_max
        self.sat_min = sat_min
        self.sat_max = sat_max
        self.val_min = val_min
        self.val_max = val_max
        self.display_thresholds = display_thresholds
        self.thsFile = thsFile

        # By store function
        self.read_thrs(self.thsFile)

        self.capture = None  # camera capture device
        self.channels = {
            'hue': None,
            'saturation': None,
            'value': None,
            'laser': None,
            'Blobed': None,
        }

        self.whichVal = 0
        self.nameList = ["hue_min", "hue_max", "sat_min", "sat_max", "val_min", "val_max"]
        self.valueList = [self.hue_min, self.hue_max, self.sat_min, self.sat_max, self.val_min, self.val_max]
        # Not same with self.hue_min...

        self.displayChannel = 'Blobed'

        self.isHueNew = True
        self.isSatNew = True
        self.isValNew = True
        self.prevHueSel = 0
        self.prevSatSel = 2
        self.prevValSel = 4

    def store_cur_thrs(self, filename):
        if filename:
            f = open(filename, 'w')
        else: #Default filename
            f = open("THS_DATA.txt", 'w')

        f.write(str(self.hue_min) + "\n")
        f.write(str(self.hue_max) + "\n")
        f.write(str(self.sat_min) + "\n")
        f.write(str(self.sat_max) + "\n")
        f.write(str(self.val_min) + "\n")
        f.write(str(self.val_max) + "\n")
        f.close()

#print "Current thresholds saved in " + self.thsFile + ".\n"

    def read_thrs(self, filename):
        if filename:
            r = open(filename, 'r')
        else:
            r = open("THS_DATA.txt", 'r')
        ths = r.readlines()

        self.hue_min = int(ths[0].split('\n')[0])
        self.hue_max = int(ths[1].split('\n')[0])
        self.sat_min = int(ths[2].split('\n')[0])
        self.sat_max = int(ths[3].split('\n')[0])
        self.val_min = int(ths[4].split('\n')[0])
        self.val_max = int(ths[5].split('\n')[0])

#print "Thresholds reading from " + self.thsFile + "...\n"

    def create_and_position_window(self, name, xpos, ypos):
        """Creates a named widow placing it on the screen at (xpos, ypos)."""
        # Create a window
        cv2.namedWindow(name, cv2.WINDOW_AUTOSIZE)
        # Resize it to the size of the camera image
        cv2.resizeWindow(name, self.cam_width, self.cam_height)
        # Move to (xpos,ypos) on the screen
        cv2.moveWindow(name, xpos, ypos)

    def setup_camera_capture(self, device_num=0):
        """Perform camera setup for the device number (default device = 0).
        Returns a reference to the camera Capture object.

        """
        try:
            device = int(device_num)
            sys.stdout.write("Using Camera Device: {0}\n".format(device))
        except (IndexError, ValueError):
            # assume we want the 1st device
            device = 0
            sys.stderr.write("Invalid Device. Using default device 0\n")

        # Try to start capturing frames
        self.capture = cv2.VideoCapture(device)
        if not self.capture.isOpened():
            sys.stderr.write("Faled to Open Capture device. Quitting.\n")
            sys.exit(1)

        # set the wanted image size from the camera
        self.capture.set(
            #cv2.CV_CAP_PROP_FRAME_WIDTH,
            cv2.cv.CV_CAP_PROP_FRAME_WIDTH,
            self.cam_width
        )
        self.capture.set(
            #cv.CV_CAP_PROP_FRAME_HEIGHT,
            cv2.cv.CV_CAP_PROP_FRAME_HEIGHT,
            self.cam_height
        )
        return self.capture

    def printValues(self):
        if self.whichVal == 0:
            print "HUE_MIN: "
        elif self.whichVal == 1:
            print "HUE_MAX: "
        elif self.whichVal == 2:
            print "SAT_MIN: "
        elif self.whichVal == 3:
            print "SAT_MAX: "
        elif self.whichVal == 4:
            print "VAL_MIN: "
        elif self.whichVal == 5:
            print "VAL_MAX: "
        print self.valueList[self.whichVal]


    def handle_keyInput(self, delay=10):
        """Quit the program if the user presses "Esc" or "q"."""
        key = cv2.waitKey(delay)
        c = chr(key & 255)
        if c in [chr(27)]:
#print "Program is stopping..\n"
            self.store_cur_thrs(self.thsFile)
            #xbee closing
            xb.xbee.halt()
            xb.ser.close()

            sys.exit(0)

        elif c in ['h', 'H']: #Toggle between sat_min and sat_max
            if self.isHueNew:
                self.displayChannel = 'hue'
                self.whichVal = self.prevHueSel
                self.isHueNew = False
                self.isSatNew = True
                self.isValNew = True
            else:
                if self.whichVal == 0:
                    self.whichVal = 1
                elif self.whichVal == 1:
                    self.whichVal = 0
                else:
                    self.whichVal = 0

            self.prevHueSel = self.whichVal # Store previous sel value

        elif c in ['s', 'S']:
            if self.isSatNew:
                self.displayChannel = 'saturation'
                self.whichVal = self.prevSatSel
                self.isSatNew = False
                self.isHueNew = True
                self.isValNew = True
            else:
                if self.whichVal == 2:
                    self.whichVal = 3
                elif self.whichVal == 3:
                    self.whichVal = 2
                else:
                    self.whichVal = 2

            self.prevSatSel = self.whichVal

        elif c in ['v', 'V']:
            if self.isValNew:
                self.displayChannel = 'value'
                self.whichVal = self.prevValSel
                self.isValNew = False
                self.isHueNew = True
                self.isSatNew = True
            else:
                if self.whichVal == 4:
                    self.whichVal = 5
                elif self.whichVal == 5:
                    self.whichVal = 4
                else:
                    self.whichVal = 4

            self.prevValSel = self.whichVal

        elif c in ['b', 'B']:
            self.isHueNew = True
            self.isSatNew = True
            self.isValNew = True
            self.displayChannel = 'Blobed'
#print "Blobed display selected.\n"

        elif c in ['l', 'L']:
            self.isHueNew = True
            self.isSatNew = True
            self.isValNew = True
            self.displayChannel = 'laser'

        elif c in ['=', '+']: # Increase selected val
            val = self.valueList[self.whichVal] 
            if self.whichVal % 2: # Odd -> max values
                if val < 255 and val >= self.valueList[self.whichVal - 1]:
                    self.valueList[self.whichVal] = self.valueList[self.whichVal] + 1
                    self.printValues()

            else: # Even -> min values
                if val >= 0 and val < self.valueList[self.whichVal + 1] - 1:
                    self.valueList[self.whichVal] = self.valueList[self.whichVal] + 1
                    self.printValues()

                # Update values
                self.hue_min = self.valueList[0]
                self.hue_max = self.valueList[1]
                self.sat_min = self.valueList[2]
                self.sat_max = self.valueList[3]
                self.val_min = self.valueList[4]
                self.val_max = self.valueList[5]

        elif c in ['-', '_']: # Decrease selected val
            val = self.valueList[self.whichVal] 
            if self.whichVal % 2: # Odd -> max values
                if val <= 255 and val > self.valueList[self.whichVal - 1] + 1:
                    self.valueList[self.whichVal] = self.valueList[self.whichVal] - 1
                    self.printValues()

            else: # Even -> min values
                if val > 0 and val < self.valueList[self.whichVal + 1]:
                    self.valueList[self.whichVal] = self.valueList[self.whichVal] - 1
                    self.printValues()

                # Update values
                self.hue_min = self.valueList[0]
                self.hue_max = self.valueList[1]
                self.sat_min = self.valueList[2]
                self.sat_max = self.valueList[3]
                self.val_min = self.valueList[4]
                self.val_max = self.valueList[5]

        elif c in [' ']: 
            self.store_cur_thrs()
            
 

    def threshold_image(self, channel):
        if channel == "hue":
            minimum = self.hue_min
            maximum = self.hue_max
        elif channel == "saturation":
            minimum = self.sat_min
            maximum = self.sat_max
        elif channel == "value":
            minimum = self.val_min
            maximum = self.val_max

        (t, tmp) = cv2.threshold(
            self.channels[channel], # src
            maximum, # threshold value
            0, # we dont care because of the selected type
            cv2.THRESH_TOZERO_INV #t type
        )

        (t, self.channels[channel]) = cv2.threshold(
            tmp, # src
            minimum, # threshold value
            255, # maxvalue
            cv2.THRESH_BINARY # type
        )

        if channel == 'hue':
            # only works for filtering red color because the range for the hue is split
            self.channels['hue'] = cv2.bitwise_not(self.channels['hue'])


    def detect(self, frame):
        #hsv_img = cv2.cvtColor(frame, cv2.CV_BGR2HSV)
        hsv_img = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # split the video frame into color channels
        h, s, v = cv2.split(hsv_img)
        self.channels['hue'] = h
        self.channels['saturation'] = s
        self.channels['value'] = v

        # Threshold ranges of HSV components; storing the results in place
        self.threshold_image("hue")
        self.threshold_image("saturation")
        self.threshold_image("value")

        # Perform an AND on HSV components to identify the laser!

#        self.channels['laser'] = cv2.bitwise_and(
#            self.channels['hue'],
#self.channels['value']
#        )
#        self.channels['laser'] = cv2.bitwise_and(
#            self.channels['saturation'],
#self.channels['laser']
#)

#Use only 'val' 
        self.channels['laser'] = self.channels['value']


        # Merge the HSV components back together.
        hsv_image = cv2.merge([
            self.channels['hue'],
            self.channels['saturation'],
            self.channels['value'],
        ])

        return hsv_image

    def display(self, img, frame):
        """Display the combined image and (optionally) all other image channels
        NOTE: default color space in OpenCV is BGR.
        """
        for i in range(0, 6):
            if i == self.whichVal:
                self.textSize = 1.3
            else:
                self.textSize = 0.8
            cv2.putText(frame, self.nameList[i] + " : " +
                str(self.valueList[i]), (20, 20 + (i*20)),
                cv2.FONT_HERSHEY_PLAIN, self.textSize, (255, 255, 255))

        cv2.imshow('RGB_VideoFrame', frame)

        displaySource = self.channels[self.displayChannel]
        for i in range(0, 6):
            if i == self.whichVal:
                self.textSize = 1.3
            else:
                self.textSize = 0.8
            cv2.putText(displaySource, self.nameList[i] + " : " +
                str(self.valueList[i]), (20, 20 + (i*20)),
                cv2.FONT_HERSHEY_PLAIN, self.textSize, (255, 255, 255))

        cv2.imshow('Blobs', displaySource)

        if self.display_thresholds:
            cv2.imshow('Thresholded_HSV_Image', img)
            cv2.imshow('Hue', self.channels['hue'])
            cv2.imshow('Saturation', self.channels['saturation'])
            cv2.imshow('Value', self.channels['value'])


    def setup_windows(self):
        sys.stdout.write("Using OpenCV version: {0}\n".format(cv2.__version__))

        # create output windows
        self.create_and_position_window('Blobs', 30 + self.cam_width, 0)
#        self.create_and_position_window('LaserPointer', 0, 0)
        self.create_and_position_window('RGB_VideoFrame', 0, 0)
        if self.display_thresholds:
            self.create_and_position_window('Thresholded_HSV_Image', 10, 10)
            self.create_and_position_window('Hue', 20, 20)
            self.create_and_position_window('Saturation', 30, 30)
            self.create_and_position_window('Value', 40, 40)


    def run(self):
        # Set up window positions
        self.setup_windows()
        # Set up the camera capture
        self.setup_camera_capture()

        while True:
            try:
                # 1. capture the current image
                success, frame = self.capture.read()
                if not success: # no image captured... end the processing
                    sys.stderr.write("Could not read camera frame. Quitting\n")
                    sys.exit(1)

                hsv_image = self.detect(frame)
                self.channels['Blobed'] = blobDtr.detectBlob(self.channels['laser'])
                xb.xbeeToOSC()
                self.display(hsv_image, frame)
                self.handle_keyInput()
                
            except KeyboardInterrupt:
                break


if __name__ == '__main__':
    xb = XbeeClass()
    blobDtr = BlobDetector()

    parser = argparse.ArgumentParser(description='Run the Laser Tracker')
    parser.add_argument('-W', '--width',
        default=640,
        type=int,
        help='Camera Width'
    )
    parser.add_argument('-H', '--height',
        default=480,
        type=int,
        help='Camera Height'
    )
    parser.add_argument('-u', '--huemin',
        default=20,
        type=int,
        help='Hue Minimum Threshold'
    )
    parser.add_argument('-U', '--huemax',
        default=160,
        type=int,
        help='Hue Maximum Threshold'
    )
    parser.add_argument('-s', '--satmin',
        default=100,
        type=int,
        help='Saturation Minimum Threshold'
    )
    parser.add_argument('-S', '--satmax',
        default=255,
        type=int,
        help='Saturation Maximum Threshold'
    )
    parser.add_argument('-v', '--valmin',
        default=200,
        type=int,
        help='Value Minimum Threshold'
    )
    parser.add_argument('-V', '--valmax',
        default=255,
        type=int,
        help='Value Maximum Threshold'
    )
    parser.add_argument('-d', '--display',
        action='store_true',
        help='Display Threshold Windows'
    )
    parser.add_argument('-f', '--thsfile',
        default="THS_DATA.txt",
        help='Set Thresholds set up file'
    )
 
    params = parser.parse_args()

    tracker = LaserTracker(
        cam_width=params.width,
        cam_height=params.height,
        hue_min=params.huemin,
        hue_max=params.huemax,
        sat_min=params.satmin,
        sat_max=params.satmax,
        val_min=params.valmin,
        val_max=params.valmax,
        display_thresholds=params.display,
        thsFile=params.thsfile
    )
    tracker.run()

