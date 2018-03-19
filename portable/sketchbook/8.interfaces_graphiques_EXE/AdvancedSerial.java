package advancedserial;

import java.awt.Desktop;
import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;
import processing.app.Editor;
import processing.app.tools.Tool;


public class AdvancedSerial implements Tool {
  Editor editor;

  public void init(Editor editor) {
    this.editor = editor;
  }

  public String getMenuTitle() {
    return "Advanced Serial Client";
  }

  public void run() {
	  try {
	  Runtime.getRuntime().exec("AdvancedSerialClient.exe", null, new File(System.getProperty("AdvancedSerialClient.dir")));
    } catch (IOException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }
  }
}
