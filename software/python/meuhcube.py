"""
Copyright Vincent LAMBERT. (2016)
email: vincent@influence-pc.fr
This software is a computer program whose purpose is to give access to an API over serial connexion to a device called Meuhcube. The Meuhcube is a Do-It-Yourself project using the Arduino Mega to drive a 7x7x7 LED cube.
This software is governed by the CeCILL license under French law and abiding by the rules of distribution of free software.  You can  use, modify and/ or redistribute the software under the terms of the CeCILL license as circulated by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
As a counterpart to the access to the source code and rights to copy, modify and redistribute granted by the license, users are provided only with a limited warranty  and the software's author,  the holder of the economic rights, and the successive licensors  have only  limited liability.
In this respect, the user's attention is drawn to the risks associated with loading,  using,  modifying and/or developing or reproducing the software by the user in light of its specific status of free software, that may mean  that it is complicated to manipulate,  and  that  also therefore means  that it is reserved for developers  and  experienced professionals having in-depth computer knowledge. Users are therefore encouraged to load and test the software's suitability as regards their requirements in conditions enabling the security of their systems and/or data to be ensured and,  more generally, to use and operate it in the same conditions as regards security.
The fact that you are presently reading this means that you have had knowledge of the CeCILL license and that you accept its terms.
"""

#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import glob
import serial
import time


def serial_ports():
  """
  Lists serial port names
  """
  if sys.platform.startswith('win'):
    ports = ['COM%s' % (i + 1) for i in range(256)]
  elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
    # this excludes your current terminal "/dev/tty"
    ports = glob.glob('/dev/tty[A-Za-z]*')
  elif sys.platform.startswith('darwin'):
    ports = glob.glob('/dev/tty.wc*')
  else:
    raise EnvironmentError('Unsupported platform')

  result = []
  for port in ports:
    try:
      s = serial.Serial(port)
      s.close()
      result.append(port)
    except (OSError, serial.SerialException):
     pass
  return result


# Commands to turn on/off LED

def set_all(layer_states = 7 * [True]):
  """
  Turn all LED on without parameter. Turn layers on if a list of seven bits is given, from right to left. Parameter:
  - layer_states, a list of seven booleans
  Example: (False, False, False, False, True, False, True) will turn off all LED of layers 7, 6, 5, 4, 2 and turn on LED of layers 3 and 1
  """
  int_states = [int(boolean) for boolean in layer_states]
  str_states = [str(boolean) for boolean in int_states]
  str_states.reverse() # bit parameters are from left to right
  bits = "".join(str_states)
  byte = int(bits, 2)
  usb_link.write(bytearray([0xE7, 0x00, 0x01, byte, 0x7E]))


def set_voxel(coordinates = [0, 0, 0], led_state = True):
  """
  Turn a voxel on. Parameters:
  - coordinates, an integer list for x, y, z
  - led_state, a boolean
  """
  usb_link.write(bytearray([0xE7, 0x01, 0x04, coordinates[0], coordinates[1], coordinates[2], int(led_state), 0x7E]))


def set_line(axis = "x", shift_side_1 = 0, shift_side_2 = 0, led_states = 7 * [True]):
  """
  Turn a line on. Parameters:
  - axis, chose an edge of the cube as reference. A string of "x", "y" or "z"
  - shift_side_1, shift the line from the chosen axis to one of the two adjacent sides. An integer between 0 and 6
  - shift_side_2, shift the line from the chosen axis to one of the two adjacent sides. An integer between 0 and 6
  - led_states, seven booleans
  """
  if axis == "x":
    axis_byte = 0
  elif axis == "y":
    axis_byte = 1
  elif axis == "z":
    axis_byte = 2
  int_states = [int(boolean) for boolean in led_states]
  str_states = [str(boolean) for boolean in int_states]
  str_states.reverse() # bit parameters are from left to right
  bits = "".join(str_states)
  state_byte = int(bits, 2)
  usb_link.write(bytearray([0xE7, 0x02, 0x04, axis_byte, shift_side_1, shift_side_2, state_byte, 0x7E]))


def set_plan(axis = "xy", shift = 0, led_states = 7 * [True]):
  """
  Turn a plan on, by a line or pixel by pixel, depending of the led_states parameter.
  """
  if type(led_states[0]) is bool:
    set_plan_by_line(axis, shift, led_states)
  else:
    set_plan_by_pixel(axis, shift, led_states)


