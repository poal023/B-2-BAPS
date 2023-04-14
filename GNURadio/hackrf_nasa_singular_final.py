#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Hackrf Nasa Singular Final
# GNU Radio version: 3.10.1.0

from packaging.version import Version as StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import blocks
from gnuradio import digital
from gnuradio import fft
from gnuradio.fft import window
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import zeromq
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import osmosdr
import time



from gnuradio import qtgui

class hackrf_nasa_singular_final(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Hackrf Nasa Singular Final", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Hackrf Nasa Singular Final")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "hackrf_nasa_singular_final")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 4e6
        self.transition_width = transition_width = samp_rate/8
        self.gain0 = gain0 = 15
        self.freq0 = freq0 = 2e9
        self.cutoff_f0 = cutoff_f0 = samp_rate/4

        ##################################################
        # Blocks
        ##################################################
        self._gain0_range = Range(0, 47, 1, 15, 200)
        self._gain0_win = RangeWidget(self._gain0_range, self.set_gain0, "gain0", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._gain0_win)
        self.zeromq_pub_sink_0 = zeromq.pub_sink(gr.sizeof_gr_complex, 1, 'tcp://127.0.0.1:5566', 100, False, -1, '')
        self._transition_width_range = Range(samp_rate/16, samp_rate/4, 10e3, samp_rate/8, 200)
        self._transition_width_win = RangeWidget(self._transition_width_range, self.set_transition_width, "transition_width", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._transition_width_win)
        self.qtgui_time_sink_x_0_0 = qtgui.time_sink_f(
            1024, #size
            samp_rate, #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0.set_update_time(0.00010)
        self.qtgui_time_sink_x_0_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0.enable_tags(True)
        self.qtgui_time_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0.enable_stem_plot(False)


        labels = ['Time Domain', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_0_win)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_f(
            1024, #size
            window.WIN_BLACKMAN_hARRIS, #wintype
            freq0, #fc
            samp_rate, #bw
            "", #name
            1,
            None # parent
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)
        self.qtgui_freq_sink_x_0.set_fft_window_normalized(False)


        self.qtgui_freq_sink_x_0.set_plot_pos_half(not True)

        labels = ['9dc3 Output', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.osmosdr_source_1 = osmosdr.source(
            args="numchan=" + str(1) + " " + 'hackrf=0000000000000000f77c60dc2a139dc3'
        )
        self.osmosdr_source_1.set_time_now(osmosdr.time_spec_t(time.time()), osmosdr.ALL_MBOARDS)
        self.osmosdr_source_1.set_sample_rate(samp_rate )
        self.osmosdr_source_1.set_center_freq(freq0, 0)
        self.osmosdr_source_1.set_freq_corr(0, 0)
        self.osmosdr_source_1.set_dc_offset_mode(2, 0)
        self.osmosdr_source_1.set_iq_balance_mode(2, 0)
        self.osmosdr_source_1.set_gain_mode(True, 0)
        self.osmosdr_source_1.set_gain(gain0, 0)
        self.osmosdr_source_1.set_if_gain(24, 0)
        self.osmosdr_source_1.set_bb_gain(24, 0)
        self.osmosdr_source_1.set_antenna('RX', 0)
        self.osmosdr_source_1.set_bandwidth(10e6, 0)
        self.fft_vxx_0 = fft.fft_vcc(1024, True, window.blackmanharris(1024), True, 1)
        self.digital_mpsk_snr_est_cc_0 = digital.mpsk_snr_est_cc(1, 10000, 0.001)
        self._cutoff_f0_range = Range(samp_rate/8, samp_rate/2, 10e3, samp_rate/4, 200)
        self._cutoff_f0_win = RangeWidget(self._cutoff_f0_range, self.set_cutoff_f0, "cutoff", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._cutoff_f0_win)
        self.blocks_vector_to_stream_0_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1024)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_float*1, 1024)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate / 8,True)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1024)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_vector_to_stream_0, 0))
        self.connect((self.blocks_stream_to_vector_0, 0), (self.fft_vxx_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.digital_mpsk_snr_est_cc_0, 0))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.blocks_vector_to_stream_0, 0), (self.qtgui_time_sink_x_0_0, 0))
        self.connect((self.blocks_vector_to_stream_0_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.digital_mpsk_snr_est_cc_0, 0), (self.zeromq_pub_sink_0, 0))
        self.connect((self.fft_vxx_0, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self.fft_vxx_0, 0), (self.blocks_vector_to_stream_0_0, 0))
        self.connect((self.osmosdr_source_1, 0), (self.blocks_stream_to_vector_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "hackrf_nasa_singular_final")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_cutoff_f0(self.samp_rate/4)
        self.set_transition_width(self.samp_rate/8)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate / 8)
        self.osmosdr_source_1.set_sample_rate(self.samp_rate )
        self.qtgui_freq_sink_x_0.set_frequency_range(self.freq0, self.samp_rate)
        self.qtgui_time_sink_x_0_0.set_samp_rate(self.samp_rate)

    def get_transition_width(self):
        return self.transition_width

    def set_transition_width(self, transition_width):
        self.transition_width = transition_width

    def get_gain0(self):
        return self.gain0

    def set_gain0(self, gain0):
        self.gain0 = gain0
        self.osmosdr_source_1.set_gain(self.gain0, 0)

    def get_freq0(self):
        return self.freq0

    def set_freq0(self, freq0):
        self.freq0 = freq0
        self.osmosdr_source_1.set_center_freq(self.freq0, 0)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.freq0, self.samp_rate)

    def get_cutoff_f0(self):
        return self.cutoff_f0

    def set_cutoff_f0(self, cutoff_f0):
        self.cutoff_f0 = cutoff_f0




def main(top_block_cls=hackrf_nasa_singular_final, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
