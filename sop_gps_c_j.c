#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Needed for Visualstudio
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


// A. Structure
struct GpsPoint {
    double latitude;
    double longitude;
    float altitude;

    // Addon: Structure for Cartesian coordinates
    double x;
    double y;
    double z;
};


// B. Display a point
void display(struct GpsPoint * point) {
    printf("%9.5f %9.5f %6.0f\n", point->latitude, point->longitude, point->altitude);
}

/* C. Read a line: Extracts a coordinate from a character string like the following: 
    46.94396000,6.718970000,1117.0
    latitude in degrees, longitude in degrees, altitude in meter - separated by commas. */

int readLine(char * line, struct GpsPoint * point) {
    // Scan for latitude, longitude and altitude (seperated by commas) - fill the struct GpsPoint
    sscanf(line, "%lf,%lf,%f", &point->latitude, &point->longitude, &point->altitude);
}

// D. Read the file (read-only)
int readFile(char * fileName, struct GpsPoint * arrayToFill, int length)
{
    // Open the file
    FILE * file = fopen(fileName, "r");
    if (file == NULL) return -1;

    // Read line by line
    int n = 0;
    char buffer[100];
    while (fgets(buffer, 100, file) != NULL) {
        if (n >= length) break;
        int ok = readLine(buffer, &arrayToFill[n]);
        if (ok) n = n + 1;
    }

    // Close the file and return the number of lines read
    fclose(file);
    return n;
}


// H. Function for Transformation in XYZ
void calculeXYZ(struct GpsPoint * point) 
{
    // cos and sin are taking values ​​in radians: radian = degree * (M_PI/180);
    double radius = 6378100;
    double radLatitude = point->latitude * (M_PI/180);
    double radLongitude = point->longitude * (M_PI/180);

    double radiusAltitude = radius + point->altitude;

    // Transforms each GPS coordinate into Cartesian coordinates 
    point->x = (radiusAltitude) * cos(radLatitude) * cos(radLongitude);
    point->y = (radiusAltitude) * cos(radLatitude) * sin(radLongitude);
    point->z = (radiusAltitude) * sin(radLatitude);
}

// I. Function for calculating distance between two points WITHOUT heights
double  distancePointsWithoutZ(struct GpsPoint * a, struct GpsPoint * b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

// I. Function for calculating distance between two points
double distancePoints(struct GpsPoint * a, struct GpsPoint * b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}


// MAIN-----------------------------------------------------------------------------------
int main(int argc, char * argv[]) {

// Struct to fill with readin gps-coordinates
struct GpsPoint points[1000];


/* Read the file and let it fill the gps-struct
    nbPoints contains the number of points,
    or -1 if the file does not exist */
//int nbPoints = readFile("data/ulagan.csv", points, 1000);
//int nbPoints = readFile("data/creux-du-van.csv", points, 1000);
int nbPoints = readFile("data/col-de-riedmatten.csv", points, 1000);


// E. Display the first 10 points
for (int i=0; i<10; ++i) {
    display(&points[i]);
}


// F. Difference in altitude
int altitudeDeparture = points[0].altitude;
int altitudeArrival =  points[nbPoints-1].altitude;
printf("Difference between the altitude of departure and that of arrival: %d m\n", abs(altitudeArrival - altitudeDeparture));


// G. Rise
float rise = 0;
for (int i=0; i < nbPoints-1; ++i) {
    if (points[i+1].altitude > points[i].altitude) {
        rise += points[i+1].altitude - points[i].altitude;
    }
}
printf("Total rise: %6.0f m\n", rise);


// H. Calculate X, Y, Z using the function above
// Add every point Cartesian coordinates
for (int i=0; i < nbPoints; ++i) {
    calculeXYZ(&points[i]);
}


// I. Calculate total distance using the function above
double cartesianDist = 0;

// Calculate distance between all the Cartesian coordinates
for (int i=0; i < nbPoints-1; ++i) {
    double dist = distancePoints(&points[i], &points[i+1]);
    cartesianDist += dist;
}
printf("Distanz: %0.3lf km\n", cartesianDist/1000);


// J. Calculate maximum slope using the function above
double slope = 0;
double maxSlope = 0;
double deltaAltitude = 0;
// Calculates the slope (Δ altitude / distance) of each segment of the trajectory, and displays the maximum slope.
for (int i=0; i < nbPoints-1; ++i) {

    // calculate distance between points
    double dist = distancePoints(&points[i], &points[i+1]);

    // and the difference in height
    deltaAltitude = points[i+1].altitude - points[i].altitude;

    //printf("Distanz: %lf, Höhe: %lf, Steigung: %lf\n", dist, deltaAltitude, deltaAltitude / dist * 100);

    slope = deltaAltitude / dist * 100;

    // and save the max slope
    if (slope > 0) {
        if (maxSlope < slope) {
            maxSlope = slope;
        }
    }
}

printf("MAX Slope: %0.0lf%%\n", maxSlope);

}
