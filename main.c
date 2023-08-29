#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "mapping.h"
#include "shipping.h"

struct Truck* yellowTruck;
struct Truck* blueTruck;
struct Truck* greenTruck;

int main(void)
{
	yellowTruck = createTruck("Yellow", 1000, 36.0, getYellowRoute());
	blueTruck = createTruck("Blue", 1000, 36.0, getBlueRoute());
	greenTruck = createTruck("Green", 1000, 36.0, getGreenRoute());

	shipmentInfo();

	

	// Free up memory for the trucks
	destroyTruck(yellowTruck);
	destroyTruck(blueTruck);
	destroyTruck(greenTruck);

	return 0;
}

// Create a new truck with dynamic shipments
struct Truck* createTruck(const char* name, int capacity, double volume, struct Route route) {
	struct Truck* truck = malloc(sizeof(struct Truck));
	if (truck == NULL) {
		printf("Failed to allocate memory for truck\n");
		return NULL;
	}

	// Set the truck name
	strncpy(truck->name, name, sizeof(truck->name) - 1);
	truck->name[sizeof(truck->name) - 1] = '\0'; // Ensure null-termination

	truck->capacity = capacity;
	truck->volume = volume;
	truck->route = route;
	truck->shipments = NULL;  // Initialize shipments to NULL
	truck->numShipments = 0;  // Initialize numShipments to 0
	return truck;
}

void shipmentInfo() {
	int stopAccepting = 0;
	while (!stopAccepting) {
		struct Shipment shipment;

		printf("Enter shipment Weight, Box size, and destination: ");
		scanf("%d %lf %d %c", &shipment.weight, &shipment.volume, &shipment.destination.row, &shipment.destination.col);
		clearInputBuffer();

		if (shipment.weight == 0 && shipment.volume == 0 && shipment.destination.row == 0 && shipment.destination.col == 'X') {
			stopAccepting = 1;
			printf("Stopping Now\n");
		}
		else if (validateShipmentInfo(&shipment) == 0) {
			stopAccepting = 1;
			printf("Invalid shipment, stopping now\n");
		}
		else {
			printf("Weight: %d\nVolume: %.1lf\nDestination: %d%c\n", shipment.weight, shipment.volume, shipment.destination.row, shipment.destination.col);
			printf("\nTime to find a truck....\n");
			shipment.destination.col -= 'A';
			shipment.destination.row -= 1;
			stopAccepting = findTruck(&shipment);
		}
	}
}

int validateShipmentInfo(struct Shipment* shipment) {
	int result = 1;

	if (shipment->weight < 0 || shipment->weight > 1000) {
		printf("Incorrect Weight Value: %d\n", shipment->weight);
		result = 0;
	}
	if (shipment->volume != 0.25 && shipment->volume != 0.5 && shipment->volume != 1) {
		printf("Incorrect Size: %.2lf\n", shipment->volume);
		result = 0;
	}
	if (shipment->destination.row < 1 || shipment->destination.row > 25) {
		printf("Incorrect destination row: %d\n", shipment->destination.row);
		result = 0;
	}
	if (shipment->destination.col < 'A' || shipment->destination.col > 'Y') {
		printf("Incorrect Destination col: %c\n", shipment->destination.col);
		result = 0;
	}

	return result;
}

