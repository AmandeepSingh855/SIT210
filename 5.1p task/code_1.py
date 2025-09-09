import tkinter as tk
from gpiozero import LED

led_map = {
    "Red": LED(17),
    "Yellow": LED(27),
    "Green": LED(22),
}

def set_led(choice):
    for name, led in led_map.items():
        if name == choice:
            led.on()
        else:
            led.off()

def on_exit():
    for led in led_map.values():
        led.off()
    root.destroy()

root = tk.Tk()
root.title("RPi LED Controller")

title = tk.Label(root, text="Select a color to light the LED", font=("Arial", 14))
title.pack(padx=16, pady=(16, 8))

selection = tk.StringVar(value="Red")

for color in ("Red", "Yellow", "Green"):
    rb = tk.Radiobutton(
        root,
        text=color,
        variable=selection,
        value=color,
        command=lambda: set_led(selection.get()),
        font=("Arial", 12)
    )
    rb.pack(anchor="w", padx=24, pady=4)

exit_btn = tk.Button(root, text="Exit", command=on_exit, font=("Arial", 12))
exit_btn.pack(pady=16)

set_led(selection.get())
root.protocol("WM_DELETE_WINDOW", on_exit)
root.mainloop()
