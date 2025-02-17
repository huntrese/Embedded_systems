#include "io.h"

String io::input() {
    String input = "";
    char ch;

    while (true) {
        if (Serial.available()) {
            ch = Serial.read();

            if (ch == '\n') break; // Stop on Enter
            
            if (ch == '\b' || ch == 127) { // Handle Backspace (127 is DEL key)
                if (input.length() > 0) {
                    input.remove(input.length() - 1);
                    Serial.print("\b \b"); // Erase character in Serial Monitor
                }
            } else {
                input += ch;
                Serial.print(ch);
            }
        }
    }
    Serial.print("\n");

    input.trim();
    return input;
}


// String scanff() {
//     String input = "";
//     while (!Serial.available()) {
//         // Wait for input
//     }

//     input = Serial.readStringUntil('\n');
//     input.trim();  // Remove any trailing newline or spaces

//     return input;  // Return the input as a string
// }