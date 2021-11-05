# Real Time Histogram Example

<p float="center">
  <img src="/result.gif" height="500">
</p>

## Compilation and Run

Before you start you should specify the `QWT_ROOT` environment variable:
```
export QWT_ROOT=/path/to/Qwt
```
it stores the path to the root directory of the Qwt library on your system. In my case, for example (this line is in the `.bashrc`):

```
export QWT_ROOT=/usr/local/qwt-6.1.2
```

After you've done with it, clone the repo:
```
git clone git@github.com:LRDPRDX/QwtRealTimeHistogram.git
```
Create a dir to build the example:
```
cd QwtRealTimeHistogram
mkdir build
cd build
```
Generate a Makefile and compile:
```
cmake ../
make
```
And run the example:
./real
