import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

np.random.seed(5)

#This function creates a sine wave of specified value
def sine_wave(freq, phase=0, fs=10000):
    rad = np.radians(phase)
    t = np.arange(0,fs) / fs
    return np.sin(2 * np.pi * freq * t + rad)

#This function creates a sine wave of specified value
def cos_wave(freq, phase=0, fs=10000):
    rad = (phase / 360) * (2.0 * np.pi)
    t = np.arange(0,fs) / fs
    return np.cos(2 * np.pi * freq * t + rad)

#This function creates a low pass filter to pass signals through
def bw_low_pass(fs, cutoff, order, data):
    ncf = cutoff / (0.5 * fs)
    b,a = signal.butter(order, ncf, btype='low', analog=False)
    return signal.filtfilt(b,a,data)

#This function generates a noise of specified value
def gen_noise(low=-1, high=1, size=10000):
    return np.clip(np.random.normal(0,1,size), low, high)

""" 
This function generates a sine wave and adds noise to the wave then plots the wave
both before and after implementing the low pass filter. Different noise levels were also 
incorporated """
def show_lock_in():
    sine = sine_wave(100, fs=20000)
    noisy_sine = gen_noise(size=20000) + 0.1*sine_wave(100, fs=20000)
    two_sines = sine * sine_wave(102, fs=20000)
    """
    plt.subplot(4,2,1)
    plt.gca().set_title("Test")
    plt.plot(two_sines, label="100Hz Sine Wave * 120Hz Sine Wave")
    plt.plot(np.full(two_sines.shape[0], np.mean(two_sines)), 'r', label="Mean of output")
    """
    print(np.full(two_sines.shape[0], np.mean(two_sines)))
    plt.subplot(3,1,1)
    noisy_with_regular = noisy_sine*sine
    noisy_wr_mean = np.full(noisy_with_regular.shape[0], np.mean(noisy_with_regular))
    plt.plot(noisy_with_regular, 'orange', label="Noisy Sine Wave")
    plt.plot(noisy_wr_mean, 'g', label="Mean of noisy sine wave * regular sine wave")
    plt.gca().set_title("Noisy Sine Wave * Regular Sine Wave and Corresponding Mean")
    plt.legend()
    print(noisy_wr_mean)
    
    plt.subplot(3,1,2)
    y = bw_low_pass(20000, 5, 1, sine_wave(120, fs=20000) * sine_wave(120,fs=20000))
    plt.plot(y, label="120Hz Sine Wave Multiplied Itself")
    plt.gca().set_title("Low-pass Filter Applied to a Signal (no noise)")
    plt.legend()


    plt.subplot(3,1,3)
    y_noisy = bw_low_pass(20000, 10, 1, noisy_with_regular)
    plt.plot(y_noisy, 'b', label="100Hz Sine Wave with Noise")
    plt.gca().set_title("Low-pass Filter Applied to a Signal (w/ noise)")

    plt.tight_layout()
    plt.legend()
    plt.show()
    
#This function plots the phases of each sine wave with noise after passing it through a low pass filter
def show_phase_sens():
    noisy_sine = gen_noise(size=10000) +0.1*sine_wave(100, fs=10000)
    noisy_135ph = noisy_sine* sine_wave(100, phase=135, fs=10000)
    noisy_90ph = noisy_sine * sine_wave(100, phase=90, fs=10000)
    #noisy_45ph = gen_noise(size=20000) + sine_wave(200, phase=45, fs=20000)

    plt.plot(bw_low_pass(10000, 10, 1, noisy_135ph), 'r', label="Noisy 100Hz Sine wave; 135 rad.")
    plt.plot(bw_low_pass(10000, 10, 1, noisy_90ph), 'b', label="Noisy 100Hz Sine wave; 90 rad")
    #plt.plot(bw_low_pass(20000, 5, 1, noisy_45ph))
    plt.gca().set_title("Phase Sensitivity of a Lock-in Amplifier")

    plt.legend()
    plt.show()

#runs the program
def main():
    show_lock_in()
    #show_phase_sens()

#Initializes main and runs program
if __name__ == "__main__":
    main()

