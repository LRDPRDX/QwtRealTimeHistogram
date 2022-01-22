# The right way 

Doesn't use the `QThread` as a processing unit. Instead the class `Processor` is moved to a `QThread`.

The example can be illustrated by the following diagram :

<p float="center">
  <img src="/right/diagram.png" height="500">
</p>

There are two classes in this example :

- `Producer`
- `Processor`

The `Producer` class is the reader - it gets the bytes from the file and if succeeded notifies the processor by
sending the `DataReady` signal.
The `Producer` lives in the main thread.

The `Processor` class transfroms the read data (supposed to be a uniform distribution) into a gaussian distribution.
The `Processor` lives in a separate thread.

The result is the following:

<p float="center">
  <img src="/right/result.gif" height="500">
</p>
