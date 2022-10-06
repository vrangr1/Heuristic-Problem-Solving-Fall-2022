#!/usr/bin/env python

import re
import pandas as pd
import sys

import matplotlib.pyplot as plt
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)


def take_time(a, b):
    return abs(a.x - b.x) + abs(a.y - b.y)


# Exception classes
class ValidationError(ValueError):
    pass


class FormatSyntaxError(ValidationError):
    pass


class DataMismatchError(ValidationError):
    pass


class IllegalPlanError(ValidationError):
    pass


#  Person object
PID = 0

# set to True to print data.
print_input_file = False

class Person:

    def __init__(self, x, y, st):
        global PID
        PID += 1
        self.pid = PID
        self.x = x
        self.y = y
        self.st = st
        self.rescued = False
        return

    def __repr__(self):
        return 'P%d:(%d,%d,%d)' % (self.pid, self.x, self.y, self.st)

    def prettify(self):
        return {self.pid: (self.x, self.y, self.st)}


#  Hospital object
HID = 0

class Hospital:

    def __init__(self, x, y, num_amb):
        global HID
        HID += 1
        self.hid = HID
        self.x = x
        self.y = y
        # amb_time array represents the time at which each ambulance ended its last tour.
        self.num_amb = num_amb
        self.amb_time = [0] * num_amb
        return

    def __repr__(self):
        return 'H%d:(%d,%d)' % (self.hid, self.x, self.y)

    def prettify(self):
        return {self.hid: (self.x, self.y)}

    def rescue(self, pers, end_hospital, start_time):
        if self.num_amb == 0:
            raise IllegalPlanError('No ambulance left at the hospital %s.' % self)
        else:
            self.amb_time.sort()
            if self.amb_time[0] > start_time:
                raise IllegalPlanError('No ambulance left at hospital %s at the start time %d minutes.' % (self, start_time))
        if 4 < len(pers):
            raise IllegalPlanError('Ignoring line as cannot rescue more than four people at once: %s.' % pers)
        already_rescued = list(filter(lambda p: p.rescued, pers))
        if already_rescued:
            print('Person %s already rescued.' % already_rescued)
        # t: time when end hospital is reached
        t = start_time + 1
        start = self
        for p in pers:
            t += take_time(start, p)
            start = p

        t += len(pers)
        t += take_time(start, end_hospital)

        self.amb_time.sort()
        rescued_persons = []
        for (i, t0) in enumerate(self.amb_time):
            if (t0 > start_time):
                continue
            rescued_persons = list(filter(lambda p: p.st >= t and p.rescued is False, pers))
            break
        self.amb_time[i] = t

        end_hospital.num_amb += 1
        end_hospital.amb_time.append(self.amb_time[i])

        self.num_amb -= 1
        self.amb_time.pop(i)

        for (i,p) in enumerate(rescued_persons):
            p.rescued = True
        if len(rescued_persons)==0:
            print('Nobody will make it by end of %d minutes.' % t)
        else:
            if len(rescued_persons)==len(pers):
                print('Rescued:', ' and '.join(map(str, rescued_persons)), 'at', t, 'minutes | Ended at Hospital', end_hospital)
            else:
                print('Rescued:', ' and '.join(map(str, rescued_persons)), 'at', t, 'minutes | Ended at Hospital', end_hospital, "| Rest could not make it in time.")
        return rescued_persons


# read_data
def read_data(fname="data.txt"):
    persons = []
    hospitals = []
    mode = 0

    with open(fname, 'r') as fil:
        data = fil.readlines()

    for line in data:
        line = line.strip().lower()
        if line.startswith("person") or line.startswith("people"):
            mode = 1
        elif line.startswith("hospital"):
            mode = 2
        elif line:
            if mode == 1:
                (a, b, c) = map(int, line.split(","))
                persons.append(Person(a, b, c))
            elif mode == 2:
                (c,) = map(int, line.split(","))
                hospitals.append(Hospital(-1, -1, c))

    if print_input_file:
        print('Reading data:', fname)
        print(persons)
        print(hospitals)
    return persons, hospitals


