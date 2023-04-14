# Scripts

This directory contains the script(s) necessary for the finalized version of our project. However, the generated GNU Radio flowgraph can be found under `B-2-BAPS/GNURadio`. 

# Script Details

## snr_data.py

### Summary
   + This script takes in data obtained via ZMQ, calculates the adjusted SNR in dB (if necessary), and then transfer that data rounded to 3 decimal places to the Arduino.
   + For all major mathematical functions (mean, standard deviation, etc), Numpy is used.
   + Users do need privledges to allow for UART communication between the Raspberry Pi (or the device running `snr_data.py` and the generated GNU Radio Python file). 
   + The SNR calculations done via `snr()` are based off of the redefinition of the SNR, the coefficient of variation

### Function Detail
   + `snr(a)` takes in a set of data (a signal, denoted by `a`) and then calcuates the [coefficient of variation](https://en.wikipedia.org/wiki/Signal-to-noise_ratio). This is simply the mean of the distribution divided by the standard deviation. To avoid division by zero errors, `np.where` is opted to allow a 0 if the SD is 0, or the ratio if it is not.

