#!/usr/bin/env python

import sys
import pigpio

pi = pigpio.pi()
if not pi.connected:
    exit()

level = pi.read(GPIO)

print("GPIO {} is {}".format(GPIO, level))

pi.stop()

