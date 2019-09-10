# Circuit-Fault-Detection
Fault Detection of Single Tube Amplifier Circuit For 2019 TI Cup

## Utilization of on-board resources£º

	Usart1£ºPA.9,10

	KEY£ºPA.0
	     PE.3,4

	LED£ºPF.9,10

	AD9910£ºPA.0~7
		PC.3,4

	AD7606£ºPA.1~7
		PF.0~7,15
		PG.0~7
    
    LCD: PB.15
         PD.0,1,4,5,8,9,10,14,15 
         PE.7~15 
         PF.12 
         PG.12 
         
## Functional realization
 > * It can generate sine wave, square wave and other waveforms, and realize the output of point frequency, sweep frequency, etc.
 > * AD readout accuracy is 24-bit accuracy, Sampling rate can be changed
 > * FFT transform and FIR filter have been applied to the signal.
 > * It can measure the input and output resistance, gain and cut-off frequency of the circuit.
 > * It can detect circuit faults of single-tube amplifier circuit.
 > * External or internal PWM signals can be input