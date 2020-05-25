/* File: Trailblazer.cpp
 * ------------------------------------
 * Name: Dannika Thompson
 * TA: Ana-Maria Istrate
 *
 * This file contains 3 algorithms that find paths through graphs representing
 * roadmaps: breadth-first search (BFS), Dijkstra's algorithm, and A* serach.
 * An alternative route function finds the next best route.
 *
 */

#include "Trailblazer.h"
#include "queue.h"
#include "vector.h"
#include "pqueue.h"
using namespace std;

/* Prototypes */
Vector<RoadNode*> aStarHeuristic(const RoadGraph &graph, RoadNode* start, RoadNode* end,
                                  bool isAStar, RoadEdge* edge = NULL);
double pathCost(const RoadGraph& graph, Path& path);
double pathDifference(Path& bestRoute, Path& currentAlt);
Set<RoadNode*> populateSet(Path &path);

/* Constants */
static const double SUFFICIENT_DIFFERENCE = 0.2;

/* Implements the breadth first search algorithm based on the pseudocode from lecture. */
Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Queue<Path> paths;
    Set<RoadNode*> visited;
    Path path;
    path.add(start);
    paths.enqueue(path);

    /* Main search loop */
    while (!paths.isEmpty()) {
        Path current = paths.dequeue();
        RoadNode* node = current[current.size() - 1];
        visited.add(node);
        node->setColor(Color::GREEN);
        /* "Base case" */
        if (node == end) {
            return current;
        }
        /* For each unvisited neighbor, make new path with neighbor as current element. */
        for (RoadNode* next : graph.neighborsOf(node)) {
            if (!visited.contains(next)) {
                next->setColor(Color::YELLOW);
                Path nextPath = current;
                nextPath.add(next);
                paths.enqueue(nextPath);
            }
        }
    }
    /* If nothings is found, return an empty path. */
    Path empty;
    return empty;
}

/*
 * Implements Dijkstra's algorithm (based on pseudocode from class) by executing the A*
 * heuristic with values of 0, effectively omitting the efficient fine-tuning of the heuristic.
 */
Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return aStarHeuristic(graph, start, end, false);
}

/*
 * Implements the A* algorithm using a heuristic that tunes the order of elements in the
 * priority queue to explore more likely elements first, increasing efficiency.
 */
Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    return aStarHeuristic(graph, start, end, true);
}

/*
 * Finds the best alternative route by first finding the best route and ignoring each edge
 * until the alterative route with the lowest cost is found.
 */
Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {
    Path bestRoute = aStar(graph, start, end);
    double bestAltCost = -1.0;
    Path altRoute;

    /* Loops through every edge of best route */
    for (int i = 1; i < bestRoute.size(); i++) {
        /* Alternative route */
        RoadEdge* edge = graph.edgeBetween(bestRoute[i - 1], bestRoute[i]);
        Path currentAlt = aStarHeuristic(graph, start, end, true, edge);

        double difference = pathDifference(bestRoute, currentAlt);
        double altPathCost = pathCost(graph, currentAlt);

        /* Compares cost of current route to best alternate already found */
        if (difference > SUFFICIENT_DIFFERENCE &&
                (altPathCost < bestAltCost || bestAltCost == -1) && !currentAlt.isEmpty()) {
            altRoute = currentAlt;
            bestAltCost = altPathCost;
        }
    }
    return altRoute;
}

/*
 * Broad implementation of the A* heuristic that allows for the executions of Dijkstra's
 * algorithm, A* algorithm, and the edge ignoring condition necessary in the alternative
 * route method.
 */
Vector<RoadNode*> aStarHeuristic(const RoadGraph& graph, RoadNode* start, RoadNode* end,
                                  bool isAStar, RoadEdge* edge) {
    /* Set up priority queue searching algorithm */
    PriorityQueue<Path> paths;
    Set<RoadNode*> visited;
    Path path;
    path.add(start);
    paths.enqueue(path, 0);

    double maxRoadSpeed = graph.maxRoadSpeed();

    /* Main search loop */
    while (!paths.isEmpty()) {
        Path current = paths.dequeue();
        double priority = pathCost(graph, current);
        RoadNode* node = current[current.size() - 1];
        visited.add(node);
        node->setColor(Color::GREEN);
        /* "Base case" */
        if (node == end) {
            return current;
        }

        /* For each unvisited neighbor, make new path with neighbor as current element. */
        for (RoadNode* next : graph.neighborsOf(node)) {
            RoadEdge* nextEdge = graph.edgeBetween(node, next);
            /* Check if edge should be ignored (only possible if alternativeRoute() is called). */
            if (!visited.contains(next) && nextEdge != edge) {
                next->setColor(Color::YELLOW);
                Path nextPath = current;
                nextPath.add(next);

                double crowFlyDistance = graph.crowFlyDistanceBetween(next, end);
                /* If isAStar is false, value is 0. */
                double cost = (crowFlyDistance / maxRoadSpeed) * isAStar;
                double nextPriority = priority + nextEdge->cost() + cost;
                paths.enqueue(nextPath, nextPriority);
            }
        }
    }
    /* If no path is found, return empty path. */
    Path empty;
    return empty;
}

/* Computes the total cost of a path. */
double pathCost(const RoadGraph& graph, Path& path) {
    double cost = 0.0;
    for (int i = 1; i < path.size(); i++) {
        RoadEdge* edge = graph.edgeBetween(path[i - 1], path[i]);
        cost += edge->cost();
    }
    return cost;
}

/* Calculates difference in path between best route and alternative route. */
double pathDifference(Path& bestRoute, Path& currentAlt) {
    Set<RoadNode*> best = populateSet(bestRoute);
    Set<RoadNode*> alt = populateSet(currentAlt);
    Set<RoadNode*> common = best * alt;

    double nodesInPNotB = best.size() - common.size();
    double nodesInB = best.size();

    return nodesInPNotB / nodesInB;
}

/* Returns a set of nodes in a given path. */
Set<RoadNode*> populateSet(Path& path) {
    Set<RoadNode*> pathSet;
    for (RoadNode* node : path) {
        pathSet.add(node);
    }
    return pathSet;
}
