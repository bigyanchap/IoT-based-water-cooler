void tempCtrl(){
  double prevTemp=temp;
  sensors.requestTemperatures();
  temp=sensors.getTempCByIndex(0);
  if(temp<-10||temp==85){//Hiding the innate error of sensor.
    temp=prevTemp;
  }
  /*The sensor has an innate problem.
  Sometimes, it automatically defaults to -127 or 85, 
  which, consequently, does unwanted switching: its hardware error.*/
  if(temp>userSetTemp){
      digitalWrite(driverPin,HIGH);//turn on Thermo-electric-cooler system
      systemStatus="Cooling...";
  }
  else if(temp<=userSetTemp)
  {
    delay(20);//switching relay=transient brown-out
    digitalWrite(driverPin,LOW);
    systemStatus="Ready to use.";
  }
//  disp();
}