def set_plan_by_line(axis = "xy", shift = 0, led_states = 7 * [True]):
  """
  Turn a plan on. The columns of the plan are defined by a single line replicated on the seven columns. Parameters:
  - axis, chose two axis letters to select a side of the cube. A string of "xy", "xz" or "yz"
  - shift, move the plan from a side of the cube into it. An integer between 0 and 6
  - led_states, seven booleans figuring a line used on the seven columns of the plan
  """
  if axis == "xz":
    axis_byte = 0
  elif axis == "xy":
    axis_byte = 1
  elif axis == "yz":
    axis_byte = 2
  int_states = [int(boolean) for boolean in led_states]
  str_states = [str(boolean) for boolean in int_states]
  str_states.reverse() # bit parameters are from left to right
  bits = "".join(str_states)
  state_byte = int(bits, 2)
  usb_link.write(bytearray([0xE7, 0x03, 0x03, axis_byte, shift, state_byte, 0x7E]))


def set_plan_by_pixel(axis = "xy", shift = 0, led_states = 7 * [7 * [True]]):
  """
  Turn a plan on. The columns of the plan are defined pixel by pixel. Parameters:
  - axis, chose two axis letters to select a side of the cube. A string of "xy", "xz" or "yz"
  - shift, move the plan from a side of the cube into it. An integer between 0 and 6
  - led_states, seven arrays of seven booleans to define the plan pixel by pixel
  """
  if axis == "xz":
    axis_byte = 0
  elif axis == "xy":
    axis_byte = 1
  elif axis == "yz":
    axis_byte = 2
  state_bytes = []
  for i in range(7):
    int_states = [int(boolean) for boolean in led_states[i]]
    str_states = [str(boolean) for boolean in int_states]
    str_states.reverse() # bit parameters are from left to right
    bits = "".join(str_states)
    state_bytes.append(int(bits, 2))
  usb_link.write(bytearray([0xE7, 0x03, 0x09, axis_byte, shift, state_bytes[0], state_bytes[1], state_bytes[2], state_bytes[3], state_bytes[4], state_bytes[5], state_bytes[6], 0x7E]))


def set_cube(led_states = 7 * [7 * [7 * [True]]]):
  """
  Turn the whole cube on, led by led. Parameters:
  - led_states, seven arrays of seven arrays of seven booleans, to define each voxel
  """
  state_bytes = []
  for i in range(7):
    for j in range(7):
      int_states = [int(boolean) for boolean in led_states[i][j]]
      str_states = [str(boolean) for boolean in int_states]
      str_states.reverse() # bit parameters are from left to right
      bits = "".join(str_states)
      state_bytes.append(int(bits, 2))
  usb_link.write(bytearray([0xE7, 0x04, 0x31] + state_bytes + [0x7E]))


# Commands to reverse the LED states

def rev_all():
  """
  Reverse current state of whole cube
  """
  usb_link.write(bytearray([0xE7, 0x10, 0x00, 0x7E]))


def rev_voxel(coordinates = [0, 0, 0]):
  """
  Reverse a voxel state. Parameters:
  - coordinates, an integer list for x, y, z
  """
  usb_link.write(bytearray([0xE7, 0x11, 0x03, coordinates[0], coordinates[1], coordinates[2], 0x7E]))


def rev_line(axis = "x", shift_side_1 = 0, shift_side_2 = 0, led_states = 7 * [True]):
  """
  Turn a line on. Parameters:
  - axis, chose an edge of the cube as reference. A string of "x", "y" or "z"
  - shift_side_1, shift the line from the chosen axis to one of the two adjacent sides. An integer between 0 and 6
  - shift_side_2, shift the line from the chosen axis to one of the two adjacent sides. An integer between 0 and 6
  - led_states, seven booleans
  """
  if axis == "x":
    axis_byte = 0
  elif axis == "y":
    axis_byte = 1
  elif axis == "z":
    axis_byte = 2
  int_states = [int(boolean) for boolean in led_states]
  str_states = [str(boolean) for boolean in int_states]
  str_states.reverse() # bit parameters are from left to right
  bits = "".join(str_states)
  state_byte = int(bits, 2)
  usb_link.write(bytearray([0xE7, 0x12, 0x04, axis_byte, shift_side_1, shift_side_2, state_byte, 0x7E]))


def rev_plan(axis = "xy", shift = 0, led_states = 7 * [True]):
  """
  Turn a plan on, by a line or pixel by pixel, depending of the led_states parameter.
  """
  if type(led_states[0]) is bool:
    rev_plan_by_line(axis, shift, led_states)
  else:
    rev_plan_by_pixel(axis, shift, led_states)


