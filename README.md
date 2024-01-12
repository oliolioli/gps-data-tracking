# GPS-data-tracking
Deals with GPS trajectories, measures and converts GPS coordinates into Cartesian coordinates.

## What does it do?
Reads an entire file (CSV) of GPS coordinates from different trajectories.

The CSV is expected to be in the following format:

### Example CSV

[Latitude, Longitude, Height [m]]

46.02541193,7.481110095,2012.0
46.02541000,7.481130000,2011.0
46.02576000,7.481230000,2011.0
46.02578000,7.481170000,2012.0
46.02577000,7.481020000,2015.0
46.02571000,7.480950000,2016.0


### Calculating difference in altitude between departure and arrival
- Calculates and displays the difference between the altitude of departure and that of arrival.
- Calculates the total rise.

### Calculates GPS coordinates into Cartesian coordinates
- Using Cartesian coordinates, calculates the total distance of the trajectory.
- Calculates the slope (Δ altitude / distance) of each segment of the trajectory, and displays the maximum slope.
