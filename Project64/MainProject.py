#!/usr/bin/env python3
import tkinter as tk
from std_msgs.msg import UInt16, Bool
from sensor_msgs.msg import JointState
import rospy

class PotentiometerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Potentiometer and Motor Control GUI")
        
        # Potentiometer Labels
        self.label1 = tk.Label(root, text="Potentiometer 1 Value:")
        self.label1.pack()

        self.pot1_value_var = tk.StringVar()
        self.pot1_value_label = tk.Label(root, textvariable=self.pot1_value_var)
        self.pot1_value_label.pack()

        self.label2 = tk.Label(root, text="Potentiometer 2 Value:")
        self.label2.pack()

        self.pot2_value_var = tk.StringVar()
        self.pot2_value_label = tk.Label(root, textvariable=self.pot2_value_var)
        self.pot2_value_label.pack()

        # Motor Control GUI
        self.setup_motor_control_gui()

        # ROS Initialization
        rospy.init_node('combined_gui', anonymous=True)
        rospy.Subscriber('pot1_value', UInt16, self.pot1_callback)
        rospy.Subscriber('pot2_value', UInt16, self.pot2_callback)

    def pot1_callback(self, data):
        pot_value = data.data
        mapped_value = self.map_value(pot_value, 2600, 5000, -82.5, 82.5)
        rounded_value = round(mapped_value, 2)
        self.pot1_value_var.set(str(rounded_value))

    # Check if pot2_value_var is not an empty string before converting to float
        pot2_value_str = self.pot2_value_var.get()
        if pot2_value_str:
            self.send_joint_states_to_arduino(mapped_value, float(pot2_value_str))

    def pot2_callback(self, data):
        pot_value = data.data
        mapped_value = self.map_value(pot_value, 0, 20, -55, 55)
        rounded_value = round(mapped_value, 2)
        self.pot2_value_var.set(str(rounded_value))

    # Check if pot1_value_var is not an empty string before converting to float
        pot1_value_str = self.pot1_value_var.get()
        if pot1_value_str:
            self.send_joint_states_to_arduino(float(pot1_value_str), mapped_value)

    def map_value(self, value, in_min, in_max, out_min, out_max):
        return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


    def setup_motor_control_gui(self):
        self.speed_scale_motor1 = tk.Scale(self.root, from_=0, to=255, orient=tk.HORIZONTAL, label="MOTOR1", command=self.on_scale_change_motor1)
        self.speed_scale_motor1.pack(pady=20)

        self.motor1_label = tk.Label(self.root, text="MOTOR1: 0")
        self.motor1_label.pack(pady=10)

        self.speed_scale_motor2 = tk.Scale(self.root, from_=0, to=255, orient=tk.HORIZONTAL, label="MOTOR2", command=self.on_scale_change_motor2)
        self.speed_scale_motor2.pack(pady=20)

        self.motor2_label = tk.Label(self.root, text="MOTOR2: 0")
        self.motor2_label.pack(pady=10)

        self.led_state_var = tk.BooleanVar()
        self.led_button = tk.Button(self.root, text="Reverse", command=self.toggle_led)
        self.led_button.pack(pady=10)

        self.mode_state_var = tk.BooleanVar()
        self.mode_button = tk.Button(self.root, text="Mode", command=self.toggle_mode)
        self.mode_button.pack(pady=10)

        self.home_state_var = tk.BooleanVar()
        self.home_button = tk.Button(self.root, text="HOME", command=self.toggle_home)
        self.home_button.pack(pady=10)

    def on_scale_change_motor1(self, value):
        speed_value_motor1 = int(value)
        self.motor1_label.config(text=f"MOTOR1: {speed_value_motor1}")
        speed_value_motor2 = int(self.speed_scale_motor2.get())
        led_state = int(self.led_state_var.get())
        
        self.send_speed_to_arduino(speed_value_motor1, speed_value_motor2, led_state)

    def on_scale_change_motor2(self, value):
        speed_value_motor2 = int(value)
        self.motor2_label.config(text=f"MOTOR2: {speed_value_motor2}")
        speed_value_motor1 = int(self.speed_scale_motor1.get())
        led_state = int(self.led_state_var.get())
        
        self.send_speed_to_arduino(speed_value_motor1, speed_value_motor2, led_state)

    def toggle_led(self):
        led_state = not self.led_state_var.get()
        self.send_speed_to_arduino(0, 0, int(led_state))
        self.led_state_var.set(led_state)

    def toggle_mode(self):
        mode_state = not self.mode_state_var.get()
        self.send_mode_to_arduino(mode_state)
        self.mode_state_var.set(mode_state)
    def toggle_home(self):
        home_state = not self.home_state_var.get()
        self.send_home_to_arduino(home_state)
        self.home_state_var.set(home_state)
    
        if home_state:  # If home_state is True (button is pressed)
            # Set both motor speeds to 0
            self.speed_scale_motor1.set(0)
            self.speed_scale_motor2.set(0)
            # Update the labels accordingly
            self.motor1_label.config(text="MOTOR1: 0")
            self.motor2_label.config(text="MOTOR2: 0")
        
            # Send speed values 0 to the Arduino
            self.send_speed_to_arduino(0, 0, int(self.led_state_var.get()))


    def send_mode_to_arduino(self, mode_state):
        rospy.Publisher('mode_state', Bool, queue_size=10).publish(mode_state)

    def send_home_to_arduino(self, home_state):
        rospy.Publisher('home_state', Bool, queue_size=10).publish(home_state)
        
    def send_speed_to_arduino(self, speed_value_motor1, speed_value_motor2, led_state):
        rospy.Publisher('control_motor1', UInt16, queue_size=10).publish(speed_value_motor1)
        rospy.Publisher('control_motor2', UInt16, queue_size=10).publish(speed_value_motor2)
        rospy.Publisher('revers', Bool, queue_size=10).publish(led_state)

    def send_joint_states_to_arduino(self, joint1_value, joint2_value):
        joint_state_msg = JointState()
        joint_state_msg.header.stamp = rospy.Time.now()
        joint_state_msg.name = ["joint1", "joint2"]
        joint_state_msg.position = [joint1_value, joint2_value]
        joint_state_msg.velocity = []  # You can add velocity values if needed
        joint_state_msg.effort = []  # You can add effort values if needed

        rospy.Publisher('joint_states', JointState, queue_size=10).publish(joint_state_msg)



if __name__ == '__main__':
    root = tk.Tk()
    app = PotentiometerGUI(root)
    root.mainloop()
