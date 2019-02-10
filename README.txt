
Harris Corner Detector

Developed with Xcode Version 8.2.1
OS X El Capitan

Dependencies:

1) OpenCV 3.3
OpenCV depenencies located in /usr/local/include and /usr/local/lib.

2) OpenCL 1.0

Usage - HarrisCornerDetector IMAGE_PATH CPU/GPU REPORT_PATH
- CPU/GPU defaults to CPU when argument not provided.
- REPORT_PATH defaults to standard out when not provided.

Eg.

HarrisCornerDetector source.jpg
HarrisCornerDetector source.jpg CPU
HarrisCornerDetector source.jpg GPU
HarrisCornerDetector source.jpg CPU report.txt
HarrisCornerDetector source.jpg GPU report.txt

Notes:

If the application is being run from Xcode you will need to change the Scheme Arguments to valid local files on you system.

Eg.

Scheme Arguments currently:
/Users/dkerr/dev/data/checkerboard.jpg GPU /Users/dkerr/Desktop/report.txt

Should change to your local file locations.




