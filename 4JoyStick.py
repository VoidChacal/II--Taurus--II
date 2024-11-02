import serial
import pyautogui
import time

# Configurar a porta serial para o Arduino (substitua por 'COM5')
ser = serial.Serial('COM6', 115200, timeout=1)
time.sleep(2)

# Variáveis de estado para evitar o envio de comandos repetidos
is_w_pressed = False
is_a_pressed = False
is_s_pressed = False
is_d_pressed = False

def process_joystick(xc, yc):
    global is_w_pressed, is_a_pressed, is_s_pressed, is_d_pressed

    # Movimentação horizontal
    if xc == 0 and not is_a_pressed:  # Esquerda
        pyautogui.keyDown('a')
        is_a_pressed = True
        if is_d_pressed:  # Libera a tecla 'd' se estiver pressionada
            pyautogui.keyUp('d')
            is_d_pressed = False
    elif xc == 2 and not is_d_pressed:  # Direita
        pyautogui.keyDown('d')
        is_d_pressed = True
        if is_a_pressed:  # Libera a tecla 'a' se estiver pressionada
            pyautogui.keyUp('a')
            is_a_pressed = False
    elif xc == 1:  # Neutro, solta ambas as teclas 'a' e 'd'
        if is_a_pressed:
            pyautogui.keyUp('a')
            is_a_pressed = False
        if is_d_pressed:
            pyautogui.keyUp('d')
            is_d_pressed = False

    # Movimentação vertical
    if yc == 0 and not is_w_pressed:  # Para frente
        pyautogui.keyDown('w')
        is_w_pressed = True
        if is_s_pressed:  # Libera a tecla 's' se estiver pressionada
            pyautogui.keyUp('s')
            is_s_pressed = False
    elif yc == 2 and not is_s_pressed:  # Para trás
        pyautogui.keyDown('s')
        is_s_pressed = True
        if is_w_pressed:  # Libera a tecla 'w' se estiver pressionada
            pyautogui.keyUp('w')
            is_w_pressed = False
    elif yc == 1:  # Neutro, solta ambas as teclas 'w' e 's'
        if is_w_pressed:
            pyautogui.keyUp('w')
            is_w_pressed = False
        if is_s_pressed:
            pyautogui.keyUp('s')
            is_s_pressed = False

try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(data)  # Exibe os dados recebidos do Arduino no console

            # Interpreta os valores do Arduino
            try:
                parts = data.split(" ")
                xc = int(parts[5])  # XC: estado
                yc = int(parts[7])  # YC: estado
                
                # Processa o joystick
                process_joystick(xc, yc)
            except (IndexError, ValueError):
                print("Erro ao processar os dados")

        # Reduz o delay para tornar o movimento mais responsivo
        time.sleep(0.01)
except KeyboardInterrupt:
    print("Programa encerrado.")
finally:
    ser.close()
