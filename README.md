# Watermark ― SPM :helicopter:
[SPM, fall 2019]

----
## What is all about? :mega:
from the :globe_with_meridians:[Wikipedia page](https://en.wikipedia.org/wiki/Digital_watermarking):

> "A digital **watermark** is a kind of marker covertly embedded in a noise-tolerant signal such as [...] **image** data. It is typically used to identify ownership of the copyright of such signal.

>The needed properties of a digital watermark depend on the use case in which it is applied. For marking media files with **copyright** information, a digital watermark has to be rather robust against modifications that can be applied to the carrier signal. Instead, if **integrity** has to be ensured, a fragile watermark would be applied."

---
## The project :pencil:
This project aims at the implementation of a C/C++ multi-thread (and so faster) version of the watermark application.

---
## Run the code :rocket:

first of all you need to download the `CImg.h` header from the [official website](http://cimg.eu/) of the `CImg template library`. Then, clone this repository to your local machine. 

In order to execute the code, copy-and-paste the `CImg.h` into one of the subfolders of the project (let's say: into the `farm-map` directory).

At this point from the `bash` you can compile the code using  `make`.
```bash
make
```
The compilation process will output the `watermark` executable. In the case -- for instance -- of `farm-map`, you can then **run the program** specifying (in order) the following arguments: the path to the directory that contains the images, the path to the watermark image, the path to the output directory, the number of workers of the farm, and the number of threads for each of the worker of the farm (i.e. the number of workers of each map).
```bash
#example
./watermark ~/source/directory/path/ ~/watermark/path/  ~/output/directory/path/ 2 4
```

:exclamation: Pay attention :warning: at the following **specifications**:
- The watermark picture must have three channels (RGB); and
- the dimensions (i.e. width and height) must be greater or equal than the dimensions of any input images;

----
## Visualize the time statistics :chart_with_upwards_trend:
After the program termination, you can run the `plot_timeline.py` script to generate a barchart reporting the time spent by the threads in different regions of code.
```bash
#example
python3 plot_timeline.py farm-map_2_4.txt
```

----
## last update
* 19-Sep-2019 first read-me version