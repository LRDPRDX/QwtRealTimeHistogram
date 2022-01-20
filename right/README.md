# The right way 

Doesn't use the `QThread` as a processing unit. Instead the class `Processor` is moved to the `QThread`.

The example is based on the following diagram :

<p float="center">
  <img src="/right/diagram.png" height="500">
</p>
