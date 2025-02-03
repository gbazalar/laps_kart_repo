# 🚗💨 LAPs Autonomous Kart Project - Hardware Team 
Repository for the Autonomous Kart Project in LAPS Clubs  
Microcontroller	STM32F4 (e.g., NUCLEO-F446RE)  

## 📡 Module 1: PWM Signal Reader
This is the first module of the LAPs Autonomous Kart Project, designed to capture and measure PWM signals using Timer 2 (TIM2) on the STM32F4 microcontroller. The module reads pulse width and frequency of an incoming PWM signal via GPIOA, Pin PA0 (TIM2 Channel 1), which will later be used for motor control and other signal processing tasks in the autonomous kart.

### 🔧 Module Overview  
✔️ Captures PWM Signals using TIM2  
✔️ Measures Pulse Width & Frequency via interrupts  

### 🛠️ Hardware & Configuration  
> [!NOTE]
> TIM2 Channel 1 (CH1), PIN PA0 (AF1 - TIM2_CH1), IntHandler TIM2_IRQHandler  

### 📡 PWM Capture Details  
1️⃣ TIM2 is configured to capture PWM input from PA0.  
2️⃣ Rising & Falling edges are detected using the capture interrupt.  
3️⃣ Pulse Width is calculated using TIM2 counter values.  
4️⃣ The LED on PA5 blinks to indicate active measurement.  

### 🔥 Future Enhancements  
✨ UART Logging for real-time data output  
✨ Low-Power Modes for energy-efficient operation  

