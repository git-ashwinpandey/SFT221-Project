#pragma once
#include "mapping.h"

/**
* A shipment is the info about a specific shipment for a customer. Weight (in KG), size (in cubic meters).
* and Point is the destination for that shipment.
*/
struct Shipment {
	int weight;
	double volume;
	struct Point destination;
};

/**
* Truck contains info about different tructs. Capacity (in KG), Volume (in cubic meters),
* route the truck it will take to deliver the shipments, and all shipments in that truck.
*/
struct Truck {
	char name[15];
	int capacity;
	double volume;
	struct Route route;
	struct Shipment* shipments;
	int numShipments;
};

/**
 * Creates a new truck with the given parameters and returns a pointer to the new truck.
 *
 * @param name: The name of the truck.
 * @param capacity: The maximum weight capacity of the truck (in KG).
 * @param volume: The volume capacity of the truck (in cubic meters).
 * @param route: The initial route the truck will take to deliver shipments.
 * @return: A pointer to the newly created truck. Returns NULL on failure.
 */
struct Truck* createTruck(const char* name, int capacity, double volume, struct Route route);

/**
 * Gathers shipment information from the user and processes it until the user decides to stop.
 */
void shipmentInfo();

/**
 * Finds a suitable truck to assign the given shipment based on the truck's capacity, volume,
 * and the distance to the shipment's destination.
 *
 * @param shipment: Pointer to the shipment to be assigned to a truck.
 * @return: Returns 1 if the shipment was successfully assigned, 0 otherwise.
 */
int findTruck(struct Shipment* shipment);

/**
 * Validates the information of the given shipment to ensure it meets the requirements.
 *
 * @param shipment: Pointer to the shipment to be validated.
 * @return: Returns 1 if the shipment is valid, 0 otherwise.
 */
int validateShipmentInfo(struct Shipment* shipment);

/**
 * Adds a new shipment to the given truck, updating the truck's capacity, volume,
 * and the list of shipments.
 *
 * @param truck: Pointer to the truck to which the shipment will be added.
 * @param weight: Weight of the shipment (in KG).
 * @param volume: Volume of the shipment (in cubic meters).
 * @param destination: The destination point for the shipment.
 */
void addShipment(struct Truck* truck, int weight, double volume, struct Point destination);

/**
 * Frees the memory allocated for the truck and its shipments.
 *
 * @param truck: Pointer to the truck to be destroyed.
 */
void destroyTruck(struct Truck* truck);

/**
 * Clears the input buffer to prevent unwanted input issues.
 */
void clearInputBuffer();

/**
 * Prints the diversion route for a given truck to the shipment's destination.
 *
 * @param diversion: The route taken by the truck to divert to the shipment's destination.
 */
void printDiversion(struct Route diversion);