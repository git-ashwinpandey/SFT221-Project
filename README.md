# SFT221 – Project

## Learning Outcomes

- Design and build the solution to a small problem,
- Design the testing for the problem.

## Project Description

You have been hired by a local delivery company that has three different trucks that deliver on three
different routes in your part of the city. The map of your city is represented as a 25 by 25 square grid as
shown in the diagram below.

![project-student-2023](https://github.com/git-ashwinpandey/SFT221-Project/assets/64428352/9fec6ecd-91c4-42a2-b80c-f09a6b2f8e4d)


Our offices are located at the square 1A. From that location our three different delivery vans start out
initially on a common path and then branch out to cover different parts of the neighborhood. The paths
taken by our delivery trucks are shown in blue, yellow, and green. Initially, all trucks start out and follow
the blue path and then the green and yellow trucks branch off from the path taken by the blue trucks.
The black rectangles represent buildings to which packages can be delivered. The white areas are empty
space and the blue, yellow, and green areas represent the paths taken by our three delivery trucks. All
addresses are specified in terms of the row number and column letter to which a package should be
delivered.

All our trucks are the same size and can hold up to 1000 kilograms of cargo. The trucks are also capable
of carrying 36 cubic meters of boxes. The boxes that we use for shipping come in 1/4 cubic meter, 1/
cubic meter, and 1 cubic meter sizes. All our boxes are square, meaning that they have the same
dimensions on all sides.

When a customer comes in with a shipment, they specify:


- The weight of the shipment in kilograms,
- The size of the box required in cubic meters,
- The destination of the box in terms of a building specified by a row number and column
    letter that is within the black rectangle represented by the building.

When a shipment comes in, your job is to find a truck which is big enough to hold the shipment as well
as finding a truck which is going to go as close as possible to the destination of the package. The trucks
can divert slightly from their assigned routes and pass through any of the white parts of the grid to
deliver the package. You should always put the package in the truck that we'll have to divert the least
distance to deliver the package. When considering the distance a truck must divert from its route to get
to the destination, you must remember the trucks cannot drive through any of the black buildings on
the map. If two trucks are the same distance away from the destination, you should put the package in
the truck which is less full.

A truck is full when it hits either its maximum weight or maximum volume – whichever is reached first.
For example, if a truck already has 900 kilograms in it but only has 10 cubic meters of boxes then the
limiting factor must be taken as the weight. If another truck has 30 cubic meters of boxes but only 200
kilograms of cargo, then you must assume that it is limited on space rather than weight. When you
compare two trucks to see which one has the most space remaining, you should look at the limiting
factor for each truck as a percentage and compare the percentages.

### Measuring Distance

While it is relatively easy to determine the truck that has the most room or weight available for a
package, it is less obvious how to determine which truck comes closest to the destination for a particular
package. In some cases, it doesn't really matter since they will be equidistant.

Consider the case where a package needs to be delivered to 7F. This location is actually on the route of
two of our trucks. Since the green and blue routes overlap at this point it does not really matter whether
we assign it to a blue or a green truck and that must be determined by the truck which has the most
available space. On the other hand, it is very close to the route of the yellow truck. To determine
whether we should put it in a yellow truck or one of the blue or green trucks we must measure the
distance from the nearest point on the yellow trucks route to destination and the distance from the blue
and green trucks to the destination. Looking at the problem visually, we can see that the blue and green
trucks come within two squares of the destination, but the yellow truck comes within three squares of
the destination. Therefore, since the blue and green trucks come equally close to the destination, the
truck in which the object will be placed is based upon available weight and volume.

Now, let's look at a more difficult problem. Consider the case where we want to make a delivery to 8P.
This is near to both the blue line as well as the green line, but it is not obvious which one is the closest in
order to find which was the closest we have to do a little bit of Euclidean geometry. As you might
remember from school, the distance between any two points is the square root of the sum of the two

sides of the triangle. The nearest point on the blue route is at 11K and the distance to 8P is √ 32 + 52
which equals 5.83. The green truck might be closest at 6 T or it might be closest at 10 T. To figure out
which is closest, we use the technique we just used above to calculate the distance for each. They
distance from 6T 8 P is 4.47 and the distance from 10T to 8P is exactly the same. Of course, 8T is only 4
spaces away from 8P, making it the closest in terms of Euclidian distance. A human can see this but the


human also sees that there is a building in the way and the truck needs to go around it, making that
route even longer.

### The Shortest Path Algorithm

One of the problems we need to solve is how to find the shortest path between two points given that
you cannot go through a building. One solution to this problem is what is called the A* algorithm. The A*
algorithm users the Euclidean distance from where you are on the path to the destination as a heuristic
as to which way to go at any point.

Consider being at 8T and wanting to go to 8P. This is the shortest Euclidian distance to the destination,
but might not be the shortest when you consider not being able to go through buildings. When you are
at 8T, you can go to 7T, 7U, 8U, 9T or 9U since these all touch on 8T and are not part of a building. What
we do next is to measure the Euclidian destination from each of these squares to the destination, as
shown in the table below.


| Start Square | Distance to 8P |
| ------------ | -------------- |
| 7T           | 4.12           |
| 7U           | 5.1            |
| 8U           | 5              |
|9T | 4.12 |
9U | 5. 1 |

From this table you can see that both 7T and 9T are equidistant to destination and our shortest
distances. The algorithm says you always pick the shortest distance and go to that square. Since the two
squares are the same distance, it does not matter which one we pick. Therefore, we will randomly select
7 T as our destination.

Once we get to 7T, we need to go through the same process again. We select all the squares it is
possible to move to and then calculate the distance to the destination square and move to the square
which is closest to the destination. We repeat this process until we reach the destination. If we count
the number of squares we move along the way, we will find the total distance that it takes to get to the
destination.

One of the big problems with the shortest path algorithm is that it can get stuck. It can get stuck either
at the edge of the map or in a corner of a building. You know you are stuck when you have tried every
direction in which you can move forward, and you cannot reach the destination. Remember, you cannot
go back along the path you came, as this can result in going in circles. At this point, you will simply say
that the destination cannot be reached and mark that as not an eligible point to divert from the route to
make a delivery.

### The Overall Algorithm

The overall algorithm will

- Follow the route for each of the trucks.
- At every square on the route for each truck it will calculate the Euclidean distance to the
    destination.


- It will select the minimum Euclidean distance for each of the trucks and then calculate the
    shortest path from each of those positions to the destination.
- In the event one of the trucks cannot find a path to the destination, that truck will not be used
    for the delivery and one of the other trucks will be used.
- Finally, it will select the truck which has the shortest path from the nearest point on its route to
    the destination and attempt to add the package to that truck.
- If that truck cannot hold the package, it will try to put it in the truck that is next closest to the
    destination.
- If no truck can take the package, it will be placed in storage at the depot until the trucks return
    empty and it will be shipped out the next day. It will print the message “Ships tomorrow”.

Our algorithm will run for one day. This means we accept items until all trucks are full or items stop
arriving. Once this happens, the trucks will be dispatched but that is not the responsibility of the
program. All the program needs to do is:

- Decide which truck the package will be placed in.
- Print out where the truck will deliver the package.
- If the truck needs to divert to deliver the package, you will print the path to divert.

```
## Sample Output

Seneca Deliveries
=================
Enter shipment weight, box size and destination (0 0 x to stop): 20 .5 28x
Invalid destination
Enter shipment weight, box size and destination (0 0 x to stop): 20 2 12L
Invalid size
Enter shipment weight, box size and destination (0 0 x to stop): 1005 .5 12L
Invalid weight (must be 1-1000 Kg.)
Enter shipment weight, box size and destination (0 0 x to stop): 20 .5 12L
Ship on BLUE LINE, no diversion
Enter shipment weight, box size and destination (0 0 x to stop): 200 1.0 8Y
Ship on GREEN LINE, divert: 7T, 7U, 7V, 7W, 7X, 7Y, 8Y
Enter shipment weight, box size and destination (0 0 x to stop): 50 0 1.0 8Y
Ship on GREEN LINE, divert: 7T, 7U, 7V, 7W, 7X, 7Y, 8Y
Enter shipment weight, box size and destination (0 0 x to stop): 500 1.0 8Y
Ship on BLUE LINE, divert 18V, 17V, 16V, 15V, 14V, 13V, 12V, 11V, 10V, 9V,
8V, 7V, 7W, 7X, 7Y, 8Y
Enter shipment weight, box size and destination (0 0 x to stop): 0 0 x
Thanks for shipping with Seneca!
```
