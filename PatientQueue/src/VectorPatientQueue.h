/* File: VectorPatientQueue.h
 * -----------------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file contains a VectorPatientQueue class that employs
 * an unsorted vector. See VectorPatientQueue.cpp for
 * descriptions of methods.
 *
 */

#pragma once

#include <iostream>
#include <string>
#include "patientqueue.h"
#include "vector.h"
using namespace std;

class VectorPatientQueue : public PatientQueue {
public:
    VectorPatientQueue();
    ~VectorPatientQueue();
    string frontName();
    void clear();
    int frontPriority();
    bool isEmpty();
    void newPatient(string name, int priority);
    string processPatient();
    void upgradePatient(string name, int newPriority);
    string toString();

private:
    struct Patient;
    int getMostUrgentIndex();
    Vector<Patient> patients;
};
