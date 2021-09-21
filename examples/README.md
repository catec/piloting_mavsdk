## **Examples**

All examples consist of two executables: one that simulates the gRCS and one that simulates the robotic platform. To test each example you have to run each executable in a different terminal.

---

### **piloting_heartbeat**

- piloting_gcs_heartbeat_example
   - Establishes the connection and manages the heartbeat

- piloting_robot_heartbeat_example
   - Establishes the connection and manages the heartbeat

---

### **piloting_telemetry**

- piloting_gcs_telemetry_example
   - Establishes the connection and manages the heartbeat
   - Subcribes to the robot telemetry and prints data when arrives

- piloting_robot_telemetry_example
   - Establishes the connection and manages the heartbeat
   - Sends fake telemetry

---

### **piloting_inspection**

- piloting_gcs_inspection_example
   - Establishes the connection and manages the heartbeat
   - Creates an inspection and sends it to the robot
   - Waits 4 secs and downloads the inspection from the robot
   - Shows the inspection progress

- piloting_robot_inspection_example
   - Establishes the connection and manages the heartbeat
   - Receives an inspection an modifies it
   - Uploads the inspection
   - Simulates the inspection progress

---

### **piloting_command**

- piloting_gcs_command_example
   - Establishes the connection and manages the heartbeat
   - Sends a command a receives the ACK from the robot (x2)

- piloting_robot_command_example
   - Establishes the connection and manages the heartbeat
   - Waits for commands and sends ACK when they arrive

---

### **piloting_checklist**

- piloting_gcs_checklist_example
   - Establishes the connection and manages the heartbeat
   - Receives the checklist from the robot

- piloting_robot_checklist_example
   - Establishes the connection and manages the heartbeat
   - Creates and sends the checklist

---

### **piloting_alarm**

- piloting_gcs_alarm_example
   - Establishes the connection and manages the heartbeat
   - Receives the alarms list from the robot
   - Receives periodically the alarms status

- piloting_robot_alarm_example
   - Establishes the connection and manages the heartbeat
   - Creates and sends the alarms list to the gRCS
   - Sends periodically the alarms status