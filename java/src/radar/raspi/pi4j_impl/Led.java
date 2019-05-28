package radar.raspi.pi4j_impl;

import java.io.IOException;

import com.pi4j.io.gpio.*;

import radar.raspi.devices.*;


public class Led implements Light {
	private int pinNum;
	private GpioPinDigitalOutput pin;
	
	public Led(int pinNum){

		this.pinNum = pinNum;
		try {
		    GpioController gpio = GpioFactory.getInstance();
		    pin = gpio.provisionDigitalOutputPin(Config.pinMap[pinNum]);		    
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public synchronized void switchOn() throws IOException {
		pin.high();
	}

	@Override
	public synchronized void switchOff() throws IOException {
		pin.low();
	}
	
}
