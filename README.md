# Motifit

## Goal
To create a motivation for students and teachers to improve their own health.
We will do this by making IoT systems that can measure the amount of people taking the stairs on the campus.
With this data, we will do a few things to make students and teachers more aware of their behavior:
* Show the amount of people that have taken the stairs (week/month/year)
* Set and show goals for weeks, months and year.
Optional:
* Show a comparison between different buildings and departments to improve competition

## design requirements 
For this project we will use the following hardware:

* Photons: are used to communicate between the different devices in different buildings.
* Pressure detectors or infrared lights and detectors: are used to detect people on the stairs.
* Led screens or servos pointing to a percentage: are used to show the current status.

All the data will be saved on the photons themselves instead of a central server. The following data will be saved:

* The addresses of photons that count together. For example: in one building or faculty.
* The number of people who took the stairs detected by one photon, grouped by day.
* The sum of all the people who took the stairs grouped together by photons that count together.

## Current example in Aarhus
On the campus there is already a similar idea used to promote biking:

<img src="https://github.com/winterworks/Motifit/blob/master/bikes.jpg" width="512">

