# Real Time Histogram Example

Suppose you have to read data from some resource and then to process the read bytes. But you don't want to block
your main GUI's event loop during either operation. In this example I provide a simple solution provided the
reading is fast enough to place it in the main thread and the processing is the slow enough to place it in another
thread. Please, focus on the `right` version.

## Structure

The goal is to divide the reading and processing into two threads. There are several ways to do this. I've done this right and wrong. See, for example, [this great article](https://wiki.qt.io/Threads_Events_QObjects).

Each version is a standalone project and contain it's own README.

### The `right` version

This is the right way to use multithreading in Qt. It *doesn't* extends the `QThread` class. Instead it uses this
class as a thread manager -- as it is supposed to be.

### The `wrong` version

This is the wrong way to use multithreading in Qt. This comes from the wrong Qt documentation and based on
extending the `QThread` class. Please, don't use this variant in your projects.

## Requirements
  - ~~Qt 5~~ Qt 6
  - [Qwt](https://qwt.sourceforge.io/index.html#installonmainpage) (>= 6.2.0)
  - cmake

## Compilation and Run

Before you start you should specify the `QWT_ROOT` environment variable:
```
export QWT_ROOT=/path/to/Qwt
```
it stores the path to the root directory of the Qwt library on your system. In my case, for example (this line is in the `.bashrc` file):

```
export QWT_ROOT=/usr/local/qwt-6.2.0
```

After you've done with it, clone the repo:
```
git clone git@github.com:LRDPRDX/QwtRealTimeHistogram.git
```
And build an example (either `wrong` or `right`) :
```
cd QwtRealTimeHistogram/right
mkdir build
cd build
```
Generate Makefile and compile:
```
cmake ../
make
```
And run the example:
```
./real
```

NOTE: that I installed Qt 6 from source that is why I need `CMAKE_PREFIX_PATH` in my `CMakeLists.txt`. In your case you might not need it.

