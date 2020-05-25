/* File: VectorPatientQueue.cpp
 * -----------------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file implements the VectorPatientQueue class.
 *
 */

#include "VectorPatientQueue.h"
#include "strlib.h"

/* Defines the Patient struct */
struct VectorPatientQueue::Patient {
    string name;
    int priority;
};

/* Constructs an empty patient priority queue stored as a vector */
VectorPatientQueue::VectorPatientQueue() {
    // does nothing
}

/* Cleans memory associated with patient queue */
VectorPatientQueue::~VectorPatientQueue() {
    // does nothing
}

/* Removes all elements from the patient queue */
void VectorPatientQueue::clear() {
    patients.clear();
}

/* Returns the name of the most urgent patient in the queue */
string VectorPatientQueue::frontName() {
    if (isEmpty()) {
        throw("Error. There are no patients in the queue.");
    }

    int urgentIndex = getMostUrgentIndex();
    return patients[urgentIndex].name;
}

/* Returns the priority value of the most urgent patient in the queue */
int VectorPatientQueue::frontPriority() {
    if (isEmpty()) {
        throw("Error. There are no patients in the queue.");
    }

    int urgentIndex = getMostUrgentIndex();
    return patients[urgentIndex].priority;
}

/* Returns true if the patient queue is empty and false if otherwise */
bool VectorPatientQueue::isEmpty() {
    return patients.isEmpty();
}

/* Adds a patient with a given name and priority to the queue */
void VectorPatientQueue::newPatient(string name, int priority) {
    Patient patientNew = {name, priority};
    patients.add(patientNew);
}

/* Removes the most urgent patient from the queue and returns their name */
string VectorPatientQueue::processPatient() {
    if (isEmpty()) {
        throw("Error. There are no patients in the queue.");
    }

    string nameMostUrgent = frontName();
    int indexMostUrgent = getMostUrgentIndex();
    patients.remove(indexMostUrgent);
    return nameMostUrgent;
}

/* Modifies the priority of an existing patient to become more urgent */
void VectorPatientQueue::upgradePatient(string name, int newPriority) {    
    int index = -1;
    int currentPriority = INT_MAX;
    int queueSize = patients.size();
    for (int i = 0; i < queueSize; i++) {
        if (patients[i].name == name && patients[i].priority < currentPriority) {
            currentPriority = patients[i].priority;
            index = i;
        }
    }

    if (index == -1) {
        throw("This patient is not present in the queue.");
    } else {
        if (currentPriority <= newPriority) {
            throw("This patient already has a more urgent priority.");
        } else {
            string tempName = patients[index].name;
            patients.remove(index);
            newPatient(tempName, newPriority);
        }
    }
}

/* Returns a formatted, unsorted patient queue */
string VectorPatientQueue::toString() {
    string result = "";
    int queueSize = patients.size();
    for (int i = 0; i < queueSize; i++) {
        result += integerToString(patients[i].priority) + ":" + patients[i].name;
        if (i != queueSize - 1) {
            result += ", ";
        }
    }
    return "{" + result + "}";
}

/* Returns the index of the most urgent element of the patient queue */
int VectorPatientQueue::getMostUrgentIndex() {
    int index = 0;
    for (int i = 0; i < patients.size(); i++) {
        if (i != index) {
            int urgent = patients[index].priority;
            int current = patients[i].priority;

            if (urgent > current) {
                index = i;
            }
        }
    }
    return index;
}
