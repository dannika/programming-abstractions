/* File: LinkedListPatientQueue.h
 * -----------------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file contains a LinkedListPatientQueue class that employs
 * a sorted linked list. See LinkedListPatientQueue.cpp for
 * descriptions of methods.
 *
 */

#pragma once

#include <iostream>
#include <string>
#include "patientnode.h"
#include "patientqueue.h"
using namespace std;

class LinkedListPatientQueue : public PatientQueue {
public:
    LinkedListPatientQueue();
    ~LinkedListPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    PatientNode *start;
};
