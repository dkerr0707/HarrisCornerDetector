
Harris Corner Detector

Developed with Xcode Version 8.2.1
OS X El Capitan

Dependencies:

1) OpenCV 3.3
OpenCV dependencies located in /usr/local/include and /usr/local/lib.

2) OpenCL 1.1

Usage - HarrisCornerDetector IMAGE_PATH CPU/GPU REPORT_PATH
- CPU/GPU defaults to CPU when argument not provided.
- REPORT_PATH defaults to standard out when not provided.

Eg.

HarrisCornerDetector source.jpg
HarrisCornerDetector source.jpg CPU
HarrisCornerDetector source.jpg GPU
HarrisCornerDetector source.jpg CPU report.txt
HarrisCornerDetector source.jpg GPU report.txt

The ExampleResults directory shows the expected results with the given source image:

- exampleCommands.txt shows the commands used to generate the results in the folder.
- reportCPU.txt shows the example CPU report.
- reportGPU.txt shows the example GPU report.
- source.jpg shows the source image used for testing.
- result.jpg shows the output result when the program is executed. Red circles are draw on the found corners.





