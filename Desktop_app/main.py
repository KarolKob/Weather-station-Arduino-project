# https://github.com/Kebablord

import urllib.request
import PySimpleGUI as sg

url = "http://192.168.8.117/"  # ESP's IP, ex: http://x (Esp prints it to serial console when connected to wifi)

tempC = "Temperature in °C: "
hum = "Humidity in %: "
br_val = ", value: "

layout = [[sg.Text(tempC + "?", size=(60, 1), font='Arial 15', key='-temp-')],
          [sg.Text(hum + "?", size=(60, 1), font='Arial 15', key='-hum-')],
          [sg.Text("?" + br_val + "?", size=(60, 1), font='Arial 15', key='-br-')],
          [sg.Button("UPDATE")]]

# Create the window
window = sg.Window("Current Weather", layout, size=(350, 150))


def get_data():
    global data, temp_val, hum_val, bri_val, bri_txt, reading_successful

    n = urllib.request.urlopen(url).read()  # get the raw html data in bytes (sends request and warn our esp8266)
    n = n.decode("utf-8")  # convert raw html bytes format to string :3

    if n != "":
        data = n.split(" ")

        bri_val = data[2]
        bri_txt = data[3]
        hum_val = data[4]
        temp_val = data[5]
        reading_successful = 1
    else:
        data = n
        reading_successful = 0


# Example usage
while True:
    event, values = window.read()
    # End program if user closes window or
    # presses the OK button
    if event == sg.WINDOW_CLOSED:
        break
    elif event == "UPDATE":
        get_data()
        if reading_successful == 1:
            window['-temp-'].update(tempC + temp_val)
            window['-hum-'].update(hum + hum_val)
            window['-br-'].update(bri_txt + br_val + bri_val)

window.close()
