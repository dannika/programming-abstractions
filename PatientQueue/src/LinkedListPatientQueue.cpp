/* File: LinkedListPatientQueue.cpp
 * -----------------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file implements the LinkedListPatientQueue class.
 *
 */

#include "LinkedListPatientQueue.h"

/* Constructs a patient priority queue in the form of a linked list */
LinkedListPatientQueue::LinkedListPatientQueue() {
    start = NULL;
}

/* Frees up used memory, deleting all nodes in linked list  */
LinkedListPatientQueue::~LinkedListPatientQueue() {
    clear();
    delete start;
}

/* Removes all elements from the patient queue */
void LinkedListPatientQueue::clear() {
    while (start != NULL) {
        PatientNode *temp = start;
        start = start->next;
        delete temp;
    }
}

/* Returns the name of the most urgent patient in the queue */
string LinkedListPatientQueue::frontName() {
    if (isEmpty()) {
        throw("The queue is empty.");
    }
    return start->name;
}

/* Returns the priority of the most urgent patient in the queue */
int LinkedListPatientQueue::frontPriority() {
    if (isEmpty()) {
        throw("The queue is empty.");
    }
    return start->priority;
}

/* Returns true if the patient queue is empty and false if otherwise */
bool LinkedListPatientQueue::isEmpty() {
    return (start == NULL);
}

/* Adds a given patient to the sorted queue according to the given priority */
void LinkedListPatientQueue::newPatient(string name, int priority) {
    PatientNode *node = new PatientNode(name, priority);
    PatientNode *previous = NULL;
    PatientNode *current = start;

    while (current != NULL) {
        if (current->priority > node->priority) {
            break;
        }
        previous = current;
        current = current->next;
    }

    node->next = current;
    if (previous == NULL) {
        start = node;
    } else {
        previous->next = node;
    }
}

/* Removes the most urgent patient from the queue and returns their name */
string LinkedListPatientQueue::processPatient() {
    if (isEmpty()) {
        throw ("The queue is empty.");
    }
    PatientNode *temp = start;
    string firstName = start->name;
    start = start->next;
    delete temp;
    return firstName;
}

/* Modifies the priority of an existing patient to become more urgent */
void LinkedListPatientQueue::upgradePatient(string name, int newPriority) {
    PatientNode *previous = NULL;
    PatientNode *node = start;

    while (node->name != name) {
        if (node->next != NULL) {
            previous = node;
            node = node->next;
        } else {
            throw ("This patient is not in the queue.");
        }
    }

    if (node->priority <= newPriority) {
        throw("The patient already has a more urgent priority.");
    } else {
        if (node == start) {
            processPatient();
        } else {
            previous->next = node->next;
            delete node;
        }
        newPatient(name, newPriority);
    }
}

/* Returns a formatted, sorted patient queue */
string LinkedListPatientQueue::toString() {
    PatientNode *node = start;
    string result = "{";
    while (node != NULL) {
        result += integerToString(node->priority) + ":" + node->name;
        if (node->next != NULL) {
            result += ", ";
        }
        node = node->next;
    }
    result += "}";
    return result;
}