# read_results
def readresults(persons, hospitals, fname='sample_result.txt'):
    print('Reading data:', fname)
    res = {}
    score = 0
    hospital_index = 0
    with open(fname, 'r') as fil:
        data = fil.readlines()

    for (i, line) in enumerate(data):
        # check for hospital coordinates
        if line.startswith('H'):
            print("\n" + line, end=" ")
            # read in hospital coordinates and set on hospital object
            hospital_no = int(line.replace('H', '').split(':')[0])
            hospital_coordinates = line.replace('H', '').split(':')[1].split(',')
            (x, y) = [int(coordinates) for coordinates in hospital_coordinates]
            print("Hospital #{idx}: coordinates ({x},{y})".format(x=x, y=y, idx=hospital_index + 1))
            if not (x and y):
                raise ValidationError("Hospital coordinates not set: line".format(line))
            hospitals[hospital_index].x = x
            hospitals[hospital_index].y = y
            hospital_index += 1
            data[i] = '0 .' + line
        else:
            line = line.strip()
            if not line:
                data[i] = '0 .' + line


    data.sort(key = lambda x: int(x.split()[0]))
    for line in data:
        line = line.strip()
        if not line or line.startswith('0 .'):
            continue

        print("\n" + line, end="\n ")

        # check for ambulance records
        try:
            hos = None
            end_hos = None
            rescue_persons = []
            start_time = int(line.split()[0])
            route = line.split()[1:]

            for (i, w) in enumerate(route):
                # Hospital 
                if w[0] == 'H':
                    hospital_index = int(w[1:])
                    if hospital_index <= 0 or len(hospitals) < hospital_index:
                        raise FormatSyntaxError('Illegal hospital id: %d' % hospital_index)
                    if i!=0 and i!=len(route)-1:
                        raise FormatSyntaxError('Specify hospitals only at beginning or at end of route: %r' % line)
                    if i == 0:
                        hos = hospitals[hospital_index - 1]
                    else:
                        end_hos = hospitals[hospital_index - 1]
                    continue

                # Person
                if w[0] == 'P':
                    if i==0 or i==len(route)-1:
                        raise FormatSyntaxError('Specify hospitals at beginning or at end of route: %r' % line)
                    person_index = int(w[1:])
                    if person_index <= 0 or len(persons) < person_index:
                        raise FormatSyntaxError('Illegal person id: %d' % person_index)
                    per = persons[person_index - 1]
                    rescue_persons.append(per)
                    continue
                # error
                raise FormatSyntaxError('!!! Incorrect format: %r' % line)

            if not hos or not rescue_persons:
                print('!!! Insufficient data: %r' % line)
                continue
            if not hos or not end_hos:
                raise FormatSyntaxError('Either start hospital or end hospital is not defined.')
            rescue_persons = hos.rescue(rescue_persons, end_hos, start_time)
            if hos.hid in res:
                res[hos.hid].extend(rescue_persons)
            else:
                res[hos.hid] = rescue_persons
            score += len(rescue_persons)
        except ValidationError as x:
            print('!!!', x)

        # uncomment the following lines to see the hospital-ambulance status after every ambulance tour line            
        # for hos in hospitals: 
        #     print("Hospital ", hos.hid, "::: ", end = " ") 
        #     for t in hos.amb_time: 
        #         print(t, end = " ") 
        #     print()
        # print("___________________________\n") 

    print()
    print('Total score:', score)
    print()
    return res


def process_list(hosp, row):
    global hospitals
    x, y = hospitals.iloc[hosp-1]["x"], hospitals.iloc[hosp-1]["y"]
    values = [(x, y)] + row

    temp = [values[0]]
    for i in range(len(values)-1):
        temp.append((values[i][0], values[i+1][1]))
        temp.append(values[i+1])
    return temp


def prettify(self):
    try:
        return {self.pid: (self.x, self.y, self.st)}
    except AttributeError:
        return {self.hid: (self.x, self.y)}


def my_solution(pers, hosps):
    """
    Place your custom solution here
    Your code SHOULD create a result.txt file consisting of the solution as per the output format
    As such, you do not need to read the data and can use the read_data() function directly.
    However, you're free to read the data if you wish to use a separate data structure
    IO Format:
        Refer to the Readme: "For Languages other than python" to know the input and output formats
    :return: Create a file named "result.txt" following the output format
    """

    # Enter your logic here

    # with open("outputs/team_name.txt", "w+") as fil:
    #     op = ""  # Change this to your solution
    #     fil.write(op)
    return


# Main
if __name__ == "__main__":
    input_file = "input_data.txt"
    result_file = "sample_result.txt"
    if len(sys.argv) > 1:
        input_file = sys.argv[1]
    if len(sys.argv) > 2:
        result_file = sys.argv[2]
    print("Using " + input_file + " as input.\n")
    (persons, hospitals) = read_data(input_file)
    my_solution(persons, hospitals)
    readresults(persons, hospitals, result_file)