def rev_plan_by_line(axis = "xy", shift = 0, led_states = 7 * [True]):
  """
  Turn a plan on. The columns of the plan are defined by a single line replicated on the seven columns. Parameters:
  - axis, chose two axis letters to select a side of the cube. A string of "xy", "xz" or "yz"
  - shift, move the plan from a side of the cube into it. An integer between 0 and 6
  - led_states, seven booleans figuring a line used on the seven columns of the plan
  """
  if axis == "xz":
    axis_byte = 0
  elif axis == "xy":
    axis_byte = 1
  elif axis == "yz":
    axis_byte = 2
  int_states = [int(boolean) for boolean in led_states]
  str_states = [str(boolean) for boolean in int_states]
  str_states.reverse() # bit parameters are from left to right
  bits = "".join(str_states)
  state_byte = int(bits, 2)
  usb_link.write(bytearray([0xE7, 0x13, 0x03, axis_byte, shift, state_byte, 0x7E]))


def rev_plan_by_pixel(axis = "xy", shift = 0, led_states = 7 * [7 * [True]]):
  """
  Turn a plan on. The columns of the plan are defined pixel by pixel. Parameters:
  - axis, chose two axis letters to select a side of the cube. A string of "xy", "xz" or "yz"
  - shift, move the plan from a side of the cube into it. An integer between 0 and 6
  - led_states, seven booleans figuring a line used on the seven columns of the plan
  """
  if axis == "xz":
    axis_byte = 0
  elif axis == "xy":
    axis_byte = 1
  elif axis == "yz":
    axis_byte = 2
  state_bytes = []
  for i in range(7):
    int_states = [int(boolean) for boolean in led_states[i]]
    str_states = [str(boolean) for boolean in int_states]
    str_states.reverse() # bit parameters are from left to right
    bits = "".join(str_states)
    state_bytes.append(int(bits, 2))
  usb_link.write(bytearray([0xE7, 0x13, 0x09, axis_byte, shift, state_bytes[0], state_bytes[1], state_bytes[2], state_bytes[3], state_bytes[4], state_bytes[5], state_bytes[6], 0x7E]))


def rev_cube(led_states = 7 * [7 * [7 * [True]]]):
  """
  Turn the whole cube on, led by led. Parameters:
  - led_states, seven arrays of seven arrays of seven booleans, to define each voxel
  """
  state_bytes = []
  for i in range(7):
    for j in range(7):
      int_states = [int(boolean) for boolean in led_states[i][j]]
      str_states = [str(boolean) for boolean in int_states]
      str_states.reverse() # bit parameters are from left to right
      bits = "".join(str_states)
      state_bytes.append(int(bits, 2))
  usb_link.write(bytearray([0xE7, 0x14, 0x31] + state_bytes + [0x7E]))


# Animation commands

def translate(axis = "x", step = 1):
  """
  Translate all turned on LED against an axis, optionnaly in steps of more than one LED. Parameters:
  - axis, chose an edge of the cube as reference. A string of "x", "y" or "z"
  - step, an integer between 1 and 7
  """
  move_xyz(axis, step)


def move_xyz(axis = "x", step = 1):
  """
  Translate all turned on LED against an axis, losing the state of LED outside the cube, optionnaly in steps of more than one LED. Parameters:
  - axis, chose an edge of the cube as reference. A string of "x", "y" or "z"
  - step, an integer between 1 and 7
  """
  if axis == "x":
    axis_byte = 0
  elif axis == "y":
    axis_byte = 1
  elif axis == "z":
    axis_byte = 2
  usb_link.write(bytearray([0xE7, 0x21, 0x02, axis_byte, step, 0x7E]))


def modu_xyz(axis = "x", step = 1):
  """
  Translate all turned on LED against an axis, looping on it, optionnaly in steps of more than one LED. Parameters:
  - axis, chose an edge of the cube as reference. A string of "x", "y" or "z"
  - step, an integer between 1 and 7
  """
  if axis == "x":
    axis_byte = 0
  elif axis == "y":
    axis_byte = 1
  elif axis == "z":
    axis_byte = 2
  usb_link.write(bytearray([0xE7, 0x22, 0x02, axis_byte, step, 0x7E]))


def rotate(axis = "x", step = 1):
  rota_xyz(axis, step)


def rota_xyz(axis = "x", step = 1):
  if axis == "x":
    axis_byte = 0
  elif axis == "y":
    axis_byte = 1
  elif axis == "z":
    axis_byte = 2
  usb_link.write(bytearray([0xE7, 0x23, 0x01, axis_byte, 0x7E]))


