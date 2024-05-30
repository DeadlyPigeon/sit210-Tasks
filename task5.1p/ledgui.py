import RPi.GPIO as GPIO
import tkinter as tk

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

PINS = {
   "blue": 10,
   "green": 12,
   "red": 8
}

GPIO.setup(10, GPIO.OUT)
GPIO.setup(12, GPIO.OUT)
GPIO.setup(8, GPIO.OUT)

def led_on(select):
    for colour, pin in PINS.items(): 
        if colour == select: #check if selected colour from radiobuttons is that colour
           GPIO.output(pin, GPIO.HIGH) #turn of it is, matching to GPIO pin in dict
        else:
           GPIO.output(pin, GPIO.LOW) #turn rest off



def exit():
    GPIO.cleanup()
    gui.destroy()

gui = tk.Tk()
gui.title("LED control")
select = tk.StringVar()
select.set("red")
for colour in PINS.keys():
    tk.Radiobutton(gui, text=colour, variable=select, value=colour, command=lambda: led_on(select.get())).pack(anchor=tk.W) #create button for each colour, make button = select variable, set value to the colour its on
tk.Button(gui, text="exit", command=exit).pack()
gui.mainloop()