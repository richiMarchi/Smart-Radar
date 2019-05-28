package radar.raspi.serial;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import radar.raspi.common.MsgEvent;
import radar.raspi.common.Observable;
import radar.raspi.main.PositionInfoMsg;
import java.io.*;

/**
 * Comm channel implementation based on serial port.
 * 
 * @author aricci
 *
 */
public class SerialCommChannel extends Observable implements Serial, SerialPortEventListener {

	private SerialPort serialPort;
	private BufferedReader input;
	private OutputStream output;

	public SerialCommChannel(String port, int rate) throws Exception {

		CommPortIdentifier portId = CommPortIdentifier.getPortIdentifier(port);
		SerialPort serialPort = (SerialPort) portId.open(this.getClass().getName(), 2000);

		serialPort.setSerialPortParams(rate,
				SerialPort.DATABITS_8,
				SerialPort.STOPBITS_1,
				SerialPort.PARITY_NONE);

		input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
		output = serialPort.getOutputStream();

		serialPort.addEventListener(this);
		serialPort.notifyOnDataAvailable(true);

	}

	@Override
	public void sendMsg(String msg) {
		char[] array = (msg+"\n").toCharArray();
		byte[] bytes = new byte[array.length];
		for (int i = 0; i < array.length; i++){
			bytes[i] = (byte) array[i];
		}
		try {
			output.write(bytes);
			output.flush();
		} catch(Exception ex){
			ex.printStackTrace();
		}
	}

	/**
	 * This should be called when you stop using the port.
	 * This will prevent port locking on platforms like Linux.
	 */
	public synchronized void close() {
		if (serialPort != null) {
			serialPort.removeEventListener();
			serialPort.close();
		}
	}

	/**
	 * Handle an event on the serial port. Read the data and print it.
	 */
	public synchronized void serialEvent(SerialPortEvent oEvent) {
		if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
			try {
				String in=input.readLine();
				notifyEvent(new MsgEvent(createPositionInfoMsg(in)));
			} catch (Exception e) {
				System.err.println(e.toString());
			}
		}
	}

	private static PositionInfoMsg createPositionInfoMsg(String in) {
	    String[] parts = in.split(";");
            return new PositionInfoMsg(Integer.parseInt(parts[0]), Float.parseFloat(parts[1]));
	}
}
