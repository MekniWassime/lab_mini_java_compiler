from tkinter import *
import subprocess
window = Tk()
window.title("mini java parser")


def click():
    with open('./ex.txt', 'w') as f:
        f.write(code_input.get("1.0", "end"))
    result = subprocess.check_output(["minijavaparser3.exe", "<", "ex.txt"], stderr=subprocess.STDOUT, shell=True)
    result = result.decode('cp850').strip()
    if(not result):
        result2 = subprocess.check_output(["semantic.exe", "ex.txt"], stderr=subprocess.STDOUT, shell=True)
        result2 = result2.decode('cp850').strip()
        result = result + "\n" + result2
    # result = subprocess.check_output(["semantic.exe", "ex.txt"], stderr=subprocess.STDOUT, shell=True)
    # result = result.decode('cp850').strip()
    output.config(state="normal")
    output.delete("1.0","end")
    output.insert(0.0, result)
    output.config(state="disabled")

code_input = Text(window, width=100, height=20)
with open('./ex.txt', 'r') as f:
    code_input.insert(0.0, f.read())

output = Text(window, width=100, height= 25, state="disabled")

submit_button = Button(window, text="Run Parser", command=click, width=100)

code_input.grid(row=0)
output.grid(row=2) 
submit_button.grid(row=1)

window.mainloop()