package radar.raspi.main;

import java.io.FileOutputStream;
import java.io.PrintStream;
import radar.raspi.devices.Light;
import radar.raspi.devices.ObservableButton;
import radar.raspi.pi4j_impl.Button;
import radar.raspi.pi4j_impl.Led;
import radar.raspi.serial.SerialCommChannel;


public class Main {

    private static String commPort;
    private static int dataRate;
    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.err.println("args: <CommPortName> <BaudRate>");
            System.exit(1);
        }
        commPort = args[0];
        dataRate = Integer.parseInt(args[1]);
        System.err.println("Start monitoring port " + commPort + " at " + dataRate + " baud");

        final Light ledOn = new Led(13);
        final Light ledDetected = new Led(12);
        final Light ledTracking = new Led(11);
        final ObservableButton btnOn = new Button(10);
        final ObservableButton btnOff = new Button(7);

        final String logPath = System.getProperty("user.home") + System.getProperty("file.separator") + "smart_radar.log";
        final PrintStream logger = new PrintStream(new FileOutputStream(logPath, true), true);
        
        final RadarController controller = new RadarController(ledOn, ledDetected, ledTracking, btnOn, btnOff, logger);
        System.out.println("Waiting Arduino for rebooting...");
        Thread.sleep(4000);
        System.out.println("Ready.");
        controller.start();    
    }
    
    public static SerialCommChannel getSerialCommChannel() {
        try {
            return new SerialCommChannel(commPort, dataRate);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
        return null;
    }

}
