=========================
GNU Radio Channel Sounder
=========================

This is a GNU Radio implementation of a correlation channel sounder, although still in alpha stage^^. This implementation works reasonably well if the transmitting USRP, as well as the receiving USRP are both equipped with Ettus' GPS-disciplined-oscillator for synchronising their oscillators.
No timing synchronisation is currently done, neither is any pulse-shaping at the receiver or transmitter.
A graphical sink at the receiver displays the channel impulse response, the Doppler spectrum and the power delay profile.
This GUI requires PyQT4 and pyQwt5, as well as the GNU Radio python blocks.
Additionally a separate GUI, using the MayaVi Data Visualizer, was written for further off-line analysis of channel sounder measurements.
The channel sounder was ported to GNU Radio 3.7 and was last tested with 3.7.2, although only with gr-channel and not 2 USRPs, so the respective flowgraphs might need a little tweaking.
Comments and suggestions are always welcome.

Dependencies
============
* GNU Radio 3.7 or later
* since Python Blocks are now an incremental part of GNU Radio, grextras is no longer needed
* PyQT4 (package pyqt4-dev-tools under Ubuntu)
* python bindings of Qwt5 (package python-qwt5-qt4 and libqwt5-qt4 under Ubuntu)
* optional: the GUI for off-line analysis requires MayaVi (package mayavi2 under Ubuntu)

Usage with USRPs
================
As mentioned earlier the frequencies of the transmitting and receiving USRPs have to be synchronised to accurately measure the Doppler spectrum.
The maximum sample rate is 20 Msamples/sec.
For even decimation/interpolation factors the digital up- and down-converter-chains somehow distort the signal, hereby degrading the performance.
This is not the case if uneven interpolation- and decimation-factors are used, since the then employed decimation/interpolation CIC filters do not affect the signal.

Examples
========
The flowgraphs that I used for my outdoor measurements can be found in apps/corr_rx_usrp.grc and apps/corr_tx_usrp.grc.
The function probe blocks in these flowgraphs can be used to periodically check the status of the GPSDO.
Under apps/channelsounder_virt_example.grc the channelsounder is used to sound a virtual channel based on the gr-channel-model block.

ToDo
====
These are things that would be nice, but I currently don't have the time to implement; but they are on my list.
* An FPGA implementation of the pseudo random number generator and the correlator
* Add symbol timing synchronization
* Add a phase-locked loop to the receiver so that a GPSDO is not necessary, although it is then impossible to measure the Doppler spectrum
