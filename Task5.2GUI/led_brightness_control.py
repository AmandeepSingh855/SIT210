from gpiozero import PWMLED
import tkinter as tk

class LEDBrightnessControl:
    def __init__(self, root, pin=4):
        self.led = PWMLED(pin)

        root.title("LED Brightness Control")
        self.slider = tk.Scale(
            root,
            from_=0,
            to=100,
            orient=tk.HORIZONTAL,
            command=self.update_pwm
        )
        self.slider.pack(padx=20, pady=20)

    def update_pwm(self, val):
        """Update LED brightness (0 to 1)"""
        self.led.value = float(val) / 100

def main():
    root = tk.Tk()
    LEDBrightnessControl(root)
    root.mainloop()

if __name__ == "__main__":
    main()
