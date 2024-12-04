Repacked for ATTinyCore 1.5.0 and later by Spence Konde / Azduino using the
binaries made available by @ArminJo at
https://github.com/ArminJo/DigistumpArduino/archive/1.7.2.tar.gz

Changes:
* Replaced DigiUSB and DigiX drivers from the 2.0a4 package, based on claims
that CDC was dead as a doornail and is unlikely to live again, while DigiUSB
still works. ArminJo had removed these from his release, beliving them to
not be used (to be fair, the documentation for the CDC libraries is not
ambiguous about this, it says all over the place to not use DigiUSB. But that
was written during the brief window of time when DigiCDC worked, and they just
haven't been updated since.
* Replaced post_install.bat. It won't *work* but the error gives the user a
file path to persue and an error to search for, rather than just wondering
why their digispark isn't detected. Without the hint, most users are unlikely
to locate the drivers that get installed 4+ levels deep inside a hidden
folder (AppData)