def mirror(axis = "x", step = 1):
  miro_xyz(axis, step)


def miro_xyz(axis = "x", step = 1):
  if axis == "x":
    axis_byte = 0
  elif axis == "y":
    axis_byte = 1
  elif axis == "z":
    axis_byte = 2
  usb_link.write(bytearray([0xE7, 0x24, 0x01, axis_byte, 0x7E]))


# System commands

def get_version():
  """
  Get the API version
  """
  usb_link.write(bytearray([0xE7, 0xE0, 0x00, 0x7E]))
  return usb_link.readline()


def load_default_parameters():
  """
  Load default parameters for framerate and layerrate
  """
  usb_link.write(bytearray([0xE7, 0xE1, 0x00, 0x7E]))


def load_last_saved_parameters():
  """
  Load last saved parameters from cube memory
  """
  usb_link.write(bytearray([0xE7, 0xE2, 0x00, 0x7E]))


def save_current_parameters():
  """
  Persist the parameters into the cube memory
  """
  usb_link.write(bytearray([0xE7, 0xE3, 0x00, 0x7E]))


# System parameters
# Not yet implemented


# Code execution limited from file
if __name__ == '__main__':
  try:
    print("Connexion on " + serial_ports()[0] + ":")
    usb_link = serial.Serial(serial_ports()[0], 115200)
    print(usb_link.readline())
    print(get_version())
    load_default_parameters()
    save_current_parameters()

    print("Executing tests:")

    # Fixtures
    loops = 5
    duration = 0.5
    state = True
    seven_states = [True, True, True, False, True, False, False]
    fourty_nine_states = [
      [True, True, True, True, True, True, False],
      [True, True, True, True, True, False, False],
      [True, True, True, True, False, False, False],
      [True, True, True, False, False, False, False],
      [True, True, False, False, False, False, False],
      [True, False, False, False, False, False, False],
      [False, False, False, False, False, False, False],
    ]
    full_cube_states = 7 * [fourty_nine_states]

    # Automated tests
    # print('  * set_all()')
    # for _ in range(loops):
    #   set_all()
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * set_all(seven_states)')
    # for _ in range(loops):
    #   set_all(seven_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * set_voxel((1, 1, 1), state)')
    # for _ in range(loops):
    #   set_voxel((1, 1, 1), state)
    #   time.sleep(duration)
    #   state = not state
    # set_all(7 * [False])

    # print('  * set_line("x", 1, 1, seven_states)')
    # for _ in range(loops):
    #   set_line("x", 1, 1, seven_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * set_plan("xz", 1, seven_states)')
    # for _ in range(loops):
    #   set_plan("xz", 1, seven_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * set_plan("xz", 1, fourty_nine_states)')
    # for _ in range(loops):
    #   set_plan("xz", 1, fourty_nine_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * set_cube(full_cube_states)')
    # for _ in range(loops):
    #   set_cube(full_cube_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * rev_all()')
    # for _ in range(loops):
    #   rev_all()
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * rev_voxel((1, 1, 1))')
    # for _ in range(loops):
    #   rev_voxel((1, 1, 1))
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * set_line("x", 1, 1, seven_states)')
    # for _ in range(loops):
    #   set_line("x", 1, 1, seven_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * rev_line("x", 1, 1, seven_states)')
    # for _ in range(loops):
    #   rev_line("x", 1, 1, seven_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * rev_plan("xz", 1, seven_states)')
    # for _ in range(loops):
    #   rev_plan("xz", 1, seven_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * rev_plan("xz", 1, fourty_nine_states)')
    # for _ in range(loops):
    #   rev_plan("xz", 1, fourty_nine_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * rev_cube(full_cube_states)')
    # for _ in range(loops):
    #   rev_cube(full_cube_states)
    #   time.sleep(duration)
    # set_all(7 * [False])

    # print('  * translate("y")')
    # set_plan("xz", 1, seven_states)
    # for _ in range(loops):
    #   translate("y")
    #   time.sleep(duration)
    # set_all(7 * [False])

    print('  * rotate("z")')
    set_plan("xz", 1, seven_states)
    for _ in range(loops):
      rotate("z")
      time.sleep(duration)
    set_all(7 * [False])

    print('  * mirror("z")')
    set_plan("xz", 1, seven_states)
    for _ in range(loops):
      mirror("z")
      time.sleep(duration)
    set_all(7 * [False])

    print("Tests completed")
  except IndexError:
    print("USB connexion not available.")
