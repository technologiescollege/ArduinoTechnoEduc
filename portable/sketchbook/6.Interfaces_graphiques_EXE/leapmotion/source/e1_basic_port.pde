import de.voidplus.leapmotion.*;
import processing.serial.*;
Serial myPort;
LeapMotion leap;
PImage logo;

void setup() {
  size(800, 500, JAVA2D);
  background(255);
  logo = loadImage("Logo509x81.png");
String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
myPort = new Serial(this, portName, 9600); 

  leap = new LeapMotion(this);
}

void draw() {
  background(255);
  image(logo, 364, 443);
  // ...
  int fps = leap.getFrameRate();
      text("main position.x : ",10,10);
      text("main position.y : ",10,20);
      text("main position.z : ",10,30);
      
      text("main rotation.x : ",10,40);
      text("main rotation.y : ",10,50);
      text("main rotation.z : ",10,60);
      
      text("angle du pouce : ",10,70);
      text("angle de l'index : ",10,80);
      text("angle du majeur : ",10,90);
      text("angle de l'annulaire : ",10,100);
      text("angle de l'auriculaire ",10,110);


  // ========= HANDS =========

  for (Hand hand : leap.getHands ()) {


    // ----- BASICS -----

    int     hand_id          = hand.getId();
    PVector hand_position    = hand.getPosition();
    PVector hand_stabilized  = hand.getStabilizedPosition();
    PVector hand_direction   = hand.getDirection();
    PVector hand_dynamics    = hand.getDynamics();
    float   hand_roll        = hand.getRoll();
    float   hand_pitch       = hand.getPitch();
    float   hand_yaw         = hand.getYaw();
    boolean hand_is_left     = hand.isLeft();
    boolean hand_is_right    = hand.isRight();
    float   hand_grab        = hand.getGrabStrength();
    float   hand_pinch       = hand.getPinchStrength();
    float   hand_time        = hand.getTimeVisible();
    PVector sphere_position  = hand.getSpherePosition();
    float   sphere_radius    = hand.getSphereRadius();


    // ----- SPECIFIC FINGER -----

    Finger  finger_thumb     = hand.getThumb();
    // or                      hand.getFinger("thumb");
    // or                      hand.getFinger(0);

    Finger  finger_index     = hand.getIndexFinger();
    // or                      hand.getFinger("index");
    // or                      hand.getFinger(1);

    Finger  finger_middle    = hand.getMiddleFinger();
    // or                      hand.getFinger("middle");
    // or                      hand.getFinger(2);

    Finger  finger_ring      = hand.getRingFinger();
    // or                      hand.getFinger("ring");
    // or                      hand.getFinger(3);

    Finger  finger_pink      = hand.getPinkyFinger();
    // or                      hand.getFinger("pinky");
    // or                      hand.getFinger(4);        


    // ----- DRAWING -----

    hand.draw();
    // hand.drawSphere();
    //  myPort.write("leapmotion:"+hand_position.x+":"+hand_position.y+":"+hand_position.z
    //  +":"+hand_stabilized.x+":"+hand_stabilized.y+":"+hand_stabilized.z
    //  +hand_roll+":"+hand_pitch +":"+hand_yaw );
    //  myPort.write("\n");
    
    
    hand_stabilized.x=map(hand_stabilized.x,0,800,0,1000);    
    hand_stabilized.y=map(hand_stabilized.y,0,500,1000,0);      
    hand_position.z=map(hand_position.z,-20,120,0,1000);
    
      text((int)hand_stabilized.x,130,10);
      text((int)hand_stabilized.y,130,20);
      text((int)hand_position.z,130,30);
      
      float rot_x=map(hand_roll,-180,180,0,1000);
      float rot_y=map(hand_yaw,-180,180,0,1000);
      float rot_z=map(hand_pitch,-180,180,0,1000);
      
      text((int)rot_x,130,40);
      text((int)rot_y,130,50);
      text((int)rot_z,130,60);
      
      
      rect(160,0,hand_stabilized.x/3,10);
      rect(160,10,hand_stabilized.y/3,10);
      rect(160,20,hand_position.z/3,10);
      
      
      rect(160,30,rot_x/3,10);
      rect(160,40,rot_y/3,10);
      rect(160,50,rot_z/3,10);
      
    //  String chaine ="<0="+hand_stabilized.x+"><1="+hand_stabilized.y+"><2="+hand_position.z+"><3="
    //+rot_x+"><4="+rot_y +"><5="+rot_z;
    myPort.write("<80="+(int)hand_stabilized.x+">");
    myPort.write("<81="+(int)hand_stabilized.y+">");
    myPort.write("<82="+(int)hand_stabilized.z+">");
    myPort.write("<83="+(int)rot_x+">");
    myPort.write("<84="+(int)rot_y+">");
    myPort.write("<85="+(int)rot_z+">");
    
    
    // ========= ARM =========

    if (hand.hasArm()) {
      Arm     arm               = hand.getArm();
      float   arm_width         = arm.getWidth();
      PVector arm_wrist_pos     = arm.getWristPosition();
      PVector arm_elbow_pos     = arm.getElbowPosition();
    }


    // ========= FINGERS =========

    for (Finger finger : hand.getFingers()) {
      // Alternatives:
      // hand.getOutstrechtedFingers();
      // hand.getOutstrechtedFingersByAngle();

      // ----- BASICS -----

      int     finger_id         = finger.getId();
      PVector finger_position   = finger.getPosition();
      PVector finger_stabilized = finger.getStabilizedPosition();
      PVector finger_velocity   = finger.getVelocity();
      PVector finger_direction  = finger.getDirection();
      float   finger_time       = finger.getTimeVisible();
      
      float angle;
      int a;

      // ----- SPECIFIC FINGER -----

      switch(finger.getType()) {
      case 0:
      a= (int)degrees(PVector.angleBetween(hand_direction, finger_direction));
      rect(160,60,a,10);
      text(a,130,70);
      myPort.write("<86="+a+">");
        // System.out.println("thumb");
        break;
      case 1:
      a= (int)degrees(PVector.angleBetween(hand_direction, finger_direction));
      rect(160,70,a,10);
      text(a,130,80);
      myPort.write("<87="+a+">");
        // System.out.println("index");
        break;
      case 2:
      angle= degrees(PVector.angleBetween(hand_direction, finger_direction));
      a=(int)map(angle,35,140,0,180);
      rect(160,80,a,10);
      text(a,130,90);
      myPort.write("<88="+a+">");
        // System.out.println("middle");
        break;
      case 3:
      a= (int)degrees(PVector.angleBetween(hand_direction, finger_direction));
      rect(160,90,a,10);
      text(a,130,100);
      myPort.write("<89="+a+">");
        // System.out.println("ring");
        break;
      case 4:
      a= (int)degrees(PVector.angleBetween(hand_direction, finger_direction));
      rect(160,100,a,10);
      text(a,130,110);
      myPort.write("<90="+a+">");
     
        // System.out.println("pinky");
        break;
      }


      // ----- SPECIFIC BONE -----

      Bone    bone_distal       = finger.getDistalBone();
      // or                       finger.get("distal");
      // or                       finger.getBone(0);

      Bone    bone_intermediate = finger.getIntermediateBone();
      // or                       finger.get("intermediate");
      // or                       finger.getBone(1);

      Bone    bone_proximal     = finger.getProximalBone();
      // or                       finger.get("proximal");
      // or                       finger.getBone(2);

      Bone    bone_metacarpal   = finger.getMetacarpalBone();
      // or                       finger.get("metacarpal");
      // or                       finger.getBone(3);


      // ----- DRAWING -----

       finger.draw(); // = drawLines()+drawJoints()
       finger.drawLines();
       finger.drawJoints();


      // ----- TOUCH EMULATION -----

      int     touch_zone        = finger.getTouchZone();
      float   touch_distance    = finger.getTouchDistance();

      switch(touch_zone) {
      case -1: // None
        break;
      case 0: // Hovering
        // println("Hovering (#"+finger_id+"): "+touch_distance);
        break;
      case 1: // Touching
        // println("Touching (#"+finger_id+")");
        break;
      }
    }


    // ========= TOOLS =========

    for (Tool tool : hand.getTools ()) {


      // ----- BASICS -----

      int     tool_id           = tool.getId();
      PVector tool_position     = tool.getPosition();
      PVector tool_stabilized   = tool.getStabilizedPosition();
      PVector tool_velocity     = tool.getVelocity();
      PVector tool_direction    = tool.getDirection();
      float   tool_time         = tool.getTimeVisible();


      // ----- DRAWING -----

      // tool.draw();


      // ----- TOUCH EMULATION -----

      int     touch_zone        = tool.getTouchZone();
      float   touch_distance    = tool.getTouchDistance();

      switch(touch_zone) {
      case -1: // None
        break;
      case 0: // Hovering
        // println("Hovering (#"+tool_id+"): "+touch_distance);
        break;
      case 1: // Touching
        // println("Touching (#"+tool_id+")");
        break;
      }
    }
  }


  // ========= DEVICES =========

  for (Device device : leap.getDevices ()) {
    float device_horizontal_view_angle = device.getHorizontalViewAngle();
    float device_verical_view_angle = device.getVerticalViewAngle();
    float device_range = device.getRange();
  }
}

// ========= CALLBACKS =========

void leapOnInit() {
  // println("Leap Motion Init");
}
void leapOnConnect() {
  // println("Leap Motion Connect");
}
void leapOnFrame() {
  // println("Leap Motion Frame");
}
void leapOnDisconnect() {
  // println("Leap Motion Disconnect");
}
void leapOnExit() {
  // println("Leap Motion Exit");
}