int findTruck(struct Shipment* shipment) {
	int stopAccepting = 0;
	struct Route shortestRoute[3];
	struct Truck* trucks[] = { yellowTruck, blueTruck, greenTruck };
	int truckCount = 3;

	//struct Point startPoint = { 1, 1 };
	struct Map baseMap = populateMap();

	// Calculate the shortest points for each truck
	int shortestPoints[3];
	for (int i = 0; i < truckCount; i++) {
		// For each truck, get the closest point from their normal route to the shipment destination.
		int closestPointIndex = getClosestPoint(&trucks[i]->route, shipment->destination);
		// Store that closest Point for each of the truck in a Point array. (Starting Point)
		struct Point startPoint = trucks[i]->route.points[closestPointIndex];
		// From that Starting Point, find a route to the shipment destination.
		shortestRoute[i] = shortestPath(&baseMap, startPoint, shipment->destination);
		// Store the number of points in the shortest route to select a truck. 
		shortestPoints[i] = shortestRoute[i].numPoints;
	}

	// Sort the trucks based on shortest points (using bubble sort)
	for (int i = 0; i < truckCount - 1; i++) {
		for (int j = 0; j < truckCount - i - 1; j++) {
			if (shortestPoints[j] > shortestPoints[j + 1]) {
				// Swap shortest points
				int temp = shortestPoints[j];
				shortestPoints[j] = shortestPoints[j + 1];
				shortestPoints[j + 1] = temp;

				//Swap Route
				struct Route tempRoute = shortestRoute[j];
				shortestRoute[j] = shortestRoute[j + 1];
				shortestRoute[j + 1] = tempRoute;

				// Swap trucks
				struct Truck* tempTruck = trucks[j];
				trucks[j] = trucks[j + 1];
				trucks[j + 1] = tempTruck;
			}
		}
	}

	// Find the first truck with enough capacity and volume for the shipment
	struct Truck* selectedTruck = NULL;
	for (int i = 0; i < truckCount; i++) {
		if (trucks[i]->capacity >= shipment->weight && trucks[i]->volume >= shipment->volume) {
			selectedTruck = trucks[i];
			if (selectedTruck != NULL) {
				// Check if the selected truck has enough capacity and volume for the shipment
				if (selectedTruck->capacity >= shipment->weight && selectedTruck->volume >= shipment->volume) {
					// Add the shipment to the selected truck
					addShipment(selectedTruck, shipment->weight, shipment->volume, shipment->destination);
					printf("Shipment added to %s truck successfully.\n", selectedTruck->name);
					printDiversion(shortestRoute[i]);
					break;
				}
				else {
					printf("Package cannot be assigned to the selected truck\n");
					selectedTruck = NULL;
				}
			}
		}
	}

	// If no truck can accommodate the shipment, set stopAccepting to 1
	if (selectedTruck == NULL) {
		printf("Package will ship tomorrow\n");
		stopAccepting = 1;
	}
	return stopAccepting;
}

void printDiversion(struct Route diversion) {
	int i;
	if (diversion.numPoints == 0) {
		printf("No Diversion needed\n");
	}
	else {
		printf("Divert: ");
		for (i = 0; i < diversion.numPoints; i++) {
			printf("%d%c", diversion.points[i].row+1, diversion.points[i].col + 'A');
			if (i + 1 < diversion.numPoints) {
				printf("->");
			}
		}
	}
	printf("\n\n");
}



// Add a new shipment to the truck
void addShipment(struct Truck* truck, int weight, double volume, struct Point destination) {
	// Verify the truck parameter
	if (truck == NULL) {
		printf("Invalid truck\n");
		return;
	}

	// Check if the truck has enough capacity and volume space for the shipment
	int remainingCapacity = truck->capacity - weight;
	double remainingVolume = truck->volume - volume;
	if (remainingCapacity < 0 || remainingVolume < 0) {
		printf("Truck is full, cannot add shipment\n");
		return;
	}

	// Allocate memory for the new shipment
	struct Shipment* newShipment = malloc(sizeof(struct Shipment));
	if (newShipment == NULL) {
		printf("Failed to allocate memory for shipment\n");
		return;
	}

	// Set the properties of the new shipment
	newShipment->weight = weight;
	newShipment->volume = volume;
	newShipment->destination = destination;

	// Increase the shipment count and update the shipments array
	truck->numShipments++;
	truck->shipments = realloc(truck->shipments, truck->numShipments * sizeof(struct Shipment));
	if (truck->shipments == NULL) {
		printf("Failed to reallocate memory for shipments\n");
		return;
	}
	truck->shipments[truck->numShipments - 1] = *newShipment;

	// Update the remaining capacity and volume of the truck
	truck->capacity = remainingCapacity;
	truck->volume = remainingVolume;
}

// Free the memory allocated for the truck and its shipments
void destroyTruck(struct Truck* truck) {
	if (truck == NULL) {
		printf("Invalid truck\n");
		return;
	}

	// Free memory for each shipment in the truck
	for (int i = 0; i < truck->numShipments; i++) {
		free(&truck->shipments[i]);
	}
	//free(truck->shipments);

	// Free the memory allocated for the truck
	free(truck);
}

void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		// Discard characters until newline or end of file is encountered
	}
}
