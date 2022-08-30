void disp(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Actual Temp:");
  display.print(round(temp*100)/100);//truncating
  display.println("C");
  display.setCursor(0,10);
  display.print("User Set Temp:");
  display.print(userSetTemp);
  display.println("C");
  display.setCursor(0,20);
  display.println("System Status: ");
  display.setCursor(0,30);
  display.print(systemStatus);
  display.setCursor(0,40);
  display.print("WiFi Status:");
  display.setCursor(0,50);
  display.println(wifiStatus);
  display.display();
}

