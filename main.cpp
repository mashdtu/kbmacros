#include <bits/stdc++.h>
#include <windows.h>
#include <winuser.h>

// Declare variables to use for emulating keypresses.
KEYBDINPUT kb={0};
INPUT Input={0};

void simulateKeyPress ( char key, int modifier ) {

    // Lift the modifier key up
    kb.wScan = modifier;
    kb.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    ::SendInput(1, &Input, sizeof(Input));

    // Press the key character down
    kb.wScan = key;
    kb.dwFlags = KEYEVENTF_UNICODE;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    ::SendInput(1, &Input, sizeof(Input));

    // Lift the key character back up
    kb.wScan = key;
    kb.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    ::SendInput(1, &Input, sizeof(Input));
};

void commitKeybind ( int key, int modifier ) {

    // Switch-case for the 3 different keybinds.
    switch ( key ) {
    case 0xBB:
        simulateKeyPress( '\\', modifier );
        break;
    case 0xBC:
        simulateKeyPress( '<', modifier );
        break;
    case 0xBD:
        simulateKeyPress( '>', modifier );
        break;
    }
};

int main () {

    // Variables for the currently active key and modifier.
    int activeKey, activeModifier;

    // Buffer keypress to avoid spam.
    int bufferKey;

    // Boolean to control the buffer.
    bool anyKeyPressed;

    // Start a loop to listen for keypresses.
    while ( true ) {

        // Reset all the previously declared variables.
        activeModifier = 0;
        activeKey = 0;
        anyKeyPressed = false;

        // Loop through all possible keys on the keyboard.
        for ( int key = 8; key < 190; key ++ ) {
            
            // Test if a key has been pressed, activeKey must be 0 to avoid multiple keybinds at the same time.
            if ( ( GetAsyncKeyState ( key ) & 0x8000 ) && activeKey == 0 ) {
                
                // The key must be part of a keybind and not equal to the buffer key, as to avoid spam.
                if ( ( key == 0xBB || key == 0xBC || key == 0xBD ) && key != bufferKey ) {

                    // Set the active key and buffer key to the pressed key.
                    activeKey = key, bufferKey = key;

                // Test for the key being a modifier of a keybind, this cannot be spammed and thus there is no buffer.
                } else if ( ( key == 0x11 || key == 0x12 ) && activeModifier == 0 ) {

                    // Set the active modifier key to the pressed modifier key.
                    activeModifier = key;
                }

                // Detect if any of the keys across every keybind is pressed.
                if ( key == 0xBB || key == 0xBC || key == 0xBD ) {

                    // If any of them is pressed the variable is set to be true, this is again to avoid spam.
                    anyKeyPressed = true;
                }
            }
        }

        // If a key and a modifier has been pressed it is sent to another function to be commited.
        if ( activeKey != 0 && activeModifier != 0 ) {
            commitKeybind ( activeKey, activeModifier );
        }

        // If none of the keys were pressed the buffer key is set to the active key.
        if ( ! anyKeyPressed ) {
            bufferKey = activeKey;
        }
    }

    // The program runs in an infinite loop and will never terminate.
    return 0;
}